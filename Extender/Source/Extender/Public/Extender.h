// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;
class FExtender;
struct FAssetData;
class UStaticMesh;
class FUICommandList;

class FExtenderModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command. */
	void PluginButtonClicked();
	
private:

	void AddToolbarExtension(FToolBarBuilder& Builder);
	void AddMenuExtension(FMenuBuilder& Builder);
	static void SetupContentBrowserContextMenuExtender();

	static TSharedRef<FExtender> OnExtendContentBrowserAssetSelectionMenu(const TArray<FAssetData>& SelectedAssets);
	static void CustomAction(TArray<UStaticMesh*> StaticMeshes);

	static TSharedRef<FExtender> ExtendStaticMeshEditorToolbar(const TSharedRef<FUICommandList> CommandList, const TArray<UObject*> Objects);
	static void CreateMenu(FMenuBuilder& ParentMenuBuilder, const TSharedRef<FUICommandList> CommandList, UStaticMesh* InStaticMesh);

private:
	TSharedPtr<class FUICommandList> PluginCommands;
};
