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

/** Broadcasts Ability Tag and Status Tag on status change */
DECLARE_MULTICAST_DELEGATE_ThreeParams( FAbilityStatusChanged,
                                        const FGameplayTag& /*AbilityTag*/,
                                        const FGameplayTag& /*StatusTag*/,
                                        const int32 /*AbilityLevel*/ )

/** Broadcasts tags associated with equipping ability */
DECLARE_MULTICAST_DELEGATE_FourParams( FAbilityEquipped,
                                       const FGameplayTag& /*AbilityTag*/,
                                       const FGameplayTag& /*StatusTag*/,
                                       const FGameplayTag& /*Slot*/,
                                       const FGameplayTag& /*PreviousSlot*/ )

/** Broadcasts tag associated with passive ability that should be deactivated */
DECLARE_MULTICAST_DELEGATE_OneParam( FDeactivatePassiveAbility, const FGameplayTag& /*AbilityTag*/ )

/** Broadcasts ability tag related to ability that should or shouldn't be activated based on bActivate */
DECLARE_MULTICAST_DELEGATE_TwoParams( FActivatePassiveEffect, const FGameplayTag& /*AbilityTag*/, bool /*bActivate*/ );

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

	/** True if startup abilities have been given */
	bool bStartupAbilitiesGiven = false;

	/** Grants given abilities */
	void AddCharacterPassiveAbilities( const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities );

	/** Activating ability on input pressed */
	void AbilityInputTagPressed( const FGameplayTag& InputTag );

	/** Activating ability on input released */
	void AbilityInputTagReleased( const FGameplayTag& InputTag );

	/** Activating ability on input held */
	void AbilityInputTagHeld( const FGameplayTag& InputTag );


	/** Broadcasts asset tags */
	FEffectAssetTags EffectAssetTagsDelegate;

	/** Broadcasted when abilities have been given */
	FAbilitiesGiven AbilitiesGivenDelegate;

	/** Broadcasted when ability status changes */
	FAbilityStatusChanged AbilityStatusChanged;

	/** Broadcasted when ability is being equipped */
	FAbilityEquipped AbilityEquipped;

	/** Broadcasted when player deactivates ability in spell menu */
	FDeactivatePassiveAbility DeactivatePassiveAbility;

	/** Broadcasted when player activates/deactivates ability in spell menu to activate related niagara component */
	FActivatePassiveEffect ActivatePassiveEffect;

	/** Loops through all activatable abilities and calls delegate */
	void ForEachAbility( const FForEachAbility& Delegate );

	/** Returns ability tag from given ability spec, empty if not found*/
	static FGameplayTag GetAbilityTagFromSpec( const FGameplayAbilitySpec& AbilitySpec );

	/** Returns input tag from given ability spec, empty if not found*/
	static FGameplayTag GetInputTagFromSpec( const FGameplayAbilitySpec& AbilitySpec );

	/** Returns status tag from given ability spec, empty if not found*/
	static FGameplayTag GetStatusTagFromSpec( const FGameplayAbilitySpec& AbilitySpec );

	/** Returns status tag associated with given ability tag, empty if not found */
	FGameplayTag GetStatusTagFromAbilityTag( const FGameplayTag& AbilityTag );

	/** Returns slot associated with given ability tag, empty if not found */
	FGameplayTag GetSlotFromAbilityTag( const FGameplayTag& AbilityTag );

	/** Returns ability spec from given ability tag, nullptr if not found */
	FGameplayAbilitySpec* GetSpecFromAbilityTag( const FGameplayTag& AbilityTag );

	/** Returns true if given Slot is empty */
	bool SlotIsEmpty( const FGameplayTag& Slot );

	/** Returns true if given Spec has given Slot */
	static bool AbilityHasSlot( const FGameplayAbilitySpec& Spec, const FGameplayTag& Slot );

	/** Returns true if given Spec has any Slot */
	static bool AbilityHasAnySlot( const FGameplayAbilitySpec& Spec );

	/** Returns ability spec associated with given slot, nullptr if not found */
	FGameplayAbilitySpec* GetSpecWithSlot( const FGameplayTag& Slot );

	/** Returns true if given ability is passive */
	bool IsPassiveAbility( const FGameplayAbilitySpec& Spec ) const;

	/** Clears slot and assigns new ability to it */
	static void AssignSlotToAbility( FGameplayAbilitySpec& Spec, const FGameplayTag& Slot );


	/** RPC function to show passive spell niagara component */
	UFUNCTION( NetMulticast, Unreliable )
	void MulticastActivatePassiveEffect( const FGameplayTag& AbilityTag, const bool bActivate );


	/** Client side, increase attribute associated with given tag */
	void UpgradeAttribute( const FGameplayTag& AttributeTag );

	/** Check if player meets level requirement for ability */
	void UpdateAbilityStatuses( const int32 Level );

	/** Sets current and next level ability description from ability tag, false if ability is locked */
	bool GetDescriptionsByAbilityTag( const FGameplayTag& AbilityTag, FString& OutDescription, FString& OutNextLevelDescription );

	/** Removes input tag associated with given ability spec */
	static void ClearSlot( FGameplayAbilitySpec* AbilitySpec );

	/** Clear abilities of a given slot */
	void ClearAbilitiesOfSlot( const FGameplayTag& Slot );

	/** Returns true if given ability contains given input tag */
	static bool AbilityHasSlot( const FGameplayAbilitySpec* AbilitySpec, const FGameplayTag& Slot );

	/** Server side, increase attribute associated with given tag */
	UFUNCTION( Server, Reliable )
	void ServerUpgradeAttribute( const FGameplayTag& AttributeTag );

	/** Called on server to manage spending points on selected ability */
	UFUNCTION( Server, Reliable )
	void ServerSpendSpellPoint( const FGameplayTag& AbilityTag );

	/** Called on server when player attempts to equip ability */
	UFUNCTION( Server, Reliable )
	void ServerEquipAbility( const FGameplayTag& AbilityTag, const FGameplayTag& Slot );

	/** Called from server on client to inform about equipping ability */
	UFUNCTION( Client, Reliable )
	void ClientEquipAbility( const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot,
	                         const FGameplayTag& PreviousSlot );

protected:
	//~ Begin UAbilitySystemComponent Interface
	virtual void OnRep_ActivateAbilities() override;
	//~ End UAbilitySystemComponent Interface

	/** Callback bound to OnGameplayEffectAppliedDelegateToSelf event */
	UFUNCTION( Client, Reliable )
	void ClientEffectApplied( UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec,
	                          FActiveGameplayEffectHandle ActiveEffectHandle );


	/** RPC that broadcasts Ability Tag and Status Tag */
	UFUNCTION( Client, Reliable )
	void ClientUpdateAbilityStatus( const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const int32 AbilityLevel );
};
