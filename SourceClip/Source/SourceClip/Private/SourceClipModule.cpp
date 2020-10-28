// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SourceClipModule.h"

#include "CoreDelegates.h"

#define LOCTEXT_NAMESPACE "FSourceClipModule"

void FSourceClipModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	UE_LOG(LogTemp, Warning, TEXT("Create Source Clipping Module."));

	m_SourceClip = MakeShareable(new FSourceClip);

	m_SourceClip->AddDelegate();

}

void FSourceClipModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSourceClipModule, SourceClip)