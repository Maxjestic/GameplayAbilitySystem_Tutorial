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

/**
 * Base class for all characters in game
 * Contains common properties and functionalities to all characters
 */

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	/**
	 * Sets weapon
	 */
	AAuraCharacterBase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

protected:
	/** A Function that initializes Ability Actor Info for AbilitySystemComponent */
	virtual void InitAbilityActorInfo();

	/** Applies given effect to self */
	void ApplyEffectToSelf( const TSubclassOf<UGameplayEffect> GameplayEffectClass, const float Level = 1.f ) const;

	/** Initializes all default attributes using gameplay effects */
	void InitializeDefaultAttributes() const;

	/** Granting startup abilities to character, calls a function on ability system component */
	void AddCharacterAbilities() const;

	//~ Begin AActor Interface
	virtual void BeginPlay() override;
	//~ End AActor Interface

	//~ Begin ICombatInterface
	virtual FVector GetCombatSocketLocation() override;
	//~ End ICombat Interface

	/** The skeletal mesh associated with this character's weapon */
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	/** FName of the weapon socket from which projectiles are launched */
	UPROPERTY(EditAnywhere, Category = "Combat")
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

	/** Gameplay effect initializing primary attributes for the character */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	/** Gameplay effect initializing secondary attributes for the character */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	/** Gameplay effect initializing vital attributes for the character */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

private:
	/** Set of ability classes granted to the character at the beginning of the game */
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
};
