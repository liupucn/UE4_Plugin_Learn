// Copyright 2018 Jack Myth. All Rights Reserved.

#include "EditorCustomizeSetting.h"
#include "SlateMaterialBrush.h"
#include "Materials/MaterialInterface.h"
#include "EditorStyleSet.h"
#include "Classes/EditorStyleSettings.h"
#include <Styling/SlateTypes.h>
#include <Styling/SlateStyle.h>
#include "Styling\SlateStyleRegistry.h"

UEditorCustomizeSetting::UEditorCustomizeSetting()
{
	EditorUseGrid = GetDefault<UEditorStyleSettings>()->bUseGrid;
	InitEditorStyle();
	InitCoreStyle();
	/*-> Init icon style*/
	InitEditorToolbarIconStyle();
	InitEditorModesToolbarIconStyle();
	InitEidtorMenuIconStyle();
	InitEditorContentBrowerIconStyle();
}

UEditorCustomizeSetting::~UEditorCustomizeSetting()
{
	UE_LOG(LogTemp, Warning, TEXT("UEditorCustomizeSetting is destructed!"))
}

void UEditorCustomizeSetting::InitEditorStyle()
{
	Grap_Panel_Background = *FEditorStyle::GetBrush("Graph.Panel.SolidBackground");
	Graph_Panel.GridLineColor = FEditorStyle::GetColor("Graph.Panel.GridLineColor");
	Graph_Panel.GridRuleColor = FEditorStyle::GetColor("Graph.Panel.GridRuleColor");
	Graph_Panel.GridCenterColor = FEditorStyle::GetColor("Graph.Panel.GridCenterColor");
	ToolPanel_GroupBorder = *FEditorStyle::GetBrush("ToolPanel.GroupBorder");
	ToolPanel_DarkGroupBorder = *FEditorStyle::GetBrush("ToolPanel.DarkGroupBorder");
	SCSEditor_TreePanel = *FEditorStyle::GetBrush("SCSEditor.TreePanel");
	SettingsEditor_CheckoutWarningBorder = *FEditorStyle::GetBrush("SettingsEditor.CheckoutWarningBorder");
	TableView_DarkRow = FEditorStyle::GetWidgetStyle<FTableRowStyle>("TableView.DarkRow");
	DetailsView.CategoryTop_Hovered = *FEditorStyle::GetBrush("DetailsView.CategoryTop_Hovered");
	DetailsView.CategoryTop = *FEditorStyle::GetBrush("DetailsView.CategoryTop");
	DetailsView.CategoryMiddle = *FEditorStyle::GetBrush("DetailsView.CategoryMiddle");
	DetailsView.CategoryMiddle_Hovered = *FEditorStyle::GetBrush("DetailsView.CategoryMiddle_Hovered");
	DetailsView.CategoryMiddle_Highlighted = *FEditorStyle::GetBrush("DetailsView.CategoryMiddle_Highlighted");
	DetailsView.CollapsedCategory_Hovered = *FEditorStyle::GetBrush("DetailsView.CollapsedCategory_Hovered");
	DetailsView.CollapsedCategory = *FEditorStyle::GetBrush("DetailsView.CollapsedCategory");
	DetailsView.CategoryBottom = *FEditorStyle::GetBrush("DetailsView.CategoryBottom");
	DetailsView.AdvancedDropdownBorder = *FEditorStyle::GetBrush("DetailsView.AdvancedDropdownBorder");
	E_Toolbar_Background = *FEditorStyle::GetBrush(TEXT("Toolbar.Background"));
	PlacementBrowser_Tab = FEditorStyle::GetWidgetStyle<FCheckBoxStyle>("PlacementBrowser.Tab");
	EditorModesToolbar_ToggleButton = FEditorStyle::GetWidgetStyle<FCheckBoxStyle>("EditorModesToolbar.ToggleButton");
	UMGEditor_Palette.UMGEditor_PaletteHeader = FEditorStyle::GetWidgetStyle<FTableRowStyle>("UMGEditor.PaletteHeader");
	UMGEditor_Palette.UMGEditor_PaletteItem = FEditorStyle::GetWidgetStyle<FTableRowStyle>("UMGEditor.PaletteItem");
	Docking_Tab_ContentAreaBrush = *FEditorStyle::GetBrush("Docking.Tab.ContentAreaBrush");
	ContentBrowser_TopBar_GroupBorder = *FEditorStyle::GetBrush("ContentBrowser.TopBar.GroupBorder");
	ContentBrowser_TileViewTooltip_NonContentBorder = *FEditorStyle::GetBrush("ContentBrowser.TileViewTooltip.NonContentBorder");
	ContentBrowser_TileViewTooltip_ContentBorder = *FEditorStyle::GetBrush("ContentBrowser.TileViewTooltip.ContentBorder");
}

void UEditorCustomizeSetting::InitCoreStyle()
{
	WindowStyle = FCoreStyle::Get().GetWidgetStyle<FWindowStyle>("Window");
	Docking_MajorTab = FCoreStyle::Get().GetWidgetStyle<FDockTabStyle>("Docking.MajorTab");
	Docking_Tab = FCoreStyle::Get().GetWidgetStyle<FDockTabStyle>("Docking.Tab");
	TableView_Header = FCoreStyle::Get().GetWidgetStyle<FHeaderRowStyle>("TableView.Header");
	SearchBox = FCoreStyle::Get().GetWidgetStyle<FSearchBoxStyle>("SearchBox");
	ScrollBar = FCoreStyle::Get().GetWidgetStyle<FScrollBarStyle>("Scrollbar");
	ToolBar_Background = *FCoreStyle::Get().GetBrush("ToolBar.Background");
	ToolBar_Separator = *FCoreStyle::Get().GetBrush("ToolBar.Separator");
	ToolTip_BrightBackground = *FCoreStyle::Get().GetBrush("ToolTip.BrightBackground");
	Menu_Background = *FCoreStyle::Get().GetBrush("Menu.Background");
}

void UEditorCustomizeSetting::InitEditorToolbarIconStyle()
{
	AssetEditor_SaveAsset = *FEditorStyle::GetBrush("AssetEditor.SaveAsset");
	LevelEditor_OpenContentBrowser = *FEditorStyle::GetBrush("LevelEditor.OpenContentBrowser");
	LevelEditor_GameSettings = *FEditorStyle::GetBrush("LevelEditor.GameSettings");
	LevelEditor_OpenLevelBlueprint = *FEditorStyle::GetBrush("LevelEditor.OpenLevelBlueprint");
	LevelEditor_EditMatinee = *FEditorStyle::GetBrush("LevelEditor.EditMatinee");
	LevelEditor_Build = *FEditorStyle::GetBrush("LevelEditor.Build");
	PlayWorld.PlayWorld_PlayInViewport = *FEditorStyle::GetBrush("PlayWorld.PlayInViewport");
	PlayWorld.PlayWorld_PausePlaySession = *FEditorStyle::GetBrush("PlayWorld.PausePlaySession");
	PlayWorld.PlayWorld_StopPlaySession = *FEditorStyle::GetBrush("PlayWorld.StopPlaySession");
	PlayWorld.PlayWorld_EjectFromPlayer = *FEditorStyle::GetBrush("PlayWorld.EjectFromPlayer");
	PlayWorld.PlayWorld_LateJoinSession = *FEditorStyle::GetBrush("PlayWorld.LateJoinSession");
	PlayWorld.PlayWorld_SingleFrameAdvance = *FEditorStyle::GetBrush("PlayWorld.SingleFrameAdvance");
	PlayWorld.PlayWorld_PossessPlayer = *FEditorStyle::GetBrush("PlayWorld.PossessPlayer");

	/* VSP Icon*/
	GetModuleStyleSlateBrush("MaterialContentStyle", "MaterialContent.PluginAction", VSP_MaterialContent);
	GetModuleStyleSlateBrush("CaveVRPackageStyle", "CavePackage.PackageCaveAction", VSP_Package);
	GetModuleStyleSlateBrush("SoErgoUIStyle", "SoErgo.SoErgo", VSP_SoErgo);
	GetModuleStyleSlateBrush("VrDeviceUIStyle", "VrDeviceUIEditor.PluginWindow", VSP_VRDevice);
	GetModuleStyleSlateBrush("GAGCEditorStyle", "EzStyleEditor.OpenEzStyleWindow", VSP_ScenePreSet);
	GetModuleStyleSlateBrush("GAGCEditorStyle", "EzStyleEditor.OpenSequenceBatchRenderingWindow", VSP_RenderQueue);
}

void UEditorCustomizeSetting::InitEditorModesToolbarIconStyle()
{
	LevelEditor_PlacementMode = *FEditorStyle::GetBrush("LevelEditor.PlacementMode");
	LevelEditor_MeshPaintMode = *FEditorStyle::GetBrush("LevelEditor.MeshPaintMode");
	LevelEditor_LandscapeMode = *FEditorStyle::GetBrush("LevelEditor.LandscapeMode");
	LevelEditor_FoliageMode = *FEditorStyle::GetBrush("LevelEditor.FoliageMode");
	LevelEditor_BspMode = *FEditorStyle::GetBrush("LevelEditor.BspMode");

	/* VSP Icon*/
	GetModuleStyleSlateBrush("DMUToolStyle", "DMUTool.OpenPluginWindow", DMUToolMode);
	GetModuleStyleSlateBrush("FeatureOperationStyle", "LevelEditor.FeatureOperationMode", FeatureOperationMode);
	GetModuleStyleSlateBrush("MultiVizStyle", "LevelEditor.MultiVizMode", MultiVizMode);
	GetModuleStyleSlateBrush("ControlSkeletonEditorStyle", "ControlSkeletonEditMode", ControlSkeletonEditMode);
	GetModuleStyleSlateBrush("PivotToolStyle", "PivotTool.PivotToolMode", PivotToolMode);
}

void UEditorCustomizeSetting::InitEidtorMenuIconStyle()
{
}

void UEditorCustomizeSetting::InitEditorContentBrowerIconStyle()
{
	ContentBrowser_AssetTreeFolderOpen = *FEditorStyle::GetBrush("ContentBrowser.AssetTreeFolderOpen");
	ContentBrowser_AssetTreeFolderClosed = *FEditorStyle::GetBrush("ContentBrowser.AssetTreeFolderClosed");
	ContentBrowser_AssetTreeFolderOpenCode = *FEditorStyle::GetBrush("ContentBrowser.AssetTreeFolderOpenCode");
	ContentBrowser_AssetTreeFolderClosedCode = *FEditorStyle::GetBrush("ContentBrowser.AssetTreeFolderClosedCode");
	SceneOutliner_FolderClosed = *FEditorStyle::GetBrush("SceneOutliner.FolderClosed");
	SceneOutliner_FolderOpen = *FEditorStyle::GetBrush("SceneOutliner.FolderOpen");
}

void UEditorCustomizeSetting::GetModuleStyleSlateBrush(FName ModuleStyleName, FName SlateBrushName, FSlateBrush& OutSlateBrush)
{
	if (const ISlateStyle* FoundStyle = FSlateStyleRegistry::FindSlateStyle(ModuleStyleName))
	{
		if (const FSlateBrush* FoundBrush = FoundStyle->GetBrush(SlateBrushName))
		{
			OutSlateBrush = *FoundBrush;
		}
	}
}

