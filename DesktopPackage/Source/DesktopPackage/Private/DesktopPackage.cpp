// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "DesktopPackage.h"
#include "DesktopPackageStyle.h"
#include "DesktopPackageCommands.h"
#include "Misc/MessageDialog.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

#include "LevelEditor.h"

#include "IUATHelperModule.h"
#include "Interfaces/IPluginManager.h"

#include "AssetThumbnail.h"
#include "AssetRegistryModule.h"
#include "IVREditorModule.h"

#include "WindowsTargetSettings.h"

#include "Settings/ProjectPackagingSettings.h"

static const FName DesktopPackageTabName("DesktopPackage");

#define LOCTEXT_NAMESPACE "FDesktopPackageModule"

void FDesktopPackageModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FDesktopPackageStyle::Initialize();
	FDesktopPackageStyle::ReloadTextures();

	FDesktopPackageCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FDesktopPackageCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FDesktopPackageModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FDesktopPackageModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FDesktopPackageModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}
}

void FDesktopPackageModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FDesktopPackageStyle::Shutdown();

	FDesktopPackageCommands::Unregister();
}

void FDesktopPackageModule::PluginButtonClicked()
{
	{

		static FName NAME_GLSL_430(TEXT("GLSL_430"));
		static FName NAME_PCD3D_SM5(TEXT("PCD3D_SM5"));

		UWindowsTargetSettings * WTS = GetMutableDefault<UWindowsTargetSettings>();
		WTS->TargetedRHIs.Reset();
		WTS->TargetedRHIs.Add(NAME_PCD3D_SM5.ToString());

		WTS->SaveConfig();

		FString GameContent = FPaths::ProjectConfigDir();

		const FString DefaultGameIniFilename = GameContent / TEXT("DefaultGame.ini");
		GConfig->UnloadFile(DefaultGameIniFilename);
		GConfig->LoadFile(DefaultGameIniFilename);
		TArray<FString> Cultures;
		Cultures.Add(TEXT("en"));
		Cultures.Add(TEXT("zh_CN"));
		GConfig->SetArray(
			TEXT("/Script/UnrealEd.ProjectPackagingSettings"),
			TEXT("+CulturesToStage"),
			Cultures,
			DefaultGameIniFilename
		);

		GConfig->Flush(false, DefaultGameIniFilename);

		const FString DefaultEngineIniFilename = GameContent / TEXT("DefaultEngine.ini");
		GConfig->UnloadFile(DefaultEngineIniFilename);
		GConfig->LoadFile(DefaultEngineIniFilename);
		////String
		//GConfig->SetString(
		//	TEXT("/Script/Engine.Engine"),
		//	TEXT("GameEngine"),
		//	DefaultEngineIniFilename
		//);
		GConfig->Flush(false, DefaultEngineIniFilename);
	}




	FString projectPath = FPaths::ConvertRelativePathToFull(FPaths::GetProjectFilePath());

	const FString ProjectName = FPaths::GetBaseFilename(projectPath);

	FString WorkerName = FPlatformProcess::ExecutableName(true);
	WorkerName.Append(TEXT("-Cmd.exe "));


	FString CommandLine = "";
	CommandLine.Append(FString::Printf(TEXT(" BuildCookRun  -project=\"%s\""), *projectPath));
	CommandLine.Append(TEXT(" -noP4 -clientconfig=Development -serverconfig=Development -nocompileeditor "));
	CommandLine.Append(FString::Printf(TEXT("-ue4exe=\"%s\""), *WorkerName));
	CommandLine.Append(TEXT(" -utf8output -platform=Win64 -targetplatform=Win64 -cook -COOKALL -unversionedcookedcontent -deploy -nocompileeditor -build  -compressed -stage "));


	CommandLine.Append(TEXT(" -COOKDIR=\""));

	//----------------------------------Start------------------------------------------
	//< 有新的Moule，只需在此下面添加即可，其他地方不要动！
	TArray<FString>Modules;
	Modules.Add(TEXT("AirSim"));
	Modules.Add(TEXT("PathFollow"));

	//-------------------------------------End-----------------------------------------

	for (FString & Module : Modules)
	{
		TSharedPtr<IPlugin> vrfunctionContentplugin = IPluginManager::Get().FindPlugin(Module);
		if (vrfunctionContentplugin.IsValid())
		{
			FString vrfunctionContentpluginDir = vrfunctionContentplugin->GetBaseDir() / TEXT("Content");
			CommandLine.Append(FString::Printf(TEXT("%s+"), *vrfunctionContentpluginDir));
		}
		else
		{
			GLog->Logf(ELogVerbosity::Warning, TEXT("Plugin[ %s ] not valid"), *Module);
		}
	}

	CommandLine.RemoveFromEnd(TEXT("+"));
	CommandLine.Append("\"");


	const FSlateBrush* Brush = FDesktopPackageStyle::Get().GetBrush(TEXT("Cook.OpenPluginWindow"));
	IUATHelperModule::UatTaskResultCallack PackageFinishedFunction = [this](const FString& ResultMessage, double InParam) {
		OnPackageFinished(ResultMessage, InParam); };
	IUATHelperModule::Get().CreateUatTask(CommandLine, FText::FromString(ProjectName),
		LOCTEXT("CookPackaging", "Packaging"), LOCTEXT("CookReleaseProject", "ReleaseProject"), Brush, PackageFinishedFunction);
}

void FDesktopPackageModule::OnPackageFinished(const FString& ResultMessage, double InParam)
{
	//if (ResultMessage == TEXT("Completed"))
	//{
	//	FString DesRootFolder = FPaths::ProjectSavedDir() + TEXT("/StagedBuilds/WindowsNoEditor/");
	//	FString VSPPhysicsFolder = FPaths::ConvertRelativePathToFull(FPaths::EnginePluginsDir()) + TEXT("/VSP/AgxPhysics/");

	//	FString SourceFolder = VSPPhysicsFolder + TEXT("Source/ThirdParty/AgxAPI/dll/Win64/");
	//	TArray<FString> CopyFileArray;
	//	IFileManager::Get().FindFiles(CopyFileArray, *SourceFolder);

	//	/*拷贝dll动态库*/
	//	FString DesPathFolder = DesRootFolder + TEXT("Engine/Binaries/Win64/");
	//	if (IFileManager::Get().DirectoryExists(*DesPathFolder))
	//	{
	//		for (const FString& CurrentFile : CopyFileArray)
	//		{
	//			IFileManager::Get().Copy(*(DesPathFolder + CurrentFile), *(SourceFolder + CurrentFile), false);
	//		}
	//	}
	//	DesPathFolder = DesRootFolder + FApp::GetProjectName() + TEXT("/Binaries/Win64/");
	//	if (IFileManager::Get().DirectoryExists(*DesPathFolder))
	//	{
	//		for (const FString& CurrentFile : CopyFileArray)
	//		{
	//			IFileManager::Get().Copy(*(DesPathFolder + CurrentFile), *(SourceFolder + CurrentFile), false);
	//		}
	//	}

	//	{
	//		FString PhysicsIconFolder = VSPPhysicsFolder + TEXT("Resources/");
	//		FString PhysicsIconDesFolder = DesRootFolder + TEXT("Engine/Plugins/VSP/AgxPhysics/Resources/");
	//		IFileManager::Get().Copy(*(PhysicsIconDesFolder + TEXT("VSPPhysics.png")), *(PhysicsIconFolder + TEXT("VSPPhysics.png")));
	//	}

	//	/*拷贝环境文件*/
	//	DesPathFolder = DesRootFolder + TEXT("Engine/Plugins/VSP/AgxPhysics/Source/ThirdParty/AgxAPI/init/");
	//	SourceFolder = VSPPhysicsFolder + TEXT("Source/ThirdParty/AgxAPI/init/");
	//	TArray<FString> FindFilePathArray;
	//	IFileManager::Get().FindFilesRecursive(FindFilePathArray, *SourceFolder, TEXT("*.*"), true, false);
	//	int32 SourceFolderLen = SourceFolder.Len();
	//	for (FString& CurrentFile : FindFilePathArray)
	//	{
	//		FString CurrentFileName = CurrentFile.Right(CurrentFile.Len() - SourceFolderLen);
	//		IFileManager::Get().Copy(*(DesPathFolder + CurrentFileName), *(CurrentFile), false);
	//	}
	//	DesPathFolder = FPaths::ProjectDir() + TEXT("/Saved/StagedBuilds/WindowsNoEditor/License/");
	//	SourceFolder = FPaths::ConvertRelativePathToFull(FPaths::RootDir()) + TEXT("/License/");
	//	/*拷贝Agx的License*/
	//	IFileManager::Get().Copy(*(DesPathFolder + TEXT("agx.lic")), *(SourceFolder + TEXT("agx.lic")));
	//	/*拷贝MakeReal3D的 License*/
	//	IFileManager::Get().Copy(*(DesPathFolder + TEXT("license.lic")), *(SourceFolder + TEXT("license.lic")));
	//}
}

void FDesktopPackageModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FDesktopPackageCommands::Get().PluginAction);
}

void FDesktopPackageModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FDesktopPackageCommands::Get().PluginAction);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDesktopPackageModule, DesktopPackage)