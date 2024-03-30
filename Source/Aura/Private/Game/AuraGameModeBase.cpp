// Copyright Maxjestic


#include "Game/AuraGameModeBase.h"

#include "Game/LoadScreenSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

bool AAuraGameModeBase::SaveSlotData( const UMVVM_LoadSlot* LoadSlot, const int32 SlotIndex )
{
	// If save exists, remove it
	if (UGameplayStatics::DoesSaveGameExist( LoadSlot->GetLoadSlotName(), SlotIndex ))
	{
		UGameplayStatics::DeleteGameInSlot( LoadSlot->GetLoadSlotName(), SlotIndex );
	}

	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject( LoadScreenSaveGameClass );
	if(ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>( SaveGameObject ))
	{
		LoadScreenSaveGame->PlayerName = LoadSlot->GetPlayerName();
		return UGameplayStatics::SaveGameToSlot(LoadScreenSaveGame, LoadSlot->GetLoadSlotName(), SlotIndex);
	}
	
	return false;
}
