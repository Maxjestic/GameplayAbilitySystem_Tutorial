// Copyright Maxjestic


#include "Game/AuraGameModeBase.h"

#include "Game/AuraGameInstance.h"
#include "Game/LoadScreenSaveGame.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

AActor* AAuraGameModeBase::ChoosePlayerStart_Implementation( AController* Player )
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass( GetWorld(), APlayerStart::StaticClass(), Actors );
	if (Actors.Num() > 0)
	{
		const UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>( GetGameInstance() );
		for (AActor* Actor : Actors)
		{
			if (APlayerStart* PlayerStart = Cast<APlayerStart>( Actor ))
			{
				if (PlayerStart->PlayerStartTag == AuraGameInstance->PlayerStartTag)
				{
					return PlayerStart;
				}
			}
		}
		return Actors[0];
	}
	return nullptr;
}

void AAuraGameModeBase::SaveSlotData( const UMVVM_LoadSlot* LoadSlot, const int32 SlotIndex ) const
{
	// If save exists, remove it
	DeleteSlotIfExists( LoadSlot->GetLoadSlotName(), SlotIndex );

	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject( LoadScreenSaveGameClass );
	ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>( SaveGameObject );

	LoadScreenSaveGame->PlayerName = LoadSlot->GetPlayerName();
	LoadScreenSaveGame->SaveSlotStatus = Taken;
	LoadScreenSaveGame->MapName = LoadSlot->GetMapName();
	LoadScreenSaveGame->PlayerStartTag = LoadSlot->PlayerStartTag;
	
	UGameplayStatics::SaveGameToSlot( LoadScreenSaveGame, LoadSlot->GetLoadSlotName(), SlotIndex );
}

void AAuraGameModeBase::SaveInGameProgressData( ULoadScreenSaveGame* SaveObject ) const
{
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>( GetGameInstance() );
	const FString InGameLoadSlotName = AuraGameInstance->LoadSlotName;
	const int32 InGameLoadSlotIndex = AuraGameInstance->LoadSlotIndex;
	AuraGameInstance->PlayerStartTag = SaveObject->PlayerStartTag;

	UGameplayStatics::SaveGameToSlot(SaveObject, InGameLoadSlotName, InGameLoadSlotIndex);
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

ULoadScreenSaveGame* AAuraGameModeBase::RetrieveInGameSaveData() const
{
	const UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>( GetGameInstance() );
	const FString InGameLoadSlotName = AuraGameInstance->LoadSlotName;
	const int32 InGameLoadSlotIndex = AuraGameInstance->LoadSlotIndex;

	return GetSaveSlotData( InGameLoadSlotName, InGameLoadSlotIndex );
}

void AAuraGameModeBase::DeleteSlotIfExists( const FString& SlotName, const int32 SlotIndex )
{
	if (UGameplayStatics::DoesSaveGameExist( SlotName, SlotIndex ))
	{
		UGameplayStatics::DeleteGameInSlot( SlotName, SlotIndex );
	}
}

void AAuraGameModeBase::TravelToMap( const UMVVM_LoadSlot* Slot )
{
	const FString SlotName = Slot->GetLoadSlotName();
	const int32 SlotIndex = Slot->SlotIndex;

	UGameplayStatics::OpenLevelBySoftObjectPtr( Slot, Maps.FindChecked( Slot->GetMapName() ) );
}

void AAuraGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	Maps.Add( DefaultMapName, DefaultMap );
}
