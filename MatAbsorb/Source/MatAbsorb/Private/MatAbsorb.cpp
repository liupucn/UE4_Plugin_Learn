// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "MatAbsorb.h"
#include "MatAbsorbStyle.h"
#include "MatAbsorbCommands.h"
#include "Misc/MessageDialog.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

#include "LevelEditor.h"
#include "Engine/Selection.h"
#include "AssetRegistryModule.h"
#include "MessageTranslate.h"

#include "Containers/Ticker.h"
#include "InputCoreModule.h"

static const FName MatAbsorbTabName("MatAbsorb");

#define LOCTEXT_NAMESPACE "FMatAbsorbModule"

void FMatAbsorbModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FMatAbsorbStyle::Initialize();
	FMatAbsorbStyle::ReloadTextures();

	FMatAbsorbCommands::Register();
	
	TickerHandle = FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateRaw(this, &FMatAbsorbModule::MyTick), 0.0f);
	FSlateApplication::Get().OnApplicationPreInputKeyDownListener().AddRaw(this,&FMatAbsorbModule::MKeyEvent);
	AbsorbMaterialI = nullptr;
	IsHasMat = false;
}

void FMatAbsorbModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FMatAbsorbStyle::Shutdown();

	FMatAbsorbCommands::Unregister();
}

void FMatAbsorbModule::MKeyEvent(const FKeyEvent& KeyEvent)
{
	FKey CurrentKey = KeyEvent.GetKey();
	if (CurrentKey == EKeys::M)
	{
		//捕获选中的Actor
		USelection* SelectedActors = GEditor->GetSelectedActors();
		TArray<AActor*> Actors;
		for (FSelectionIterator Iter(*SelectedActors); Iter; ++Iter)
		{
			AActor* Actor = Cast<AActor>(*Iter);
			if (Actor)
			{
				Actors.Add(Actor);
			}
		}

		//获取Mat
		if (Actors.Num() > 0)
		{
			TArray<UStaticMeshComponent*> StaticMeshComponent;
			Actors[0]->GetComponents<UStaticMeshComponent>(StaticMeshComponent);
			AbsorbMaterialI = StaticMeshComponent[0]->GetMaterial(0);
			SelectActorName = Actors[0]->GetName();
		}
	}
}

bool FMatAbsorbModule::MyTick(float DeltaTime)
{
	//捕获选中的Actor
	USelection* SelectedActors = GEditor->GetSelectedActors();
	TArray<AActor*> Actors;
	for (FSelectionIterator Iter(*SelectedActors); Iter; ++Iter)
	{
		AActor* Actor = Cast<AActor>(*Iter);
		if (Actor)
		{
			Actors.Add(Actor);
		}
	}

	if (AbsorbMaterialI && Actors.Num() > 0 && SelectActorName != Actors[0]->GetName())
	{
		TArray<UStaticMeshComponent*> StaticMeshComponent;
		Actors[0]->GetComponents<UStaticMeshComponent>(StaticMeshComponent);
		StaticMeshComponent[0]->SetMaterial(0, AbsorbMaterialI);
		AbsorbMaterialI = nullptr;
		Actors.Empty();
	}
	return true;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMatAbsorbModule, MatAbsorb)