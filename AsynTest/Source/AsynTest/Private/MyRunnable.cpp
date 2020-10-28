#include "MyRunnable.h"
#include "EngineUtils.h"
#include "Misc/MessageDialog.h"

FMyRunnable::FMyRunnable()
{
	Thread = FRunnableThread::Create(this, TEXT("GeneratingUV"), 0, TPri_Normal); //实际的线程创建工作
}

FMyRunnable::~FMyRunnable()
{
	if (Thread) delete Thread;
}

uint32 FMyRunnable::Run()
{
	Task();
	return 1;
}

void FMyRunnable::Task()
{
	int all = 1000;
	for (int i = 0;i < all;i++)
	{
		FGraphEventRef QuitTask = FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
			FSimpleDelegateGraphTask::FDelegate::CreateLambda([=]() {

			FString text = "Task_" + FString::FromInt(i);

			//EAppReturnType::Type UserResponse = FMessageDialog::Open(EAppMsgType::OkCancel, FText::FromString(text));
		}),
			TStatId(),
			nullptr,
			ENamedThreads::GameThread
			);
		ProgressDelegate.ExecuteIfBound(i, all);
	}
	CompletedDelegate.ExecuteIfBound();
}
