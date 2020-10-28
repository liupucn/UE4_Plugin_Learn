// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "ImportCustomModelStyle.h"

class FImportCustomModelCommands : public TCommands<FImportCustomModelCommands>
{
public:

	FImportCustomModelCommands()
		: TCommands<FImportCustomModelCommands>(TEXT("ImportCustomModel"), NSLOCTEXT("Contexts", "ImportCustomModel", "ImportCustomModel Plugin"), NAME_None, FImportCustomModelStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
	TSharedPtr<FUICommandInfo>ImportAction;
};
