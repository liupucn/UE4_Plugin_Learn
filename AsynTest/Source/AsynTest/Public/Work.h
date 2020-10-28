#pragma once

#include "CoreMinimal.h"
#include "Async/ParallelFor.h"
#include "Async/AsyncWork.h"
#include "Misc/MessageDialog.h"
#include "Modules/ModuleManager.h"

class FMyTask :public FNonAbandonableTask
{
public:
	FMyTask(const FString TN0, const FString TN1, const FString TN2)
		: TaskName0("task0")
		, TaskName1("task1")
		, TaskName2("task2")
	{
		TaskName0 = TN0;
		TaskName1 = TN1;
		TaskName2 = TN2;
	}

	void Function0(FString TN0)
	{
		EAppReturnType::Type UserResponse = FMessageDialog::Open(EAppMsgType::OkCancel, FText::FromString(TaskName0));
	}

	void Function1(FString TN1)
	{
		EAppReturnType::Type UserResponse = FMessageDialog::Open(EAppMsgType::OkCancel, FText::FromString(TaskName1));
	}

	void Function2(FString TN2)
	{
		EAppReturnType::Type UserResponse = FMessageDialog::Open(EAppMsgType::OkCancel, FText::FromString(TaskName2));
	}

	void DoWork()
	{
		Function0(TaskName0);
		Function1(TaskName1);
		Function2(TaskName2);
	}

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FAsyncImportMorphTargetWork, STATGROUP_ThreadPoolAsyncTasks);
	}

private:
	FString TaskName0;
	FString TaskName1;
	FString TaskName2;
};

namespace FTaskGraphTest
{
	//任务A
	class FTaskA
	{
	public:
		FTaskA(const FString Na)
			: Name("DefaultName")
		{
			Name = Na;
		}

		static FORCEINLINE TStatId GetStatId()
		{
			RETURN_QUICK_DECLARE_CYCLE_STAT(FParallelBlendPhysicsTask, STATGROUP_TaskGraphTasks);
		}
		static FORCEINLINE ENamedThreads::Type GetDesiredThread()
		{
			return ENamedThreads::AnyThread;
		}
		static FORCEINLINE ESubsequentsMode::Type GetSubsequentsMode()
		{
			return ESubsequentsMode::TrackSubsequents;
		}

		void DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
		{
			EAppReturnType::Type UserResponse = FMessageDialog::Open(EAppMsgType::OkCancel, FText::FromString(Name));
		}

	private:
		FString Name;
	};

	//任务B
	class FTaskB
	{
	public:
		FTaskB(const FString Na)
			: Name("DefaultName")
		{
			Name = Na;
		}

		static FORCEINLINE TStatId GetStatId()
		{
			RETURN_QUICK_DECLARE_CYCLE_STAT(FParallelBlendPhysicsTask, STATGROUP_TaskGraphTasks);
		}
		static FORCEINLINE ENamedThreads::Type GetDesiredThread()
		{
			return ENamedThreads::AnyThread;
		}
		static FORCEINLINE ESubsequentsMode::Type GetSubsequentsMode()
		{
			return ESubsequentsMode::TrackSubsequents;
		}

		void DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
		{
			EAppReturnType::Type UserResponse = FMessageDialog::Open(EAppMsgType::OkCancel, FText::FromString(Name));
		}

	private:
		FString Name;
	};

	void Start()
	{
		FGraphEventRef TaskA, TaskB;
		TaskA = TGraphTask<FTaskA>::CreateTask().ConstructAndDispatchWhenReady("Task_A is running！");

		/*FGraphEventArray Prerequistes;
		Prerequistes.Add(TaskB);*/

		TaskB = TGraphTask<FTaskB>::CreateTask(/*&Prerequistes*/).ConstructAndDispatchWhenReady("Task_B is running！");
	}
};
