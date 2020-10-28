// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "AsynTestStyle.h"

class FAsynTestCommands : public TCommands<FAsynTestCommands>
{
public:

	FAsynTestCommands()
		: TCommands<FAsynTestCommands>(TEXT("AsynTest"), NSLOCTEXT("Contexts", "AsynTest", "AsynTest Plugin"), NAME_None, FAsynTestStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
