// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AbilityInfo.generated.h"

class UGameplayAbility;
/**
 * Stores data associated with ability and shown in overlay
 */
USTRUCT( BlueprintType )
struct FAuraAbilityInfo
{
	GENERATED_BODY()

	/** Gameplay tag associated with the ability */
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	FGameplayTag AbilityTag = FGameplayTag();

	/** Gameplay tag associated with the input of the ability */
	UPROPERTY( BlueprintReadOnly )
	FGameplayTag InputTag = FGameplayTag();

	/** Gameplay tag representing status of the ability */
	UPROPERTY( BlueprintReadOnly )
	FGameplayTag StatusTag = FGameplayTag();

	/** Gameplay tag associated with the cooldown of the ability */
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	FGameplayTag CooldownTag = FGameplayTag();

	/** Gameplay tag associated with the type of the ability */
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	FGameplayTag AbilityTypeTag = FGameplayTag();

	/** Icon of the ability */
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	TObjectPtr<const UTexture2D> Icon = nullptr;

	/** Background of the ability */
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	TObjectPtr<const UMaterialInterface> BackgroundMaterial = nullptr;

	/** Level required to make ability eligible */
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	int32 LevelRequirement = 1;

	/** Class of this ability */
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	TSubclassOf<UGameplayAbility> AbilityClass;
};

/**
 * Data asset storing FAuraAbilityInfo used in overlay
 */
UCLASS()
class AURA_API UAbilityInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	/** Array of Ability Infos */
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityInformation" )
	TArray<FAuraAbilityInfo> AbilityInformation;

	/** Returns Ability for given tag, may be empty */
	FAuraAbilityInfo FindAbilityInfoForTag( const FGameplayTag& AbilityTag, bool bLogNotFound = false ) const;
};
