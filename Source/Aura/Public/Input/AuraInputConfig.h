// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AuraInputConfig.generated.h"

class UInputAction;

/**
 * Stores Input Action with associated Gameplay Tag
 */
USTRUCT(BlueprintType)
struct FAuraInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	const UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();
};

/**
 * Input configuration data asset
 * Associates Input Actions with Gameplay Tags
 */
UCLASS()
class AURA_API UAuraInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	/** looks for input action associated with given gameplay tag, may return nullptr if not found (can be logged) */
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, const bool bLogNotFound = false) const;

	/** Pairs of associated input action and gameplay tag */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAuraInputAction> AbilityInputActions;
};
