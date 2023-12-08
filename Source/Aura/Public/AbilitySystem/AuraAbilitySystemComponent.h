// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

/** Declaration of delegate broadcasting  */
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /*AssetTags*/)

/**
 * A class used as base Ability System Component in Aura
 */
UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	/** Called when actor info is set */
	void AbilityActorInfoSet();

	/** Grants given abilities */
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);

	/** Activating ability on input released */
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	/** Activating ability on input held */
	void AbilityInputTagHeld(const FGameplayTag& InputTag);

	/** Delegate, sending out asset tags */
	FEffectAssetTags EffectAssetTags;
	
protected:
	/** Callback bound to OnGameplayEffectAppliedDelegateToSelf event */
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied( UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec,
	                    FActiveGameplayEffectHandle ActiveEffectHandle );
};
