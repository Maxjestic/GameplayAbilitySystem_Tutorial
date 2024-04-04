// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LoadScreenSaveGame.generated.h"

UENUM( BlueprintType )
enum ESaveSlotStatus
{
	Vacant,
	EnterName,
	Taken
};

/**
 * SaveGame object specified for Aura
 */
UCLASS()
class AURA_API ULoadScreenSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	/** Slot name associated with save */
	UPROPERTY()
	FString SlotName = FString();

	/** Slot index associated with save */
	UPROPERTY()
	int32 SlotIndex = 0;

	/** Saved slot status */
	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SaveSlotStatus = Vacant;

	/** Saved player name */
	UPROPERTY()
	FString PlayerName = FString( "Default Name" );

	/** Saved map name */
	UPROPERTY()
	FString MapName = FString( "Default Map Name" );

	/** Saved tag for player start */
	UPROPERTY()
	FName PlayerStartTag;

	/** Used to determine if default attributes and abilities should be loaded in */
	UPROPERTY()
	bool bFirstTimeLoadIn = true;

	/**
	 * Player related
	 */

	/** Saved player level*/
	UPROPERTY()
	int32 PlayerLevel = 0;

	/** Saved experience points*/
	UPROPERTY()
	int32 ExperiencePoints = 0;

	/** Saved left spell points*/
	UPROPERTY()
	int32 SpellPoints = 0;

	/** Saved left attribute points */
	UPROPERTY()
	int32 AttributePoints = 0;

	/** Saved strength attribute value */
	UPROPERTY()
	float Strength = 0;

	/** Saved intelligence attribute value */
	UPROPERTY()
	float Intelligence = 0;

	/** Saved resilience attribute value */
	UPROPERTY()
	float Resilience = 0;

	/** Saved vigor attribute value */
	UPROPERTY()
	float Vigor = 0;
};
