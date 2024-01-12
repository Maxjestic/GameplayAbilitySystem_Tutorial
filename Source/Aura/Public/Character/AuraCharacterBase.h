// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class UAttributeSet;
class UAnimMontage;

/**
 * Base class for all characters in game
 * Contains common properties and functionalities to all characters
 */

UCLASS( Abstract )
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	/**
	 * Sets weapon and mesh collisions
	 */
	AAuraCharacterBase();

	//~ Begin IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ End IAbilitySystemInterface


	//~ Begin ICombatInterface
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual void Die() override;
	//~ End ICombat Interface

	/** Returns attribute set */
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	/** Handles what happens on all clients whenever a character dies */
	UFUNCTION( NetMulticast, Reliable )
	virtual void MulticastHandleDeath();

protected:
	/** A Function that initializes Ability Actor Info for AbilitySystemComponent */
	virtual void InitAbilityActorInfo();

	/** Applies given effect to self */
	void ApplyEffectToSelf( const TSubclassOf<UGameplayEffect> GameplayEffectClass, const float Level = 1.f ) const;

	/** Initializes all default attributes using gameplay effects */
	virtual void InitializeDefaultAttributes() const;

	/** Granting startup abilities to character, calls a function on ability system component */
	void AddCharacterAbilities() const;

	//~ Begin AActor Interface
	virtual void BeginPlay() override;
	//~ End AActor Interface

	//~ Begin ICombatInterface
	virtual FVector GetCombatSocketLocation_Implementation() override;
	//~ End ICombat Interface

	/** The skeletal mesh associated with this character's weapon */
	UPROPERTY( EditAnywhere, Category = "Combat" )
	TObjectPtr<USkeletalMeshComponent> Weapon;

	/** FName of the weapon socket from which projectiles are launched */
	UPROPERTY( EditAnywhere, Category = "Combat" )
	FName WeaponTipSocketName;

	/**
	 * AbilitySystemComponent owned by Character
	 * Exist on PlayerState for player characters
	 * Exist directly on pawn for enemy (AI) characters
	 */
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	/**
	 * AttributeSet owned by Character
	 * Exist on PlayerState for player characters
	 * Exist directly on pawn for enemy (AI) characters
	 */
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	/**
	 * Dissolve effects
	 */

	/** Called on death to start dissolving */
	void Dissolve();

	/** Blueprint event used to create timeline */
	UFUNCTION( BlueprintImplementableEvent )
	void MeshStartDissolveTimeline( UMaterialInstanceDynamic* DynamicMaterialInstance );

	UFUNCTION( BlueprintImplementableEvent )
	void WeaponStartDissolveTimeline( UMaterialInstanceDynamic* DynamicMaterialInstance );

	/** Material used to create dynamic material with dissolve effect for mesh */
	UPROPERTY( EditAnywhere, BlueprintReadOnly )
	TObjectPtr<UMaterialInstance> MeshDissolveMaterialInstance;

	/** Material used to create dynamic material with dissolve effect for weapon */
	UPROPERTY( EditAnywhere, BlueprintReadOnly )
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

private:
	/** Set of ability classes granted to the character at the beginning of the game */
	UPROPERTY( EditAnywhere, Category = "Abilities" )
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	/** Animation montage played when character is reacting to hit, may be nullptr */
	UPROPERTY( EditAnywhere, Category = "Combat" )
	TObjectPtr<UAnimMontage> HitReactMontage;
};
