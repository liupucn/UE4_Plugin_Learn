// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "DesktopPackageStyle.h"

class FDesktopPackageCommands : public TCommands<FDesktopPackageCommands>
{
public:

	FDesktopPackageCommands()
		: TCommands<FDesktopPackageCommands>(TEXT("DesktopPackage"), NSLOCTEXT("Contexts", "DesktopPackage", "DesktopPackage Plugin"), NAME_None, FDesktopPackageStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
