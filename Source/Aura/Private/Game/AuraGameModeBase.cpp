// Copyright Maxjestic


#include "Game/AuraGameModeBase.h"

#include "Game/LoadScreenSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

bool AAuraGameModeBase::SaveSlotData( const UMVVM_LoadSlot* LoadSlot, const int32 SlotIndex ) const
{
	// If save exists, remove it
	DeleteSlotIfExists(LoadSlot->GetLoadSlotName(), SlotIndex);

	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject( LoadScreenSaveGameClass );
	if (ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>( SaveGameObject ))
	{
		LoadScreenSaveGame->PlayerName = LoadSlot->GetPlayerName();
		LoadScreenSaveGame->SaveSlotStatus = Taken;
		LoadScreenSaveGame->LevelName = LoadSlot->GetLevelName();
		return UGameplayStatics::SaveGameToSlot( LoadScreenSaveGame, LoadSlot->GetLoadSlotName(), SlotIndex );
	}

	return false;
}

ULoadScreenSaveGame* AAuraGameModeBase::GetSaveSlotData( const FString& SlotName, const int32 SlotIndex ) const
{
	USaveGame* SaveGameObject;
	if (UGameplayStatics::DoesSaveGameExist( SlotName, SlotIndex ))
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot( SlotName, SlotIndex );
	}
	else
	{
		SaveGameObject = UGameplayStatics::CreateSaveGameObject( LoadScreenSaveGameClass );
	}
	ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>( SaveGameObject );
	return LoadScreenSaveGame;
}

void AAuraGameModeBase::DeleteSlotIfExists( const FString& SlotName, const int32 SlotIndex )
{
	if (UGameplayStatics::DoesSaveGameExist( SlotName, SlotIndex ))
	{
		UGameplayStatics::DeleteGameInSlot( SlotName, SlotIndex );
	}
}

void AAuraGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	Levels.Add( DefaultLevelName, DefaultLevel );
}
