// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AuraGameInstance.generated.h"

/**
 * Game Instance classed dedicated to Aura
 * Stores persistent data that should be persist from one level to another
 * i.e. PlayerStart tag, Slot name and index...
 */
UCLASS()
class AURA_API UAuraGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	/** Used when loading to a new level to choose correct player start */
	UPROPERTY()
	FName PlayerStartTag = FName();

	/** Allows to load data from disk */
	UPROPERTY()
	FString LoadSlotName = FString();

	/** Allows to load data from disk */
	UPROPERTY()
	int32 LoadSlotIndex = 0;
};
