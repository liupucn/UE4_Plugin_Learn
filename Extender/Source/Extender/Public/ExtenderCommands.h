// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "ExtenderStyle.h"

class FExtenderCommands : public TCommands<FExtenderCommands>
{
public:

	FExtenderCommands()
		: TCommands<FExtenderCommands>(TEXT("Extender"), NSLOCTEXT("Contexts", "Extender", "Extender Plugin"), NAME_None, FExtenderStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
