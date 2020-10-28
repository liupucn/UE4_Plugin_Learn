// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "DesktopPackageCommands.h"

#define LOCTEXT_NAMESPACE "FDesktopPackageModule"

void FDesktopPackageCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "DesktopPackage", "Execute DesktopPackage action", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
