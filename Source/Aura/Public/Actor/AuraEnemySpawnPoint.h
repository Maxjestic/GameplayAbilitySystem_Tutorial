// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Engine/TargetPoint.h"
#include "AuraEnemySpawnPoint.generated.h"

class AAuraEnemy;

/**
 * Used to spawn enemy on given location
 */
UCLASS()
class AURA_API AAuraEnemySpawnPoint : public ATargetPoint
{
	GENERATED_BODY()

public:
	/** Spawns enemy with specified parameters */
	UFUNCTION(BlueprintCallable)
	void SpawnEnemy();
	
	/** Class of enemy to spawn */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyClass")
	TSubclassOf<AAuraEnemy> EnemyClass;
	
	/** Level of the spawned enemy */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyClass")
	int32 EnemyLevel = 1;

	/** Class of the spawned enemy */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyClass")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;
};
