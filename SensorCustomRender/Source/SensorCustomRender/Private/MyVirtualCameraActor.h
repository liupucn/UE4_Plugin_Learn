#pragma once
#include "CoreMinimal.h"
#include "Engine/SceneCapture2D.h"
//#include "OpenCVLensDistortionBlueprintLibrary.h"
#include "MyVirtualCameraActor.generated.h"


UENUM(BlueprintType)
enum class ECameraType : uint8
{
	Color UMETA(DisplayName = "可见光"),
	PointCloud UMETA(DisplayName = "点云"),
};

UCLASS()
class AMyVirtualCameraActor : public ASceneCapture2D
{
	GENERATED_BODY()
	
public:	
	AMyVirtualCameraActor();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "MarkerManager")
	void CaptureOneFrame();

	UFUNCTION(BlueprintImplementableEvent, Category = "MarkerManager")
	void UpdateLensDistortion();

	UFUNCTION(BlueprintCallable, Category = "MarkerManager")
	void UpdateCameraConfig();

	static void WriteImage(void* User_Ptr);

	void SetFOV(const float InFOV);

	void SetImageW(const float InWidth);

	void SetImageH(const float InHegiht);

	void SetFocalLength(const float InFocalLength);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason)override;

	bool ConvertDepthsToPointCloud(const TArray<float> Depths, TArray<FVector>& OutPoints);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "CameraConfig")
	bool bCaptureImage = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "CameraConfig")
	ECameraType CameraType = ECameraType::Color;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "CameraConfig")
	FString CameraName = TEXT("VC");

	//图像保持路径
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "CameraConfig")
	FString ImageOutPath;

	// 传感器宽度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "CameraConfig")
	float ImageWidth = 1920;

	// 传感器高度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "CameraConfig")
	float ImageHeight = 1080;

	// 焦距 单位：毫米
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "CameraConfig")
	float FocalLength = 14.49;

	//像元大小 单位：微米
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "CameraConfig")
	float PixelSize = 5.5;

	////畸变参数
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraConfig")
	//FOpenCVLensDistortionParameters LensDistortion;

	//是否畸变
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraConfig")
	bool bLensDistortion = false;

	UPROPERTY()
	UTextureRenderTarget2D* RenderTarget2D;

private:
	//后处理材质
	UPROPERTY(Transient)
	UMaterialInstance* PostBaseMaterial;

	UPROPERTY(Transient)
	UMaterialInstanceDynamic* DynamicMaterialInstance;

	class FImageWirteThread* WriteImageThread;
	TQueue<TArray<FColor>> outBMPs;
	int WriteImageIndex = 0;

	TSharedPtr<class ISensorCustomRenderExtension, ESPMode::ThreadSafe > BlendCamera_Extension;
};
