// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "MatAbsorbStyle.h"

class FMatAbsorbCommands : public TCommands<FMatAbsorbCommands>
{
public:

	FMatAbsorbCommands()
		: TCommands<FMatAbsorbCommands>(TEXT("MatAbsorb"), NSLOCTEXT("Contexts", "MatAbsorb", "MatAbsorb Plugin"), NAME_None, FMatAbsorbStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
