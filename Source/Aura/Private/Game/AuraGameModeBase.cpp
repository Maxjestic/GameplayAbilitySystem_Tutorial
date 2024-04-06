// Copyright Maxjestic


#include "Game/AuraGameModeBase.h"

#include "EngineUtils.h"
#include "Game/AuraGameInstance.h"
#include "Game/LoadScreenSaveGame.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/SaveInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
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

	UGameplayStatics::SaveGameToSlot( SaveObject, InGameLoadSlotName, InGameLoadSlotIndex );
}

void AAuraGameModeBase::SaveWorldState( UWorld* World )
{
	FString WorldName = World->GetMapName();
	WorldName.RemoveFromStart( World->StreamingLevelsPrefix );

	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>( GetGameInstance() );
	check( AuraGameInstance );

	if (ULoadScreenSaveGame* SaveGame = GetSaveSlotData( AuraGameInstance->LoadSlotName, AuraGameInstance->LoadSlotIndex ))
	{
		FSavedMap NewSavedMap;
		if (!SaveGame->GetSavedMapWithMapName( WorldName, NewSavedMap ))
		{
			NewSavedMap.MapAssetName = WorldName;
			SaveGame->SavedMaps.Add( NewSavedMap );
		}

		// clear before refilling with "actors"
		NewSavedMap.SavedActors.Empty();

		for (FActorIterator ActorIterator( World ); ActorIterator; ++ActorIterator)
		{
			AActor* Actor = *ActorIterator;

			if (!IsValid( Actor ) || !Actor->Implements<USaveInterface>())
			{
				continue;
			}

			FSavedActor SavedActor;
			SavedActor.ActorName = Actor->GetFName();
			SavedActor.Transform = Actor->GetTransform();

			FMemoryWriter MemoryWriter( SavedActor.Bytes );

			FObjectAndNameAsStringProxyArchive Archive( MemoryWriter, true );
			Archive.ArIsSaveGame = true;

			Actor->Serialize( Archive );

			NewSavedMap.SavedActors.AddUnique( SavedActor );
		}

		for (FSavedMap& MapToReplace : SaveGame->SavedMaps)
		{
			if (MapToReplace.MapAssetName == WorldName)
			{
				MapToReplace = NewSavedMap;
				break;
			}
		}
		UGameplayStatics::SaveGameToSlot( SaveGame, AuraGameInstance->LoadSlotName, AuraGameInstance->LoadSlotIndex );
	}
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
