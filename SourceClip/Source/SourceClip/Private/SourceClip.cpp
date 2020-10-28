// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SourceClip.h"

#include "NameTypes.h"
#include "CoreDelegates.h"

#include "ToolMenus.h"
#include "LevelEditor.h"
#include "TabManager.h"
#include "ModuleManager.h"


FSourceClip::FSourceClip(void)
{
}

FSourceClip::~FSourceClip(void)
{
}

void FSourceClip::AddDelegate()
{
	FCoreDelegates::OnBeginFrame.AddRaw(this, &FSourceClip::Clipping);

	UE_LOG(LogTemp, Warning, TEXT("Add Clipping Delegate Successful."));

}

void FSourceClip::Clipping(void)
{
	if (GEngine)
	{
		ToolMenus = UToolMenus::Get();
		ClipMainMenu();
		ClipToolBar();

		const FName LevelEditorModuleName("LevelEditor");
		FLevelEditorModule& LevelEditorModule = FModuleManager::GetModuleChecked<FLevelEditorModule>(LevelEditorModuleName);
		LevelEditorTabManager = LevelEditorModule.GetLevelEditorTabManager();
		ClipWindowMenu();

		ClipFileMenu();

		ClipDeveloperTools();
		
		ClipEditorMode();

		ClipContextMenu();
	}

	/*->Rebuilds all widgets generated from a specific menu.*/
	ToolMenus->RefreshAllWidgets();

	FCoreDelegates::OnBeginFrame.RemoveAll(this);
}

void FSourceClip::ClipMainMenu(void)
{
	const FName MenuName = "MainFrame.MainMenu";
	FName SectionName = "Help";

	/*-> +Help Menu*/
	/*-> +Para*/
	ToolMenus->RemoveSection(MenuName, SectionName);
}

void FSourceClip::ClipFileMenu(void)
{
	const FName MenuName = "MainFrame.MainTabMenu.File";
	FName SectionName = "FileProject";

	FName EntryName;
	/*-> Adds C++ code to the project*/
	/*-> +Para*/
	{
		EntryName = "Add C++ Code";
		ToolMenus->RemoveEntry(MenuName, SectionName, EntryName);
	}

	/*-> Refresh Visual Studio Project*/
	{
		EntryName = "CodeProject";
		ToolMenus->RemoveEntry(MenuName, SectionName, EntryName);
	}

	/*-> Open Visual Studio*/
	/*-> +Para*/
	{
		EntryName = "Open Visual Studio";
		ToolMenus->RemoveEntry(MenuName, SectionName, EntryName);
	}
}

void FSourceClip::ClipWindowMenu(void)
{
	FName TabId;

	/*-> Layers Window SubMenu*/
	{
		TabId = "LevelEditorLayerBrowser";
		LevelEditorTabManager->UnregisterTabSpawner(TabId);
	}
	
	/*-> Hierarchical LOD Outliner Window SubMenu*/
	{
		TabId = "LevelEditorHierarchicalLODOutliner";
		LevelEditorTabManager->UnregisterTabSpawner(TabId);
	}

	/*-> Statistics Window SubMenu*/
	{
		TabId = "LevelEditorStatsViewer";
		LevelEditorTabManager->UnregisterTabSpawner(TabId);
	}

	/*-> Sequencer Window SubMenu*/
	{
		TabId = "Sequencer";
		LevelEditorTabManager->UnregisterTabSpawner(TabId);
	}
}

void FSourceClip::ClipDeveloperTools(void)
{
	const TSharedRef<FGlobalTabmanager>& GlobalTabmanager = FGlobalTabmanager::Get();
	FName TabId;

	/*-> Blueprint Debugger*/
	{
		TabId = TEXT("DebuggerApp");
		GlobalTabmanager->UnregisterNomadTabSpawner(TabId);
	}

	/*-> Sequence Recorder*/
	{
		TabId = TEXT("SequenceRecorder");
		GlobalTabmanager->UnregisterNomadTabSpawner(TabId);
	}

	/*-> Collision Analyzer*/
	{
		TabId = TEXT("CollisionAnalyzerApp");
		GlobalTabmanager->UnregisterNomadTabSpawner(TabId);
	}

	/*-> Debug Tools*/
	{
		TabId = "DebugTools";
		GlobalTabmanager->UnregisterNomadTabSpawner(TabId);
	}

	/*-> Tools Modules*/
	{
		TabId = "ModulesTab";
		GlobalTabmanager->UnregisterNomadTabSpawner(TabId);
	}

	/*-> Device Manager*/
	{
		TabId = "DeviceManager";
		GlobalTabmanager->UnregisterNomadTabSpawner(TabId);
	}

	/*-> Device Profiles*/
	{
		TabId = "DeviceProfileEditor";
		GlobalTabmanager->UnregisterNomadTabSpawner(TabId);
	}

	/*-> Merge Actors*/
	{
		TabId = FName("MergeActors");
		GlobalTabmanager->UnregisterNomadTabSpawner(TabId);
	}

	/*-> Pixel Inspector*/
	{
		TabId = "LevelEditorPixelInspector";
		GlobalTabmanager->UnregisterNomadTabSpawner(TabId);
	}

	/*-> Class Viewer*/
	{
		TabId = FName("ClassViewerApp");
		GlobalTabmanager->UnregisterNomadTabSpawner(TabId);
	}

	/*-> Visual Logger*/
	{
		TabId = "VisualLogger";
		GlobalTabmanager->UnregisterNomadTabSpawner(TabId);
	}

	/*-> Profile Data Visualizer*/
	{
		TabId = "VisualizerSpawnPoint";
		GlobalTabmanager->UnregisterNomadTabSpawner(TabId);
	}

	/*-> Timecode Provider*/
	{
		TabId = FName("TimecodeProviderTab");
		LevelEditorTabManager->UnregisterTabSpawner(TabId);
	}

	/*-> Struct Viewer*/
	{
		TabId = FName("StructViewerApp");
		GlobalTabmanager->UnregisterNomadTabSpawner(TabId);
	}

	/*-> Session Frontend*/
	{
		TabId = "SessionFrontend";
		GlobalTabmanager->UnregisterNomadTabSpawner(TabId);
	}

	/*-> Widget Reflector*/
	{
		TabId = "WidgetReflector";
		GlobalTabmanager->UnregisterNomadTabSpawner(TabId);
	}

	/*-> Asset Audio*/
	{
		TabId = TEXT("AssetAudit");
		GlobalTabmanager->UnregisterNomadTabSpawner(TabId);
	}

	/*-> "Material Analyzer"*/
	{
		TabId = "MaterialAnalyzer";
		GlobalTabmanager->UnregisterNomadTabSpawner(TabId);
	}	 
}

void FSourceClip::ClipToolBar(void)
{
	const FName MenuName = "LevelEditor.LevelEditorToolBar";

	FName SectionName, EntryName;

	/*->Source Control ToolBar*/
	{
		SectionName = "File";
		EntryName = "SourceControl";
		ToolMenus->RemoveEntry(MenuName, SectionName, EntryName);
	}

	/*-> Marketplace ToolBar*/
	{
		SectionName = "Content";
		EntryName = "Marketplace";
		ToolMenus->RemoveEntry(MenuName, SectionName, EntryName);
	}

	/*-> Cinematics ToolBar*/
	{
		SectionName = "Misc";
		EntryName = "EditCinematics";
		ToolMenus->RemoveEntry(MenuName, SectionName, EntryName);
	}

	/*-> Compile ToolBar*/
	{
		SectionName = "Compile";
		EntryName = "CompileButton";
		ToolMenus->RemoveEntry(MenuName, SectionName, EntryName);

		EntryName = "CompileComboButton";
		ToolMenus->RemoveEntry(MenuName, SectionName, EntryName);
	}
}

void FSourceClip::ClipEditorMode(void)
{
	FModuleManager& ModuleManager = FModuleManager::Get();

	FName ModuleName;

	/*-> Geometry Mode*/
	{
		ModuleName = TEXT("GeometryMode");
		ModuleManager.UnloadModule(ModuleName, true);
	}

	/*-> Mesh Paint Mode*/
	{
		ModuleName = TEXT("MeshPaintMode");
		ModuleManager.UnloadModule(ModuleName, true);
	}

	/*-> Landscape Editor Mode*/
	{
		ModuleName = TEXT("LandscapeEditor");
		ModuleManager.UnloadModule(ModuleName, true);
	}

	/*-> Foliage Edit Mode*/
	{
		ModuleName = TEXT("FoliageEdit");
		ModuleManager.UnloadModule(ModuleName, true);
	}
}

void FSourceClip::ClipContextMenu(void)
{
}

