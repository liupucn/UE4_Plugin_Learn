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

	int CreateChirdModel(FString Text);//���ɶ��StaticMesh��һ����Ϊһ��StaticMesh,������һ��ģ�Ͷ������
	void ImportModelData(FRawMesh&);//�����ݵ���һ��StaticMesh
	void AddToolbarExtension(FToolBarBuilder& Builder);
	void AddMenuExtension(FMenuBuilder& Builder);

private:

	UStaticMesh* MyMesh;
	TSharedPtr<class FUICommandList> PluginCommands;
};
