// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ExtenderCommands.h"

#define LOCTEXT_NAMESPACE "FExtenderModule"

void FExtenderCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "Extender", "Execute Extender action", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
