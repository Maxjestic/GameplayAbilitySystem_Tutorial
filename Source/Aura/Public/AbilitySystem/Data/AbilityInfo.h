// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AbilityInfo.generated.h"

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

	/** Icon of the ability */
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	TObjectPtr<const UTexture2D> Icon = nullptr;

	/** Background of the ability */
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	TObjectPtr<const UMaterialInterface> BackgroundMaterial = nullptr;
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

	FAuraAbilityInfo FindAbilityForTag( const FGameplayTag& AbilityTag, bool bLogNotFound = false ) const;
};
