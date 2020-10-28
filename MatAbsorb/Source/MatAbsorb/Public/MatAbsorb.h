// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "UnrealClient.h"
#include "Widgets/SCompoundWidget.h"

class FToolBarBuilder;
class FMenuBuilder;
class UMaterialInterface;

class FMatAbsorbModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
private:

	bool MyTick(float DeltaTime);
	void MKeyEvent(const FKeyEvent& KeyEvent);

private:

	FDelegateHandle TickerHandle;
	UMaterialInterface* AbsorbMaterialI;
	FString SelectActorName;
	bool IsHasMat;
};