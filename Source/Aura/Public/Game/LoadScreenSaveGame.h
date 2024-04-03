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
 * saved data from the load screen
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

	/** Saved player name */
	UPROPERTY()
	FString PlayerName = FString( "Default Name" );

	/** Saved map name */
	UPROPERTY()
	FString MapName = FString( "Default Map Name" );

	/** Saved slot status */
	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SaveSlotStatus = Vacant;

	/** Saved tag for player start */
	UPROPERTY()
	FName PlayerStartTag;
};
