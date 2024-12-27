#include "MyVirtualCameraActor.h"

#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "ImageUtils.h"
#include "Async/Async.h"
#include "Misc/FileHelper.h"
#include "SensorCustomRenderAPI.h"

#include "UObject/ConstructorHelpers.h"

#include "Windows/AllowWindowsPlatformTypes.h"
#include <Windows.h>
#include <string>
#include <sstream>
#include<io.h>
#include <direct.h>
#include <shlobj.h>
#include <algorithm>
#include "Windows/HideWindowsPlatformTypes.h"

using namespace std;

FString DefaultImageOutPath;

#pragma optimize("", off)
string GetTime()
{
	time_t now = time(NULL);
	tm* tm_t = localtime(&now);
	std::stringstream ss;
	ss << tm_t->tm_year + 1900 << tm_t->tm_mon + 1 << tm_t->tm_mday
		<< tm_t->tm_hour << tm_t->tm_min;

	return ss.str();
}

void InitDefaultImageOutPath(FString CameraName)
{
	char bmpPath[MAX_PATH];
	char my_documents[MAX_PATH];
	HRESULT result = SHGetFolderPathA(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, my_documents);

	if (result != S_OK)
		GLog->Log(ELogVerbosity::Error, FString::Printf(TEXT("CreateFloder Error: %d"), result));
	else
		GLog->Log(ELogVerbosity::Error, FString::Printf(TEXT("CreateFloder Path: %s"), my_documents));

	char RecordPath[MAX_PATH];
	sprintf(RecordPath, "%s\\CameraRecord", my_documents);

	if (0 != access(RecordPath, 0))
	{
		// if this folder not exist, create a new one.
		mkdir(RecordPath);   // 返回 0 表示创建成功，-1 表示失败
	}

	string TimeStr = GetTime();
	string CameraName_Str = TCHAR_TO_UTF8(*CameraName);

	sprintf(bmpPath, "%s\\%s_%s", RecordPath, CameraName_Str.c_str(), TimeStr.c_str());

	if (0 != access(bmpPath, 0))
	{
		// if this folder not exist, create a new one.
		mkdir(bmpPath);   // 返回 0 表示创建成功，-1 表示失败
	}

	string TempStr = my_documents;
	std::replace(TempStr.begin(), TempStr.end(), '\\', '/');

	char bmpSavePath[260];
	sprintf(bmpSavePath, "%s/CameraRecord/%s_%s", TempStr.c_str(), CameraName_Str.c_str(), TimeStr.c_str());

	DefaultImageOutPath = UTF8_TO_TCHAR(bmpSavePath);
}


typedef TFunction<void(void*&)> FWorkFunction;

class FImageWirteThread : FRunnable
{
public:
	FImageWirteThread()
	{
		bShouldRunWorkThread = false;
		RunnableThreadID = INDEX_NONE;
		RunnableThread = nullptr;
	}
	~FImageWirteThread()
	{
		ReleaseThread();
	}

	void Start(FWorkFunction InWorkFunction, void* InUSer_Ptr)
	{
		if (!RunnableThread)
		{
			User_Ptr = InUSer_Ptr;
			WorkFunction = InWorkFunction;

			bShouldRunWorkThread = true;
			RunnableThread = FRunnableThread::Create(this, TEXT("ImageWirteThread"), 0, TPri_Normal);
			RunnableThreadID = RunnableThread->GetThreadID();
		}
	}

	virtual void Stop()
	{
		ReleaseThread();
	}

	void MyStop()
	{
		bShouldRunWorkThread = false;
	}

	void Runable()
	{
		bShouldRunWorkThread = true;
	}

protected:
	void ReleaseThread()
	{
		bShouldRunWorkThread = false;
		if (RunnableThread)
		{
			//delete RunnableThread;
			RunnableThread = nullptr;
			bShouldRunWorkThread = false;
			RunnableThreadID = INDEX_NONE;
		}
	}

	virtual uint32 Run()override
	{
		while (bShouldRunWorkThread)
		{
			WorkFunction(User_Ptr);
		}
		return 0;
	}

	virtual void Exit()override
	{
	}
public:
	bool bShouldRunWorkThread;
private:
	FRunnableThread* RunnableThread;
	uint32 RunnableThreadID;
	FWorkFunction WorkFunction;
	void* User_Ptr;
};

// Sets default values
AMyVirtualCameraActor::AMyVirtualCameraActor()
{
	ConstructorHelpers::FObjectFinder<UMaterialInstance> TempPostBaseMaterial(TEXT("MaterialInstanceConstant'/MaxQ/DMap/DMapMaterial_Inst.DMapMaterial_Inst'"));
	PostBaseMaterial = TempPostBaseMaterial.Object;
}

// Called when the game starts or when spawned
void AMyVirtualCameraActor::BeginPlay()
{
	Super::BeginPlay();

	{
		// 创建动态材质实例
		DynamicMaterialInstance = UMaterialInstanceDynamic::Create(PostBaseMaterial, this);

		FWeightedBlendables& WeightedBlendables = GetCaptureComponent2D()->PostProcessSettings.WeightedBlendables;
		FWeightedBlendable NewWeightedBlendable(1, DynamicMaterialInstance);
		WeightedBlendables.Array.Add(NewWeightedBlendable);
	}

	UpdateCameraConfig();

	TObjectPtr<UTextureRenderTarget2D>& TextureTarget = GetCaptureComponent2D()->TextureTarget;
	if (!TextureTarget)
	{
		RenderTarget2D = NewObject<UTextureRenderTarget2D>(this);

		if (CameraType == ECameraType::Color)
		{
			GetCaptureComponent2D()->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
			RenderTarget2D->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGBA8;

			RenderTarget2D->InitAutoFormat(ImageWidth, ImageHeight);
			RenderTarget2D->UpdateResourceImmediate(true);
			TextureTarget = RenderTarget2D;
		}
		else if(CameraType == ECameraType::PointCloud)
		{
			GetCaptureComponent2D()->CaptureSource = ESceneCaptureSource::SCS_SceneDepth;
			//RenderTarget2D->RenderTargetFormat = ETextureRenderTargetFormat::PF_FloatRGBA;

			RenderTarget2D->InitCustomFormat(ImageWidth, ImageHeight, PF_FloatRGBA, false);
			TextureTarget = RenderTarget2D;

			/*GetCaptureComponent2D()->CaptureSource = ESceneCaptureSource::SCS_SceneDepth;
			RenderTarget2D->RenderTargetFormat = ETextureRenderTargetFormat::PF_FloatRGBA;

			RenderTarget2D->InitAutoFormat(ImageWidth, ImageHeight);
			RenderTarget2D->UpdateResourceImmediate(true);
			TextureTarget = RenderTarget2D;*/
		}

		

		GLog->Log(ELogVerbosity::Error, FString::Printf(TEXT("TextureTarget is nullptr!")));
	}

	if (GetCaptureComponent2D())
	{
		BlendCamera_Extension = ISensorCustomRenderExtension::CreateBlendCameraExtension();
		GetCaptureComponent2D()->SceneViewExtensions.Emplace(BlendCamera_Extension);
		BlendCamera_Extension->SetCameraSourceMode(EBlendCamera_SourceMode::ANGLE_DEPTH_Intensity);
	}

	WriteImageIndex = 0;
}

void AMyVirtualCameraActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

bool AMyVirtualCameraActor::ConvertDepthsToPointCloud(const TArray<float> Depths, TArray<FVector>& OutPoints)
{
	if(!RenderTarget2D)
		return false;

	FTransform BaseTransform = GetActorTransform();
	float FOVAngle = GetCaptureComponent2D()->FOVAngle;

	int32 Width = RenderTarget2D->SizeX;
	int32 Height = RenderTarget2D->SizeY;

	const FVector BaseLocation = BaseTransform.GetLocation();
	const FQuat BaseRotation = BaseTransform.GetRotation();
	const FVector ForwardDirection = BaseRotation.GetForwardVector().GetSafeNormal();
	const FVector RightDirection = BaseRotation.GetRightVector().GetSafeNormal();
	const FVector UpDirection = BaseRotation.GetUpVector().GetSafeNormal();
	OutPoints.AddZeroed(Width * Height);

	int32 CurrentIndex = 0;
	float AngleToRadian = PI / 180.f;

	float VerticalAngle = FOVAngle * ((float)Height / (float)Width);
	float HalfFOVAngle = FOVAngle * 0.5f;

	float HalfVerticalAngle = FMath::RadiansToDegrees(2 * FMath::Atan(FMath::Tan(FMath::DegreesToRadians(FOVAngle / 2.0f)) * (float)Height / (float)Width)) * 0.5f;

	float LastSizeIndexX = Width - 1;
	float LastSizeIndexY = Height - 1;

	float HorizonRange = FMath::Tan(HalfFOVAngle * AngleToRadian) * 2.f;
	float VerticalRange = FMath::Tan(HalfVerticalAngle * AngleToRadian) * 2.f;
	FVector HorizonDelta = HorizonRange * RightDirection / LastSizeIndexX;
	FVector VerticalDelta = VerticalRange * UpDirection / LastSizeIndexY;

	float SizeRangeX = (LastSizeIndexX) * 0.5f;
	float SizeRangeY = (LastSizeIndexY) * 0.5f;

	static FVector InvalidLocation(BIG_NUMBER);
	for (float CurrentY = SizeRangeY; CurrentY >= -SizeRangeY; CurrentY -= 1.f)
	{
		FVector BaseXDirection = ForwardDirection + (CurrentY * VerticalDelta);
		for (float CurrentX = -SizeRangeX; CurrentX <= SizeRangeX; CurrentX += 1.f)
		{
			if (Depths[CurrentIndex] < 1E+10)
			{
				FVector CurrentDirection = (BaseXDirection + (CurrentX * HorizonDelta)).GetSafeNormal();
				float DepthValue = Depths[CurrentIndex] / FVector::DotProduct(CurrentDirection, ForwardDirection);
				OutPoints[CurrentIndex] = BaseLocation + (CurrentDirection * DepthValue);
			}
			else
			{
				OutPoints[CurrentIndex] = InvalidLocation;
				return false;
			}
			++CurrentIndex;
		}
	}
	return true;
}

void AMyVirtualCameraActor::UpdateCameraConfig()
{
	/*float CW = PixelSize / 1000.f * ImageWidth;

	GetCaptureComponent2D()->FOVAngle = FMath::RadiansToDegrees(2.f * FMath::Atan(CW / (2.f * FocalLength)));

	GetCaptureComponent2D()->CaptureScene();

	float AA = float(bLensDistortion);
	DynamicMaterialInstance->SetScalarParameterValue(FName(TEXT("EnablePP")), AA);*/
}

#if WITH_EDITOR
void AMyVirtualCameraActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	UpdateCameraConfig();
	UpdateLensDistortion();
}
#endif

void AMyVirtualCameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyVirtualCameraActor::CaptureOneFrame()
{
	if(!bCaptureImage)
		return;

	if(!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*ImageOutPath) || ImageOutPath.IsEmpty())
	{
		InitDefaultImageOutPath(CameraName);
		ImageOutPath = DefaultImageOutPath;
		GLog->Log(ELogVerbosity::Error, FString::Printf(TEXT("no set image path use default: %s"), *ImageOutPath));
	}

	if(RenderTarget2D && GetCaptureComponent2D()->CaptureSource == ESceneCaptureSource::SCS_FinalColorLDR)
	{
		FString ImageFullName = FString::Printf(TEXT("%s/%d.png"), *ImageOutPath, WriteImageIndex++);

		FTextureRenderTargetResource* RenderTarget = RenderTarget2D->GameThread_GetRenderTargetResource();

		FReadSurfaceDataFlags readPixelFlags(RCM_UNorm);
		TArray<FColor> outBMP;
		outBMP.AddUninitialized(ImageWidth * ImageHeight);
		RenderTarget->ReadPixels(outBMP, readPixelFlags);

		AsyncTask(ENamedThreads::AnyThread, [this, ImageFullName, outBMP]()
		{
			//outBMPs.Enqueue(outBMP);
			TArray<uint8> ImageData;
			FImageUtils::CompressImageArray(RenderTarget2D->GetSurfaceWidth(), RenderTarget2D->GetSurfaceHeight(), outBMP, ImageData);
			FFileHelper::SaveArrayToFile(ImageData, *ImageFullName);
		});
	}
	else if(RenderTarget2D && GetCaptureComponent2D()->CaptureSource == ESceneCaptureSource::SCS_SceneDepth)
	{
		TArray<FFloat16Color> PixelData;
		RenderTarget2D->GameThread_GetRenderTargetResource()->ReadFloat16Pixels(PixelData);
		int32 Width = RenderTarget2D->SizeX;
		int32 Height = RenderTarget2D->SizeY;
		TArray<float> SceneDepths;
		SceneDepths.Reserve(Width * Height);
		for (int32 i = 0; i < PixelData.Num(); ++i)
		{
			/*auto Pixel = PixelData[i];
			float R = Pixel.R.GetFloat();
			float G = Pixel.G.GetFloat();
			float B = Pixel.B.GetFloat();
			float Depth = R / 255.0f + G / 65025.0f + B / 16581375.0f;*/
			SceneDepths.Add(PixelData[i].R);
		}

		TArray<FVector> PointCloudData;
		ConvertDepthsToPointCloud(SceneDepths, PointCloudData);
		FString OutFullName = FString::Printf(TEXT("%s/%d.xyz"), *ImageOutPath, WriteImageIndex++);

		AsyncTask(ENamedThreads::AnyThread, [this, OutFullName, Width, Height, PointCloudData]()
		{
			FString LineStr;
			for (int i = 0; i < PointCloudData.Num(); ++i)
			{
				LineStr = LineStr + FString::Printf(TEXT("%f %f %f \n"), PointCloudData[i].X, PointCloudData[i].Y, PointCloudData[i].Z);
			}

			FFileHelper::SaveStringToFile(LineStr, *OutFullName);

		});
	}
}

void AMyVirtualCameraActor::WriteImage(void* User_Ptr)
{
	AMyVirtualCameraActor* pCamera = (AMyVirtualCameraActor*)User_Ptr;

	if (pCamera && !pCamera->outBMPs.IsEmpty())
	{
		FString ImageFullName = FString::Printf(TEXT("%s/%d.png"), *pCamera->ImageOutPath, pCamera->WriteImageIndex);

		TArray<FColor> outBMP;
		pCamera->outBMPs.Dequeue(outBMP);

		TArray<uint8> ImageData;
		FImageUtils::CompressImageArray(pCamera->ImageWidth, pCamera->ImageHeight, outBMP, ImageData);
		FFileHelper::SaveArrayToFile(ImageData, *ImageFullName);

		pCamera->WriteImageIndex++;
	}
}

void AMyVirtualCameraActor::SetFOV(const float InFOV)
{
	GetCaptureComponent2D()->FOVAngle = InFOV;

	GetCaptureComponent2D()->CaptureScene();
}

void AMyVirtualCameraActor::SetImageW(const float InWidth)
{
	ImageWidth = InWidth;

	TObjectPtr<UTextureRenderTarget2D>& TextureTarget = GetCaptureComponent2D()->TextureTarget;
	GetCaptureComponent2D()->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
	RenderTarget2D = NewObject<UTextureRenderTarget2D>(this);
	RenderTarget2D->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGBA8;
	RenderTarget2D->InitAutoFormat(ImageWidth, ImageHeight);
	RenderTarget2D->UpdateResourceImmediate(true);
	TextureTarget = RenderTarget2D;

	GetCaptureComponent2D()->CaptureScene();
}

void AMyVirtualCameraActor::SetImageH(const float InHegiht)
{
	ImageHeight = InHegiht;

	TObjectPtr<UTextureRenderTarget2D>& TextureTarget = GetCaptureComponent2D()->TextureTarget;
	GetCaptureComponent2D()->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
	RenderTarget2D = NewObject<UTextureRenderTarget2D>(this);
	RenderTarget2D->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGBA8;
	RenderTarget2D->InitAutoFormat(ImageWidth, ImageHeight);
	RenderTarget2D->UpdateResourceImmediate(true);
	TextureTarget = RenderTarget2D;

	GetCaptureComponent2D()->CaptureScene();
}

void AMyVirtualCameraActor::SetFocalLength(const float InFocalLength)
{
	FocalLength = InFocalLength;

	float CW = PixelSize / 1000.f * ImageWidth;

	GetCaptureComponent2D()->FOVAngle = FMath::RadiansToDegrees(2.f * FMath::Atan(CW / (2.f * FocalLength)));

	GetCaptureComponent2D()->CaptureScene();
}

#pragma optimize("", on)