// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AuraGameModeBase.generated.h"

class ULoadScreenSaveGame;
class USaveGame;
class UMVVM_LoadSlot;
class UAbilityInfo;
class UCharacterClassInfo;
/**
 * Default game mode class in Aura
 * Has Character class data asset
 */
UCLASS()
class AURA_API AAuraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	//~ Begin AGameModeBase Interface
	virtual AActor* ChoosePlayerStart_Implementation( AController* Player ) override;
	//~ End AGameModeBase Interface

	/** Function to save load screen data */
	void SaveSlotData( const UMVVM_LoadSlot* LoadSlot, const int32 SlotIndex ) const;

	/** Saves in game progress */
	void SaveInGameProgressData( ULoadScreenSaveGame* SaveObject ) const;

	/** Saves world state - map with actors */
	void SaveWorldState( UWorld* World );

	/** Loads saved load screen data to display correct slot data */
	ULoadScreenSaveGame* GetSaveSlotData( const FString& SlotName, const int32 SlotIndex ) const;

	/** Retrieves saved game in game */
	ULoadScreenSaveGame* RetrieveInGameSaveData() const;

	/** Deletes saved slot if exists */
	static void DeleteSlotIfExists( const FString& SlotName, const int32 SlotIndex );

	/** Handles traveling to map when loading */
	void TravelToMap( const UMVVM_LoadSlot* Slot );

	/** Data asset used to initialize enemy attributes */
	UPROPERTY( EditDefaultsOnly, Category = "Character Class Defaults" )
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	/** Data asset with all ability infos */
	UPROPERTY( EditDefaultsOnly, Category = "Ability Info" )
	TObjectPtr<UAbilityInfo> AbilityInfo;

	/** Class used to save game */
	UPROPERTY( EditDefaultsOnly )
	TSubclassOf<USaveGame> LoadScreenSaveGameClass;

	/** Name of starting map */
	UPROPERTY( EditDefaultsOnly )
	FString DefaultMapName;

	/** Starting map */
	UPROPERTY( EditDefaultsOnly )
	TSoftObjectPtr<UWorld> DefaultMap;

	/** Map of all playable maps in game */
	UPROPERTY( EditDefaultsOnly )
	TMap<FString, TSoftObjectPtr<UWorld>> Maps;

	/** Default tag to find player start */
	UPROPERTY( EditDefaultsOnly )
	FName DefaultPlayerStartTag;

protected:
	//~ Begin AActor Interface
	virtual void BeginPlay() override;
	//~ End AActor Interface
};
