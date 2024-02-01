// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"

/**
 * Stores data related to level and level up
 */
USTRUCT( BlueprintType )
struct FAuraLevelUpInfo
{
	GENERATED_BODY()

	/** Experience required to reach next level */
	UPROPERTY( EditDefaultsOnly )
	int32 LevelUpRequirement = 0;

	/** Attribute points received after reaching this level */
	UPROPERTY( EditDefaultsOnly )
	int32 AttributePointReward = 1;

	/** Ability points received after reaching this level */
	UPROPERTY( EditDefaultsOnly )
	int32 SpellPointReward = 1;
};

/**
 * Storing FAuraLevelUpInfo used to track player progress
 */
UCLASS()
class AURA_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	/** Array Level Up Infos */
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "LevelInformation" )
	TArray<FAuraLevelUpInfo> LevelUpInformation;

	/** Returns level for given amount of experience points, 0 if wrong amount of XP is given */
	UFUNCTION()
	int32 FindLevelForExperience( int32 XP ) const;
};
