// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FAsynTestModule : public IModuleInterface
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
	void HandleGUVProcessCompleted(TSharedPtr<SNotificationItem> NotificationItemPtr);
	void HandleGUVProcessProgress(uint32 CurrentIndex, uint32 AllNum, TSharedPtr<SNotificationItem> NotificationItemPtr);

private:
	TSharedPtr<class FUICommandList> PluginCommands;
	uint32 RecordTime = 0;
};
