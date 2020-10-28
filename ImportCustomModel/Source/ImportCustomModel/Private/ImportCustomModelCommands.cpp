// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ImportCustomModelCommands.h"

#define LOCTEXT_NAMESPACE "FImportCustomModelModule"

void FImportCustomModelCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "ImportCustomModel", "Execute ImportCustomModel action", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(ImportAction, "ImportModel", "Import the Model's Data!", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
