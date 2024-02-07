// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"


/** Declaration of delegate broadcasting  */
DECLARE_MULTICAST_DELEGATE_OneParam( FEffectAssetTags, const FGameplayTagContainer& /*AssetTags*/ )

/** Broadcast to inform that abilities have been given */
DECLARE_MULTICAST_DELEGATE( FAbilitiesGiven );

/** Delegate created in OverlayWidgetController and executed on every activable abilities and broadcasting AbilityInfo to widgets  */
DECLARE_DELEGATE_OneParam( FForEachAbility, const FGameplayAbilitySpec& /*AbilitySpec*/ );

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
	void AddCharacterAbilities( const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities );

	/** Grants given abilities */
	void AddCharacterPassiveAbilities( const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities );

	/** Activating ability on input released */
	void AbilityInputTagReleased( const FGameplayTag& InputTag );

	/** Activating ability on input held */
	void AbilityInputTagHeld( const FGameplayTag& InputTag );

	/** Broadcasts asset tags */
	FEffectAssetTags EffectAssetTagsDelegate;

	/** Broadcasts when abilities have been given */
	FAbilitiesGiven AbilitiesGivenDelegate;

	/** True if startup abilities have been given */
	bool bStartupAbilitiesGiven = false;

	/** Loops through all activatable abilities and calls delegate */
	void ForEachAbility( const FForEachAbility& Delegate );

	/** Returns ability tag from given ability spec, empty if not found*/
	static FGameplayTag GetAbilityTagFromSpec( const FGameplayAbilitySpec& AbilitySpec );

	/** Returns input tag from given ability spec, empty if not found*/
	static FGameplayTag GetInputTagFromSpec( const FGameplayAbilitySpec& AbilitySpec );

	/** Client side, increase attribute associated with given tag */
	void UpgradeAttribute( const FGameplayTag& AttributeTag );

	/** Server side, increase attribute associated with given tag */
	UFUNCTION(Server, Reliable)
	void ServerUpgradeAttribute( const FGameplayTag& AttributeTag );

protected:
	//~ Begin UAbilitySystemComponent Interface
	virtual void OnRep_ActivateAbilities() override;
	//~ End UAbilitySystemComponent Interface

	/** Callback bound to OnGameplayEffectAppliedDelegateToSelf event */
	UFUNCTION( Client, Reliable )
	void ClientEffectApplied( UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec,
	                          FActiveGameplayEffectHandle ActiveEffectHandle );
};
