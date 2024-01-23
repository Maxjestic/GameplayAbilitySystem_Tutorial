// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/AuraGameplayAbility.h"
#include "AuraSummonAbility.generated.h"

/**
 * Ability that summons minions
 */
UCLASS()
class AURA_API UAuraSummonAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

public:
	/** Function that returns possible locations to spawn minions */
	UFUNCTION( BlueprintCallable )
	TArray<FVector> GetSpawnLocations();

	/** Returns random minions class from set minion classes */
	UFUNCTION(BlueprintPure, Category = "Summoning" )
	TSubclassOf<APawn> GetRandomMinionClass();

	/** Number of minions that can be spawned via this ability */
	UPROPERTY( EditDefaultsOnly, Category = "Summoning" )
	int32 NumMinionsToSpawn = 5;

	/** Classes of minions that can be spawned via this ability */
	UPROPERTY( EditDefaultsOnly, Category = "Summoning" )
	TArray<TSubclassOf<APawn>> MinionClasses;

	/** Minimum distance in which minions can be spawned */
	UPROPERTY( EditDefaultsOnly, Category = "Summoning" )
	float MinSpawnDistance = 50.f;

	/** Maximum distance in which minions can be spawned */
	UPROPERTY( EditDefaultsOnly, Category = "Summoning" )
	float MaxSpawnDistance = 250.f;

	/** Angle in which minions will be summoned */
	UPROPERTY( EditDefaultsOnly, Category = "Summoning" )
	float SpawnSpreadAngle = 90.f;
};
