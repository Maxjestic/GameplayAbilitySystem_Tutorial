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
	/** Function to save load screen data, returns true if successful */
	bool SaveSlotData( const UMVVM_LoadSlot* LoadSlot, const int32 SlotIndex ) const;

	/** Function to load saved load screen data to display correct slot data */
	ULoadScreenSaveGame* GetSaveSlotData( const FString& SlotName, const int32 SlotIndex ) const;

	/** Deletes saved slot if exists */
	static void DeleteSlotIfExists( const FString& SlotName, const int32 SlotIndex );

	/** Handles traveling to map when loading */
	void TravelToMap(const UMVVM_LoadSlot* Slot);

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

protected:
	//~ Begin AActor Interface
	virtual void BeginPlay() override;
	//~ End AActor Interface
};
