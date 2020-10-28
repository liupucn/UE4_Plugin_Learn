#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"

DECLARE_DELEGATE(FOnProcessCompleted)
DECLARE_DELEGATE_TwoParams(FOnProcessProgress, uint32, uint32)

class FMyRunnable : public FRunnable
{
public:
	FMyRunnable();

	~FMyRunnable();

public:
	/*Interface*/
	virtual uint32 Run() override;

public:
	/*»Øµ÷º¯Êý*/
	FOnProcessCompleted& OnCompletedDelegate()
	{
		return CompletedDelegate;
	}

	FOnProcessProgress& OnProgressDelegate()
	{
		return ProgressDelegate;
	}
private:

	void Task();

private:
	FRunnableThread* Thread;
	FOnProcessCompleted CompletedDelegate;
	FOnProcessProgress ProgressDelegate;
};
