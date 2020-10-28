// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "AsynTestCommands.h"

#define LOCTEXT_NAMESPACE "FAsynTestModule"

void FAsynTestCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "AsynTest", "Execute AsynTest action", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
