// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LootTiers.generated.h"

USTRUCT( BlueprintType )
struct FLootItem
{
	GENERATED_BODY()

	/** Class to spawn */
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "LootTiers|Spawning" )
	TSubclassOf<AActor> LootClass;

	/** Chance to spawn */
	UPROPERTY( EditAnywhere, Category = "LootTiers|Spawning" )
	float ChanceToSpawn = 0.f;

	/** Max number "dice rolls" for given item */
	UPROPERTY( EditAnywhere, Category = "LootTiers|Spawning" )
	int32 MaxNumberToSpawn = 0.f;

	/** Defines if loot level should be overridden */
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "LootTiers|Spawning" )
	bool bLootLevelOverride = true;
};

/**
 * 
 */
UCLASS()
class AURA_API ULootTiers : public UDataAsset
{
	GENERATED_BODY()

public:
	/** Calculates and returns loot items */
	UFUNCTION( BlueprintCallable )
	TArray<FLootItem> GetLootItems();

	/** Main data */
	UPROPERTY( EditDefaultsOnly, Category = "LootTiers|Spawning" )
	TArray<FLootItem> LootItems;
};
