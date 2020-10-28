// Copyright 2018 Jack Myth. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Styling/SlateTypes.h"
#include "Brushes\SlateBoxBrush.h"
#include "EditorCustomizeSetting.generated.h"

USTRUCT()
struct FEditorCustomizeDetailsView
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category="DetailsViewStyle")
		FSlateBrush CategoryTop_Hovered;

	UPROPERTY(EditAnywhere, Category = "DetailsViewStyle")
		FSlateBrush CategoryTop;

	UPROPERTY(EditAnywhere, Category = "DetailsViewStyle")
		FSlateBrush CategoryMiddle_Highlighted;
	
	UPROPERTY(EditAnywhere, Category = "DetailsViewStyle")
		FSlateBrush CategoryMiddle_Hovered;

	UPROPERTY(EditAnywhere, Category = "DetailsViewStyle")
		FSlateBrush CategoryMiddle;

	UPROPERTY(EditAnywhere, Category = "DetailsViewStyle")
		FSlateBrush CollapsedCategory_Hovered;

	UPROPERTY(EditAnywhere, Category = "DetailsViewStyle")
		FSlateBrush CollapsedCategory;

	UPROPERTY(EditAnywhere, Category = "DetailsViewStyle")
		FSlateBrush CategoryBottom;

	UPROPERTY(EditAnywhere, Category = "DetailsViewStyle")
		FSlateBrush AdvancedDropdownBorder;
};

USTRUCT()
struct FPropertyWindow
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "PropertyWindow")
		FSlateFontInfo NormalFont;

	UPROPERTY(EditAnywhere, Category = "PropertyWindow")
		FSlateFontInfo BoldFont;

	UPROPERTY(EditAnywhere, Category = "PropertyWindow")
		FSlateFontInfo ItalicFont;
};

USTRUCT()
struct FContentBrowserFont
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "ContentBrowser")
		FSlateFontInfo SourceTreeRootItemFont;

	UPROPERTY(EditAnywhere, Category = "ContentBrowser")
		FSlateFontInfo SourceTreeItemFont;

	UPROPERTY(EditAnywhere, Category = "ContentBrowser")
		FSlateFontInfo AssetTileViewNameFontVerySmall;

	UPROPERTY(EditAnywhere, Category = "ContentBrowser")
		FSlateFontInfo AssetTileViewNameFontSmall;

	UPROPERTY(EditAnywhere, Category = "ContentBrowser")
		FSlateFontInfo AssetTileViewNameFont;

	UPROPERTY(EditAnywhere, Category = "ContentBrowser")
		FTextBlockStyle TopBar_Font;

	UPROPERTY(EditAnywhere, Category = "ContentBrowser")
		FTextBlockStyle PathText;
};

USTRUCT()
struct FGraphPanelStyle
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "GraphPanelStyle")
		FLinearColor GridLineColor;

	UPROPERTY(EditAnywhere, Category = "GraphPanelStyle")
		FLinearColor GridRuleColor;

	UPROPERTY(EditAnywhere, Category = "GraphPanelStyle")
		FLinearColor GridCenterColor;
};

USTRUCT()
struct FUMGEditorPalette
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "UMGEditorPalette", meta = (DisplayName = "UMGEditor.PaletteHeader"))
		FTableRowStyle UMGEditor_PaletteHeader;

	UPROPERTY(EditAnywhere, Category = "UMGEditorPalette", meta = (DisplayName = "UMGEditor.PaletteItem"))
		FTableRowStyle UMGEditor_PaletteItem;
};

USTRUCT()
struct FUE4ECCustomStyle
{
	GENERATED_USTRUCT_BODY()

	//////////////////////////////////////////////////////////////////////////
	////////////////////////Internal use only////////////////////////////////
	UPROPERTY(Config)
		int CustomStyleCount = 0;
	//////////////////////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere,Category="CustomStyle", meta = (DisplayName = "FSlateBrush"))
		TMap<FName, FSlateBrush> SlateBrush;

	UPROPERTY(EditAnywhere,Category="CustomStyle",meta=(DisplayName="SlateFontInfo"))
		TMap<FName, FSlateFontInfo> SlateFontInfo;

	UPROPERTY(EditAnywhere,Category="CustomStyle", meta = (DisplayName = "FTextBlockStyle"))
		TMap<FName, FTextBlockStyle> TextBlockStyle;

	UPROPERTY(EditAnywhere,Category="CustomStyle", meta = (DisplayName = "FTextBlockStyle"))
		TMap<FName, FButtonStyle> ButtonStyle;

	UPROPERTY(EditAnywhere,Category="CustomStyle", meta = (DisplayName = "FComboButtonStyle"))
		TMap<FName, FComboButtonStyle> ComboButtonStyle;

	UPROPERTY(EditAnywhere,Category="CustomStyle", meta = (DisplayName = "FComboBoxStyle"))
		TMap<FName, FComboBoxStyle> ComboBoxStyle;

	UPROPERTY(EditAnywhere,Category="CustomStyle", meta = (DisplayName = "FEditableTextStyle"))
		TMap<FName, FEditableTextStyle> EditableTextStyle;

	UPROPERTY(EditAnywhere,Category="CustomStyle", meta = (DisplayName = "FScrollBarStyle"))
		TMap<FName, FScrollBarStyle> ScrollBarStyle;

	UPROPERTY(EditAnywhere,Category="CustomStyle", meta = (DisplayName = "FEditableTextBoxStyle"))
		TMap<FName, FEditableTextBoxStyle> EditableTextBoxStyle;

	UPROPERTY(EditAnywhere,Category="CustomStyle", meta = (DisplayName = "FInlineEditableTextBlockStyle"))
		TMap<FName, FInlineEditableTextBlockStyle> InlineEditableTextBlockStyle;

	UPROPERTY(EditAnywhere,Category="CustomStyle", meta = (DisplayName = "FProgressBarStyle"))
		TMap<FName, FProgressBarStyle> ProgressBarStyle;

	UPROPERTY(EditAnywhere,Category="CustomStyle", meta = (DisplayName = "FExpandableAreaStyle"))
		TMap<FName, FExpandableAreaStyle> ExpandableAreaStyle;

	UPROPERTY(EditAnywhere,Category="CustomStyle", meta = (DisplayName = "FSearchBoxStyle"))
		TMap<FName, FSearchBoxStyle> SearchBoxStyle;

	UPROPERTY(EditAnywhere,Category="CustomStyle", meta = (DisplayName = "FSliderStyle"))
		TMap<FName, FSliderStyle> SliderStyle;

	UPROPERTY(EditAnywhere,Category="CustomStyle", meta = (DisplayName = "FVolumeControlStyle"))
		TMap<FName, FVolumeControlStyle> VolumeControlStyle;

	UPROPERTY(EditAnywhere,Category="CustomStyle", meta = (DisplayName = "FInlineTextImageStyle"))
		TMap<FName, FInlineTextImageStyle> InlineTextImageStyle;

	UPROPERTY(EditAnywhere,Category="CustomStyle", meta = (DisplayName = "FSpinBoxStyle"))
		TMap<FName, FSpinBoxStyle> SpinBoxStyle;

	UPROPERTY(EditAnywhere,Category="CustomStyle", meta = (DisplayName = "FSplitterStyle"))
		TMap<FName, FSplitterStyle> SplitterStyle;

	UPROPERTY(EditAnywhere,Category="CustomStyle", meta = (DisplayName = "FTableRowStyle"))
		TMap<FName, FTableRowStyle> TableRowStyle;

	UPROPERTY(EditAnywhere,Category="CustomStyle", meta = (DisplayName = "FTableColumnHeaderStyle"))
		TMap<FName, FTableColumnHeaderStyle> TableColumnHeaderStyle;

	//UPROPERTY(EditAnywhere,Category="CustomStyle", meta = (DisplayName = "FHeaderRowStyle"))
		//TMap<FName, FHeaderRowStyle> HeaderRowStyle;

	UPROPERTY(EditAnywhere,Category="CustomStyle", meta = (DisplayName = "FDockTabStyle"))
		TMap<FName, FDockTabStyle> DockTabStyle;

	UPROPERTY(EditAnywhere,Category="CustomStyle", meta = (DisplayName = "FScrollBoxStyle"))
		TMap<FName, FScrollBoxStyle> ScrollBoxStyle;

	UPROPERTY(EditAnywhere,Category="CustomStyle", meta = (DisplayName = "FScrollBorderStyle"))
		TMap<FName, FScrollBorderStyle> ScrollBorderStyle;

	UPROPERTY(EditAnywhere,Category="CustomStyle", meta = (DisplayName = "FWindowStyle"))
		TMap<FName, FWindowStyle> WindowStyle;

};

/*-> EditorToolbar icon Play World */
USTRUCT()
struct FEditorToolbarIconPlayWorld
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "ToolbarIconPlayWorld")
		FSlateBrush PlayWorld_PlayInViewport;

	UPROPERTY(EditAnywhere, Category = "ToolbarIconPlayWorld")
		FSlateBrush PlayWorld_PausePlaySession;

	UPROPERTY(EditAnywhere, Category = "ToolbarIconPlayWorld")
		FSlateBrush PlayWorld_StopPlaySession;

	UPROPERTY(EditAnywhere, Category = "ToolbarIconPlayWorld")
		FSlateBrush PlayWorld_EjectFromPlayer;

	UPROPERTY(EditAnywhere, Category = "ToolbarIconPlayWorld")
		FSlateBrush PlayWorld_LateJoinSession;

	UPROPERTY(EditAnywhere, Category = "ToolbarIconPlayWorld")
		FSlateBrush PlayWorld_SingleFrameAdvance;

	UPROPERTY(EditAnywhere, Category = "ToolbarIconPlayWorld")
		FSlateBrush PlayWorld_PossessPlayer;

};

/**
 * 
 */
UCLASS(config = EditorSettings)
class UEditorCustomizeSetting : public UObject
{
	GENERATED_BODY()

public:
	UEditorCustomizeSetting();

	~UEditorCustomizeSetting();

	UPROPERTY(Config, EditAnywhere, Category = "Editor Style", meta = (DisplayName = "Editor.UseGrid"))
		bool EditorUseGrid;

	UPROPERTY(Config,EditAnywhere,Category="Editor Style", meta = (DisplayName = "Graph.Panel.SolidBackground"))
		FSlateBrush Grap_Panel_Background;

	UPROPERTY(Config, EditAnywhere, Category = "Editor Style", meta = (DisplayName = "Graph.Panel"))
		FGraphPanelStyle Graph_Panel;

	UPROPERTY(Config, EditAnywhere, Category = "Editor Style", meta = (DisplayName = "ToolPanel.GroupBorder"))
		FSlateBrush ToolPanel_GroupBorder;

	UPROPERTY(Config, EditAnywhere, Category = "Editor Style", meta = (DisplayName = "ToolPanel.DarkGroupBorder"))
		FSlateBrush ToolPanel_DarkGroupBorder;

	UPROPERTY(Config, EditAnywhere, Category = "Editor Style", meta = (DisplayName = "SCSEditor.TreePanel"))
		FSlateBrush SCSEditor_TreePanel;

	UPROPERTY(Config, EditAnywhere, Category = "Editor Style", meta = (DisplayName = "SettingsEditor.CheckoutWarningBorder"))
		FSlateBrush SettingsEditor_CheckoutWarningBorder;

	UPROPERTY(Config, EditAnywhere, Category = "Editor Style", meta = (DisplayName = "Docking.Tab.ContentAreaBrush"))
		FSlateBrush Docking_Tab_ContentAreaBrush;

	UPROPERTY(Config,EditAnywhere,Category="Editor Style",meta = (DisplayName="ContentBrowser.TopBar.GroupBorder"))
		FSlateBrush ContentBrowser_TopBar_GroupBorder;

	UPROPERTY(Config, EditAnywhere, Category = "Editor Style", meta = (DisplayName = "ContentBrowser.TileViewTooltip.NonContentBorder"))
		FSlateBrush ContentBrowser_TileViewTooltip_NonContentBorder;
	
	UPROPERTY(Config, EditAnywhere, Category = "Editor Style", meta = (DisplayName = "ContentBrowser.TileViewTooltip.ContentBorder"))
		FSlateBrush ContentBrowser_TileViewTooltip_ContentBorder;

	UPROPERTY(Config, EditAnywhere, Category = "Editor Style", meta = (DisplayName = "TableView.DarkRow"))
		FTableRowStyle TableView_DarkRow;

	UPROPERTY(Config, EditAnywhere, Category = "Editor Style", meta = (DisplayName = "DetailsView"))
		FEditorCustomizeDetailsView DetailsView;

	UPROPERTY(Config, EditAnywhere, Category = "Editor Style", meta = (DisplayName = "PlacementBrowser.Tab"))
		FCheckBoxStyle PlacementBrowser_Tab;

	UPROPERTY(Config, EditAnywhere, Category = "Editor Style", meta = (DisplayName = "EditorModesToolbar.ToggleButton"))
		FCheckBoxStyle EditorModesToolbar_ToggleButton;

	UPROPERTY(Config, EditAnywhere, Category = "Editor Style", meta = (DisplayName = "Toolbar.Background"))
		FSlateBrush E_Toolbar_Background;

	UPROPERTY(Config, EditAnywhere, Category = "Editor Style", meta = (DisplayName = "UMGEditor.Palette"))
		FUMGEditorPalette UMGEditor_Palette;

	UPROPERTY(Config, EditAnywhere, Category = "Core Style", meta = (DisplayName = "WindowStyle"))
		FWindowStyle WindowStyle;

	UPROPERTY(Config, EditAnywhere, Category = "Core Style", meta = (DisplayName = "Docking.MajorTab"))
		FDockTabStyle Docking_MajorTab;

	UPROPERTY(Config, EditAnywhere, Category = "Core Style", meta = (DisplayName = "Docking.Tab"))
		FDockTabStyle Docking_Tab;

	UPROPERTY(Config, EditAnywhere, Category = "Core Style", meta = (DisplayName = "TableView.Header"))
		FHeaderRowStyle TableView_Header;

	UPROPERTY(Config, EditAnywhere, Category = "Core Style", meta = (DisplayName = "SearchBox"))
		FSearchBoxStyle SearchBox;

	UPROPERTY(Config, EditAnywhere, Category = "Core Style", meta = (DisplayName = "ScrollBar"))
		FScrollBarStyle ScrollBar;

	UPROPERTY(Config, EditAnywhere, Category = "Core Style", meta = (DisplayName = "ToolBar.Background"))
		FSlateBrush ToolBar_Background;

	UPROPERTY(Config, EditAnywhere, Category = "Core Style", meta = (DisplayName = "ToolBar.Separator"))
		FSlateBrush ToolBar_Separator;
	
	UPROPERTY(Config, EditAnywhere, Category = "Core Style", meta = (DisplayName = "ToolTip.BrightBackground"))
		FSlateBrush ToolTip_BrightBackground;

	UPROPERTY(Config, EditAnywhere, Category = "Core Style", meta = (DisplayName = "Menu.Background"))
		FSlateBrush Menu_Background;

public:
	/*-> Editor Toolbar icon*/
	UPROPERTY(Config, EditAnywhere, Category = "Editor Toolbar Icon", meta = (DisplayName = "AssetEditor.SaveAsset"))
		FSlateBrush AssetEditor_SaveAsset;

	UPROPERTY(Config, EditAnywhere, Category = "Editor Toolbar Icon", meta = (DisplayName = "LevelEditor.OpenContentBrowser"))
		FSlateBrush LevelEditor_OpenContentBrowser;

	UPROPERTY(Config, EditAnywhere, Category = "Editor Toolbar Icon", meta = (DisplayName = "LevelEditor.GameSettings"))
		FSlateBrush LevelEditor_GameSettings;

	UPROPERTY(Config, EditAnywhere, Category = "Editor Toolbar Icon", meta = (DisplayName = "LevelEditor.OpenLevelBlueprint"))
		FSlateBrush LevelEditor_OpenLevelBlueprint;

	UPROPERTY(Config, EditAnywhere, Category = "Editor Toolbar Icon", meta = (DisplayName = "LevelEditor.EditMatinee"))
		FSlateBrush LevelEditor_EditMatinee;

	UPROPERTY(Config, EditAnywhere, Category = "Editor Toolbar Icon", meta = (DisplayName = "LevelEditor.Build"))
		FSlateBrush LevelEditor_Build;

	UPROPERTY(Config, EditAnywhere, Category = "Editor Toolbar Icon", meta = (DisplayName = "PlayWorld"))
		FEditorToolbarIconPlayWorld PlayWorld;

	// VSP Icon
	UPROPERTY(Config, EditAnywhere, Category = "Editor Toolbar Icon", meta = (DisplayName = "MaterialContent"))
	FSlateBrush VSP_MaterialContent;

	UPROPERTY(Config, EditAnywhere, Category = "Editor Toolbar Icon", meta = (DisplayName = "Package"))
	FSlateBrush	VSP_Package;

	UPROPERTY(Config, EditAnywhere, Category = "Editor Toolbar Icon", meta = (DisplayName = "SoErgo"))
	FSlateBrush	VSP_SoErgo;

	UPROPERTY(Config, EditAnywhere, Category = "Editor Toolbar Icon", meta = (DisplayName = "VRDevice"))
	FSlateBrush VSP_VRDevice;

	UPROPERTY(Config, EditAnywhere, Category = "Editor Toolbar Icon", meta = (DisplayName = "RenderQueue"))
	FSlateBrush VSP_RenderQueue;

	UPROPERTY(Config, EditAnywhere, Category = "Editor Toolbar Icon", meta = (DisplayName = "ScenePreSet"))
	FSlateBrush VSP_ScenePreSet;

	/*-> EditorModes Toolbar icon*/
	UPROPERTY(Config, EditAnywhere, Category = "EditorModes Toolbar Icon", meta = (DisplayName = "LevelEditor.PlacementMode"))
		FSlateBrush LevelEditor_PlacementMode;

	UPROPERTY(Config, EditAnywhere, Category = "EditorModes Toolbar Icon", meta = (DisplayName = "LevelEditor.MeshPaintMode"))
		FSlateBrush LevelEditor_MeshPaintMode;

	UPROPERTY(Config, EditAnywhere, Category = "EditorModes Toolbar Icon", meta = (DisplayName = "LevelEditor.LandscapeMode"))
		FSlateBrush LevelEditor_LandscapeMode;

	UPROPERTY(Config, EditAnywhere, Category = "EditorModes Toolbar Icon", meta = (DisplayName = "LevelEditor_FoliageMode"))
		FSlateBrush LevelEditor_FoliageMode;

	UPROPERTY(Config, EditAnywhere, Category = "EditorModes Toolbar Icon", meta = (DisplayName = "LevelEditor.BspMode"))
		FSlateBrush LevelEditor_BspMode;

	// VSP Icon.
	UPROPERTY(Config, EditAnywhere, Category = "EditorModes Toolbar Icon", meta = (DisplayName = "LevelEditor.DMUToolMode"))
	FSlateBrush DMUToolMode;
	UPROPERTY(Config, EditAnywhere, Category = "EditorModes Toolbar Icon", meta = (DisplayName = "LevelEditor.FeatureOperationMode"))
	FSlateBrush	FeatureOperationMode;
	UPROPERTY(Config, EditAnywhere, Category = "EditorModes Toolbar Icon", meta = (DisplayName = "LevelEditor.MultiVizMode"))
	FSlateBrush	MultiVizMode;
	UPROPERTY(Config, EditAnywhere, Category = "EditorModes Toolbar Icon", meta = (DisplayName = "LevelEditor.ControlSkeletonEditMode"))
	FSlateBrush	ControlSkeletonEditMode;
	UPROPERTY(Config, EditAnywhere, Category = "EditorModes Toolbar Icon", meta = (DisplayName = "LevelEditor.PivotToolMode"))
	FSlateBrush PivotToolMode;

	//TODO Eidtor Menu icon

	/*-> Editor ContentBrower */
	UPROPERTY(Config, EditAnywhere, Category = "Editor ContentBrower", meta = (DisplayName = "ContentBrowser.AssetTreeFolderOpen"))
		FSlateBrush ContentBrowser_AssetTreeFolderOpen;

	UPROPERTY(Config, EditAnywhere, Category = "Editor ContentBrower", meta = (DisplayName = "ContentBrowser.AssetTreeFolderClosed"))
		FSlateBrush ContentBrowser_AssetTreeFolderClosed;

	UPROPERTY(Config, EditAnywhere, Category = "Editor ContentBrower", meta = (DisplayName = "ContentBrowser.AssetTreeFolderOpenCode"))
		FSlateBrush ContentBrowser_AssetTreeFolderOpenCode;

	UPROPERTY(Config, EditAnywhere, Category = "Editor ContentBrower", meta = (DisplayName = "ContentBrowser.AssetTreeFolderClosedCode"))
		FSlateBrush ContentBrowser_AssetTreeFolderClosedCode;
	
	UPROPERTY(Config, EditAnywhere, Category = "Editor ContentBrower", meta = (DisplayName = "SceneOutliner.FolderClosed"))
		FSlateBrush SceneOutliner_FolderClosed;
	
	UPROPERTY(Config, EditAnywhere, Category = "Editor ContentBrower", meta = (DisplayName = "SceneOutliner.FolderOpen"))
		FSlateBrush SceneOutliner_FolderOpen;

public:
	void InitEditorStyle();
	void InitCoreStyle();
	/*-> Init icon style*/
	void InitEditorToolbarIconStyle();
	void InitEditorModesToolbarIconStyle();
	void InitEidtorMenuIconStyle();
	void InitEditorContentBrowerIconStyle();

private:
	void GetModuleStyleSlateBrush(FName ModuleStyleName, FName SlateBrushName, FSlateBrush& OutSlateBrush);
};

