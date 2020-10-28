// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ImportCustomModel.h"
#include "ImportCustomModelStyle.h"
#include "ImportCustomModelCommands.h"
#include "Misc/MessageDialog.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

#include "AssetRegistryModule.h"
#include "RawMesh.h"
#include "Misc/FileHelper.h"
#include "Materials/Material.h"
#include "Materials/MaterialExpressionConstant3Vector.h"

#include "LevelEditor.h"

static const FName ImportCustomModelTabName("ImportCustomModel");

#define LOCTEXT_NAMESPACE "FImportCustomModelModule"

void FImportCustomModelModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FImportCustomModelStyle::Initialize();
	FImportCustomModelStyle::ReloadTextures();

	FImportCustomModelCommands::Register();
	
	//�󶨶���
	PluginCommands = MakeShareable(new FUICommandList);
	PluginCommands->MapAction(
		FImportCustomModelCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FImportCustomModelModule::PluginButtonClicked),
		FCanExecuteAction());
		
	PluginCommands->MapAction(
		FImportCustomModelCommands::Get().ImportAction,
		FExecuteAction::CreateRaw(this, &FImportCustomModelModule::ImportModelDataButtonClicked),
		FCanExecuteAction());

	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FImportCustomModelModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FImportCustomModelModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}
}

void FImportCustomModelModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FImportCustomModelStyle::Shutdown();

	FImportCustomModelCommands::Unregister();
}


void FImportCustomModelModule::PluginButtonClicked()
{
#if 1 //����Mesh
	FString AssetName = FString(TEXT("Mesh1"));
	FString PathName = FString(TEXT("/Game/"));
	FString PackageName = PathName + AssetName;
	UPackage* Package = CreatePackage(NULL,*PackageName);
	check(Package);
	MyMesh = NewObject<UStaticMesh>(Package, FName(*AssetName), RF_Public | RF_Standalone | RF_Transactional);
	MyMesh->AddSourceModel();
	FAssetRegistryModule::AssetCreated(MyMesh);
	MyMesh->MarkPackageDirty();
#elif 0 //��������
	FString AssetName1 = FString(TEXT("Material"));
	FString PathName1 = FString(TEXT("/Game/"));
	FString PackageName1 = PathName1 + AssetName1;
	UPackage* Package1 = CreatePackage(NULL, *PackageName1);
	check(Package1);
	UMaterial* MyMaterial = NewObject<UMaterial>(Package1, FName(*AssetName1), RF_Public | RF_Standalone);

	UMaterialExpressionConstant3Vector* BaseColorFactorNode = NewObject<UMaterialExpressionConstant3Vector>(MyMaterial);
	BaseColorFactorNode->Constant = FColor::Red; //you can specify any color her
	MyMaterial->BaseColor.Connect(0, BaseColorFactorNode);
	FAssetRegistryModule::AssetCreated(MyMaterial);
	MyMaterial->PreEditChange(NULL);
	MyMaterial->PostEditChange();

	UStaticMesh* MeshWithMaterial = LoadObject<UStaticMesh>(NULL, TEXT("/Game/Sphere"));
	MeshWithMaterial->SetMaterial(0, MyMaterial);
#endif
}

#define MESH 1 //���ֵ�����

void FImportCustomModelModule::ImportModelDataButtonClicked()
{
	UE_LOG(LogClass, Log, TEXT("ImportModelDataButtonClicked"));
#if MESH
	if (MyMesh)
	{
		FRawMesh RawMesh;
		MyMesh->GetSourceModel(0).LoadRawMesh(RawMesh);
		ImportModelData(RawMesh);//��������
		MyMesh->GetSourceModel(0).SaveRawMesh(RawMesh);
		MyMesh->Build();
	}
#elif !MESH
	FString Text;
	FString LeftS;
	FString RightS;
	TArray<FString> StrGroupArray;
	FString FilePath = "C:/Users/liupu/Desktop/Modle/CatiaModel.obj";
	if (FFileHelper::LoadFileToString(Text, *FilePath))
	{
		RightS = Text;
		while (RightS.Split("g", &LeftS, &RightS))
		{
			if (!LeftS.IsEmpty())
				StrGroupArray.Add(LeftS);//��ȡ����
		}
		StrGroupArray.Add(RightS);

		for (int i = 0;i < StrGroupArray.Num();i++)
		{
			CreateChirdModel(StrGroupArray[i]);//����StaticMesh
		}
	}
#endif
}

int FImportCustomModelModule::CreateChirdModel(FString Text)
{
	int VertexNum=0;
	FString LeftS;
	FString RightS = Text;

	if(!RightS.IsEmpty())
	{
		TArray<FString> StrGroupArray;
		RightS.ReplaceCharInline('\n', '=');
		RightS.ReplaceCharInline('\r', '=');

		RightS.Split("=", &LeftS, &RightS);
		FString MeshIndex = LeftS; //��ȡ���
		MeshIndex.TrimStartAndEndInline();

		while (RightS.Split("=", &LeftS, &RightS))
			StrGroupArray.Add(LeftS);

		FString MeshAssetName = FString(TEXT("CatiaModel" + MeshIndex));
		FString PathName = FString(TEXT("/Game/Mesh/Model1/"));
		FString PackageName = PathName + MeshAssetName;
		UPackage* MeshPackage = CreatePackage(NULL, *PackageName);
		check(MeshPackage);
		UStaticMesh* Mesh = NewObject<UStaticMesh>(MeshPackage, FName(*MeshAssetName), RF_Public | RF_Standalone);
		Mesh->AddSourceModel();
		FAssetRegistryModule::AssetCreated(Mesh);
		Mesh->MarkPackageDirty();//����StaticMesh

		FString MaterialAssetName = MeshAssetName + "_Mat";
		FString MaterialPackageName = PathName + MeshAssetName;
		UPackage* MaterialPackage = CreatePackage(NULL, *MaterialPackageName);
		check(MaterialPackage);
		UMaterial* Material = NewObject<UMaterial>(MaterialPackage, FName(*MaterialAssetName), RF_Public | RF_Standalone);//����Material
		UMaterialExpressionConstant3Vector* BaseColorFactorNode = NewObject<UMaterialExpressionConstant3Vector>(Material);//����һ����ɫ�ڵ�

		FRawMesh RawMesh;//StaticMesh������Ϣ
		Mesh->GetSourceModel(0).LoadRawMesh(RawMesh);

		FColor Color;//����������ɫ
		TArray<FVector> VertexPosition;//����λ��
		TArray<FVector> VertexNormal;//���㷨��
		TArray<FVector2D> VertexTexCoords;//������������
		int k = 0;

		for (int i = 0;i < StrGroupArray.Num();i++)
		{
			FString TemStr = StrGroupArray[i];
			TemStr = TemStr.Trim();
			if (TemStr.IsEmpty())//����ȥ��
				continue;

			StrGroupArray[i].TrimStartInline();//ȥ����һ���ַ�ǰ�ո�
			if (!StrGroupArray[i].Split(" ", &LeftS, &RightS))
				continue;

			if (LeftS.Equals("m"))
			{

			}
			else if (LeftS.Equals("vc"))//ģ�Ͳ���
			{
				float r, g, b;
				if (RightS.Split(" ", &LeftS, &RightS))
					r = FCString::Atof(*LeftS);
				if (RightS.Split(" ", &LeftS, &RightS))
				{
					g = FCString::Atof(*LeftS);
					b = FCString::Atof(*RightS);
				}
				BaseColorFactorNode->Constant = FColor(r * 255, g * 255, b * 255);
			}
			else if (LeftS.Equals("v"))//����λ��
			{
				float x, y, z;
				if (RightS.Split(" ", &LeftS, &RightS))
					x = FCString::Atof(*LeftS);
				if (RightS.Split(" ", &LeftS, &RightS))
				{
					y = FCString::Atof(*LeftS);
					z = FCString::Atof(*RightS);
				}
				RawMesh.VertexPositions.Add(FVector(x, -y, z));
			}
			else if (LeftS.Equals("vn"))//����
			{
				float x, y, z;
				if (RightS.Split(" ", &LeftS, &RightS))
					x = FCString::Atof(*LeftS);
				if (RightS.Split(" ", &LeftS, &RightS))
				{
					y = FCString::Atof(*LeftS);
					z = FCString::Atof(*RightS);
				}
				VertexNormal.Add(FVector(x, y, z));
			}
			else if (LeftS.Equals("vt"))//����
			{
				float u, v;
				if (LeftS.Split(" ", &LeftS, &RightS))
				{
					u = FCString::Atof(*LeftS);
					v = FCString::Atof(*RightS);
				}
				VertexTexCoords.Add(FVector2D(u, v));
			}
			else if (LeftS.Equals("f"))//��
			{
				int p0, p1, p2;
				if (RightS.Split(" ", &LeftS, &RightS))
					p0 = FCString::Atof(*LeftS);
				if (RightS.Split(" ", &LeftS, &RightS))
				{
					p1 = FCString::Atof(*LeftS);
					p2 = FCString::Atof(*RightS);
				}
				RawMesh.WedgeIndices.Add(p0);
				RawMesh.WedgeIndices.Add(p1);
				RawMesh.WedgeIndices.Add(p2);

				RawMesh.FaceMaterialIndices.Add(0);
				RawMesh.FaceSmoothingMasks.Add(0);
			}
		}

		int Log = RawMesh.WedgeIndices[0];
		for (int32 j = 0;j < RawMesh.WedgeIndices.Num();j++)
		{
			RawMesh.WedgeIndices[j] = RawMesh.WedgeIndices[j] - Log;
			RawMesh.WedgeColors.Add(FColor::Red);//������ɫ

			RawMesh.WedgeTangentX.Add(FVector(0, 0, 0));
			RawMesh.WedgeTangentY.Add(FVector(0, 0, 0));

			if ((int32)RawMesh.WedgeIndices[j] >= VertexNormal.Num())
				RawMesh.WedgeTangentZ.Add(FVector(0, 0, 0));//���㷨��
			else
				RawMesh.WedgeTangentZ.Add(VertexNormal[RawMesh.WedgeIndices[j]]);//���㷨��

			//RawMesh.WedgeTexCoords[0].Add(VertexTexCoords[RawMesh.WedgeIndices[i]]);//��������
			RawMesh.WedgeTexCoords[0].Add(FVector2D(0, 0));//��������
		}

		Material->BaseColor.Connect(0, BaseColorFactorNode);//�����ɫ�ڵ㵽����
		FAssetRegistryModule::AssetCreated(Material);
		Material->PreEditChange(NULL);
		Material->PostEditChange();
		
		VertexNum = RawMesh.WedgeIndices[0];//���ض������
		Mesh->GetSourceModel(0).SaveRawMesh(RawMesh);
		Mesh->StaticMaterials.Add(Material); //ΪMesh���һ������
		//Mesh->SetMaterial(0, Material); //�����ʸ���Mesh
		Mesh->Build();
	}
	return VertexNum;
}

void FImportCustomModelModule::ImportModelData(FRawMesh&RawMesh)
{
	FString FilePath = "C:/Users/liupu/Desktop/Modle/CatiaModel.obj";
	FString Text;
	TArray<FString> AxisInfoArray;
	AxisInfoArray.Empty();
	if (FFileHelper::LoadFileToString(Text, *FilePath))
	{
		FString LeftS;
		FString RightS;
		TArray<FString> StrGroupArray;
		TArray<int> Group;
		TArray<FColor> VertexColor;
		TArray<FVector> VertexPosition;
		TArray<FVector> VertexNormal;
		TArray<FVector2D> VertexTexCoords;

		//�ѻ��лس��� �滻��==��ʹ�����==�и�
		Text.ReplaceCharInline('\n', '=');
		Text.ReplaceCharInline('\r', '=');

		RightS = Text;
		while (RightS.Split("=", &LeftS, &RightS))
			StrGroupArray.Add(LeftS);

		for (int i = 0;i < StrGroupArray.Num();i++)
		{
			FString TemStr = StrGroupArray[i];
			TemStr = TemStr.Trim();
			if (TemStr.IsEmpty())//����ȥ��
				continue;

			StrGroupArray[i].TrimStartInline();//ȥ����һ���ַ�ǰ�ո�
			if (!StrGroupArray[i].Split(" ", &LeftS, &RightS))
				continue;

			if (LeftS.Equals("g"))
			{

			}
			else if (LeftS.Equals("m"))
			{

			}
			else if (LeftS.Equals("vc"))
			{
				int r, g, b;
				if (RightS.Split(" ", &LeftS, &RightS))
					r = FCString::Atof(*LeftS);
				if (RightS.Split(" ", &LeftS, &RightS))
				{
					g = FCString::Atof(*LeftS);
					b = FCString::Atof(*RightS);
				}
				VertexColor.Add(FColor(r, g, b));
				VertexColor.Add(FColor(r, g, b));
				VertexColor.Add(FColor(r, g, b));
			}
			else if (LeftS.Equals("v"))
			{
				float x, y, z; 
				if (RightS.Split(" ", &LeftS, &RightS))
					x = FCString::Atof(*LeftS);
				if (RightS.Split(" ", &LeftS, &RightS))
				{
					y = FCString::Atof(*LeftS);
					z = FCString::Atof(*RightS);
				}
				RawMesh.VertexPositions.Add(FVector(x, y, z));
			}
			else if (LeftS.Equals("vn"))
			{
				float x, y, z;
				if (RightS.Split(" ", &LeftS, &RightS))
					x = FCString::Atof(*LeftS);
				if (RightS.Split(" ", &LeftS, &RightS))
				{
					y = FCString::Atof(*LeftS);
					z = FCString::Atof(*RightS);
				}
				VertexNormal.Add(FVector(x, y, z));
			}
			else if (LeftS.Equals("vt"))
			{
				float u, v;
				if (RightS.Split(" ", &LeftS, &RightS))
				{
					u = FCString::Atof(*LeftS);
					v = FCString::Atof(*RightS);
				}
				VertexTexCoords.Add(FVector2D(u, v));
			}
			else if (LeftS.Equals("f"))
			{
				int p0, p1, p2; 
				if (RightS.Split(" ", &LeftS, &RightS))
					p0 = FCString::Atof(*LeftS) - 1;
				if (RightS.Split(" ", &LeftS, &RightS))
				{
					p1 = FCString::Atof(*LeftS) - 1;
					p2 = FCString::Atof(*RightS) - 1;
				}
				RawMesh.WedgeIndices.Add(p0);
				RawMesh.WedgeIndices.Add(p1);
				RawMesh.WedgeIndices.Add(p2);

				RawMesh.FaceMaterialIndices.Add(1);
				RawMesh.FaceSmoothingMasks.Add(1);
			}
		}

		for (int i = 0;i < RawMesh.WedgeIndices.Num();i++)
		{
			RawMesh.WedgeColors.Add(FColor::Red);//������ɫ

			RawMesh.WedgeTangentX.Add(FVector(0, 0, 0));
			RawMesh.WedgeTangentY.Add(FVector(0, 0, 0));

			if((int32)RawMesh.WedgeIndices[i]>= VertexNormal.Num())
				RawMesh.WedgeTangentZ.Add(FVector(0, 0, 1));//���㷨��
			else
				RawMesh.WedgeTangentZ.Add(VertexNormal[RawMesh.WedgeIndices[i]]);//���㷨��

			//RawMesh.WedgeTexCoords[0].Add(VertexTexCoords[RawMesh.WedgeIndices[i]]);//��������
			RawMesh.WedgeTexCoords[0].Add(FVector2D(0, 1));//��������
		}
	}
}

void FImportCustomModelModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FImportCustomModelCommands::Get().PluginAction);
	Builder.AddMenuEntry(FImportCustomModelCommands::Get().ImportAction);
}

void FImportCustomModelModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FImportCustomModelCommands::Get().PluginAction);
	Builder.AddToolBarButton(FImportCustomModelCommands::Get().ImportAction);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FImportCustomModelModule, ImportCustomModel)