// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Extender.h"
#include "ExtenderStyle.h"
#include "ExtenderCommands.h"
#include "Misc/MessageDialog.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

#include "LevelEditor.h"

#include "ContentBrowserModule.h"
#include "StaticMeshEditorModule.h"

static const FName ExtenderTabName("Extender");

#define LOCTEXT_NAMESPACE "FExtenderModule"

void FExtenderModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FExtenderStyle::Initialize();
	FExtenderStyle::ReloadTextures();

	FExtenderCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FExtenderCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FExtenderModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FExtenderModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FExtenderModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}
	SetupContentBrowserContextMenuExtender();

	IStaticMeshEditorModule& StaticMeshEditorModule = FModuleManager::Get().LoadModuleChecked<IStaticMeshEditorModule>("StaticMeshEditor");
	TArray<FAssetEditorExtender>& ToolbarExtenders = StaticMeshEditorModule.GetToolBarExtensibilityManager()->GetExtenderDelegates();
	ToolbarExtenders.Add(FAssetEditorExtender::CreateStatic(&FExtenderModule::ExtendStaticMeshEditorToolbar));
}

void FExtenderModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FExtenderStyle::Shutdown();

	FExtenderCommands::Unregister();
}

//��չ��Դ�Ҽ��˵�
void FExtenderModule::SetupContentBrowserContextMenuExtender()
{
	if (!IsRunningCommandlet())
	{
		FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
		TArray< FContentBrowserMenuExtender_SelectedAssets >& CBMenuExtenderDelegates = ContentBrowserModule.GetAllAssetViewContextMenuExtenders();

		CBMenuExtenderDelegates.Add(FContentBrowserMenuExtender_SelectedAssets::CreateStatic(&FExtenderModule::OnExtendContentBrowserAssetSelectionMenu));
	}
}

TSharedRef<FExtender> FExtenderModule::OnExtendContentBrowserAssetSelectionMenu(const TArray<FAssetData>& SelectedAssets)
{
	TSharedRef<FExtender> Extender = MakeShared<FExtender>();
	TArray<UStaticMesh*> StaticMeshes;

	//ɸѡѡ����Դ�����ͣ���ǰɸѡ����UStaticMesh����
	for (const FAssetData& Asset : SelectedAssets)
	{
		if (Asset.AssetClass == UStaticMesh::StaticClass()->GetFName())
		{
			if (UStaticMesh * SelectedStaticMesh = Cast<UStaticMesh>(Asset.GetAsset()))
			{
				StaticMeshes.Add(SelectedStaticMesh);
			}
		}
	}

	//���ѡ����һ�����ϵ�UStaticMesh��Դ����Ϊ������һ���Ҽ��˵�
	if (StaticMeshes.Num() > 0)
	{
		Extender->AddMenuExtension("GetAssetActions", EExtensionHook::First, nullptr, FMenuExtensionDelegate::CreateLambda(
			[StaticMeshes](FMenuBuilder& MenuBuilder)
		{
			MenuBuilder.AddMenuEntry(
				LOCTEXT("Labeltoshow", "Label to show"),//Ҫ��ʾ�Ĳ˵���ǩ��
				LOCTEXT("Tooltip", "Tool tip"),//�����ͣ�ڸò˵���ʱ����ʾ
				FSlateIcon(),//ʹ�õ�ͼ��
				FUIAction(FExecuteAction::CreateStatic(&FExtenderModule::CustomAction, StaticMeshes), FCanExecuteAction()));//�˵�ִ�еĲ���
		}));
	}
	return Extender;
}

//��չUstaticMesh�༭���еĲ˵�
TSharedRef<FExtender> FExtenderModule::ExtendStaticMeshEditorToolbar(const TSharedRef<FUICommandList> CommandList, const TArray<UObject*> Objects)
{
	checkf(Objects.Num() && Objects[0]->IsA<UStaticMesh>(), TEXT("Invalid object for static mesh editor toolbar extender"));

	TSharedRef<FExtender> Extender = MakeShared<FExtender>();
	// Add extension to StaticMesh Editor's toolbar
	Extender->AddMenuExtension(
		"UVActionOptions",
		EExtensionHook::First,
		CommandList,
		FMenuExtensionDelegate::CreateStatic(&FExtenderModule::CreateMenu, CommandList, Cast<UStaticMesh>(Objects[0]))
	);
	return Extender;
}

void FExtenderModule::CreateMenu(FMenuBuilder& ParentMenuBuilder, const TSharedRef<FUICommandList> CommandList, UStaticMesh* InStaticMesh)
{
	FUIAction GenerateUnwrappedUVMenuAction;
	GenerateUnwrappedUVMenuAction.ExecuteAction.BindLambda([InStaticMesh]()
	{
		IAssetEditorInstance* EditorInstance = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->FindEditorForAsset(InStaticMesh, false);
		if (!EditorInstance || !EditorInstance->GetEditorName().ToString().Contains(TEXT("StaticMeshEditor")))
		{
			return;
		}
		FExtenderModule::CustomAction({ InStaticMesh });
	});
	ParentMenuBuilder.AddMenuEntry(
		LOCTEXT("MyMenu", "My Menu"),
		LOCTEXT("MyMenutip", "Trigger Action of this menu"),
		FSlateIcon(),
		GenerateUnwrappedUVMenuAction
	);
}

void FExtenderModule::CustomAction(TArray<UStaticMesh*> StaticMeshes)
{
	//��Ӿ���Ĵ�����
	FMessageDialog::Open(EAppMsgType::Ok, FText::FromString("is Triggering Action of this menu!"));
}

void FExtenderModule::PluginButtonClicked()
{
	// Put your "OnButtonClicked" stuff here
	FText DialogText = FText::Format(
							LOCTEXT("PluginButtonDialogText", "Add code to {0} in {1} to override this button's actions"),
							FText::FromString(TEXT("FExtenderModule::PluginButtonClicked()")),
							FText::FromString(TEXT("Extender.cpp"))
					   );
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);
}

void FExtenderModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FExtenderCommands::Get().PluginAction);
}

void FExtenderModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FExtenderCommands::Get().PluginAction);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FExtenderModule, Extender)