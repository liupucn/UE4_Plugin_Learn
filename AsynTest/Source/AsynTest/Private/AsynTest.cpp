// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "AsynTest.h"
#include "AsynTestStyle.h"
#include "AsynTestCommands.h"
#include "Misc/MessageDialog.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

#include "LevelEditor.h"

#include "Work.h"
#include "MyRunnable.h"

#include "Notifications/INotificationWidget.h"
#include "Notifications/NotificationManager.h"


static const FName AsynTestTabName("AsynTest");

#define LOCTEXT_NAMESPACE "FAsynTestModule"

void FAsynTestModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FAsynTestStyle::Initialize();
	FAsynTestStyle::ReloadTextures();

	FAsynTestCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FAsynTestCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FAsynTestModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FAsynTestModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FAsynTestModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}
}

void FAsynTestModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FAsynTestStyle::Shutdown();

	FAsynTestCommands::Unregister();
}

void FAsynTestModule::PluginButtonClicked()
{
	EAppReturnType::Type UserResponse = FMessageDialog::Open(EAppMsgType::OkCancel, FText::FromString(TEXT("Task is run without Asyn!")));

#if 0

	//AsyncTask 异步操作
	FAsyncTask<FMyTask>* NewWork = new FAsyncTask<FMyTask>("Task_0", "Task_1", "Task_2");
	NewWork->StartBackgroundTask();


#elif 0

	//AsyncTask 同步操作
	FAsyncTask<FMyTask>* NewWork = new FAsyncTask<FMyTask>("Task_0", "Task_1", "Task_2");
	NewWork->StartSynchronousTask();

#elif 0

	//TaskGraph
	FTaskGraphTest::Start();

#elif 1

	//FRunnable
	FMyRunnable* MyRunnable = new FMyRunnable();

	/*信息提示*/
	FNotificationInfo Info(FText::FromString(FString::Printf(TEXT("Calculating UV Coordinate: %i / "), 0) + FString::Printf(TEXT("%i"), 1000)));
	Info.bFireAndForget = false;  //信息提示栏将一直显示直至调用FadeOut()
	TSharedPtr<SNotificationItem> NotificationItem = FSlateNotificationManager::Get().AddNotification(Info);
	NotificationItem->SetCompletionState(SNotificationItem::CS_Pending);

	if (NotificationItem.IsValid())
	{
		MyRunnable->OnCompletedDelegate().BindRaw(this, &FAsynTestModule::HandleGUVProcessCompleted, NotificationItem);
		MyRunnable->OnProgressDelegate().BindRaw(this, &FAsynTestModule::HandleGUVProcessProgress, NotificationItem);
	}
#endif
}

void FAsynTestModule::HandleGUVProcessCompleted(TSharedPtr<SNotificationItem> NotificationItemPtr)
{
	FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
		FSimpleDelegateGraphTask::FDelegate::CreateLambda([=]() {

		NotificationItemPtr->SetCompletionState(SNotificationItem::CS_Success);
		NotificationItemPtr->Fadeout();

		FNotificationInfo Info(LOCTEXT("Runnable world", "Runnable is Done!"));
		Info.ExpireDuration = 3.0f;
		TSharedPtr<SNotificationItem> NotificationItem = FSlateNotificationManager::Get().AddNotification(Info);

	}),
		TStatId(),
		nullptr,
		ENamedThreads::GameThread
		);
}

void FAsynTestModule::HandleGUVProcessProgress(uint32 CurrentIndex, uint32 AllNum, TSharedPtr<SNotificationItem> NotificationItemPtr)
{
	FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
		FSimpleDelegateGraphTask::FDelegate::CreateLambda([=]() {

		FString OutMessage = FString::Printf(TEXT("Calculating UV Coordinate: %i / "), CurrentIndex + 1) + FString::Printf(TEXT("%i"), AllNum);
		NotificationItemPtr->SetText(FText::FromString(OutMessage));

	}),
		TStatId(),
		nullptr,
		ENamedThreads::GameThread
		);
}

void FAsynTestModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FAsynTestCommands::Get().PluginAction);
}

void FAsynTestModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FAsynTestCommands::Get().PluginAction);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAsynTestModule, AsynTest)