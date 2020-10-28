// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;
struct FRawMesh;

class FImportCustomModelModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command. */
	void PluginButtonClicked();
	void ImportModelDataButtonClicked();
	
private:

	int CreateChirdModel(FString Text);//生成多个StaticMesh，一个组为一个StaticMesh,返回上一个模型顶点个数
	void ImportModelData(FRawMesh&);//将数据导入一个StaticMesh
	void AddToolbarExtension(FToolBarBuilder& Builder);
	void AddMenuExtension(FMenuBuilder& Builder);

private:

	UStaticMesh* MyMesh;
	TSharedPtr<class FUICommandList> PluginCommands;
};
