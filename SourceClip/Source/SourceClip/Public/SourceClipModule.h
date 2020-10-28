// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#include "SourceClip.h"

class FSourceClipModule : public IModuleInterface
{
public:
	static constexpr auto ModuleName = TEXT("SourceClip");

	static inline FSourceClipModule& Get()
	{
		return FModuleManager::LoadModuleChecked< FSourceClipModule >(FSourceClipModule::ModuleName);
	}

	class FSourceClip* GetSourceClip(void)
	{
		return m_SourceClip.Get();
	}

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	TSharedPtr<class FSourceClip> m_SourceClip;
	

};
