// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UDebuffNiagaraComponent;
class UNiagaraSystem;
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
	 * Sets weapon and mesh collisions, Debuff Niagara Components
	 */
	AAuraCharacterBase();

	//~ Begin UObject Interface
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override;
	//~ End UObject Interface

	//~ Begin IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ End IAbilitySystemInterface


	//~ Begin ICombatInterface
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual void Die( const FVector& DeathImpulse ) override;
	virtual FOnDeathSignature& GetOnDeathDelegate() override;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override;
	virtual UNiagaraSystem* GetBloodEffect_Implementation() override;
	virtual FTaggedMontage GetTaggedMontageByTag_Implementation( const FGameplayTag& MontageTag ) override;
	virtual int32 GetMinionCount_Implementation() override;
	virtual void UpdateMinionCount_Implementation( const int32 Amount ) override;
	virtual ECharacterClass GetCharacterClass_Implementation() override;
	virtual FOnAbilitySystemComponentRegisteredSignature& GetOnAbilitySystemComponentDelegate() override;
	virtual USkeletalMeshComponent* GetWeapon_Implementation() override;
	virtual bool IsBeingShocked_Implementation() const override;
	virtual void SetIsBeingShocked_Implementation(bool bInIsBeingShocked) override;
	//~ End ICombat Interface

	/** Broadcasts ability system component as soon as it is valid */
	FOnAbilitySystemComponentRegisteredSignature OnAbilitySystemComponentRegistered;

	/** Broadcasts self (Actor) on my death */
	FOnDeathSignature OnDeath;

	/** Returns my attribute set */
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	/** Handles what happens on all clients whenever I die */
	UFUNCTION( NetMulticast, Reliable )
	virtual void MulticastHandleDeath( const FVector& DeathImpulse );

	/** Montages used to perform an attack */
	UPROPERTY( EditAnywhere, Category = "Combat" )
	TArray<FTaggedMontage> AttackMontages;

	/** True if I'm stunned */
	UPROPERTY( ReplicatedUsing = OnRep_Stunned, BlueprintReadOnly )
	bool bIsStunned = false;

	/** True if I'm burned */
	UPROPERTY( ReplicatedUsing = OnRep_Burned, BlueprintReadOnly )
	bool bIsBurned = false;

	/** True if I'm in shock loop */
	UPROPERTY( Replicated, BlueprintReadOnly )
	bool bIsBeingShocked = false;

	/**
	 * Rep notifies
	 */
	UFUNCTION()
	virtual void OnRep_Burned();

	UFUNCTION()
	virtual void OnRep_Stunned();

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
	virtual FVector GetCombatSocketLocation_Implementation( const FGameplayTag& MontageTag ) override;
	//~ End ICombat Interface

	/** Callback function when stun tag changes */
	virtual void StunTagChanged( const FGameplayTag CallbackTag, int32 NewCount );

	/** The skeletal mesh associated with this character's weapon */
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Combat" )
	TObjectPtr<USkeletalMeshComponent> Weapon;

	/** Socket of the weapon socket */
	UPROPERTY( EditAnywhere, Category = "Combat" )
	FName WeaponTipSocketName;

	/** Socket of the right hand */
	UPROPERTY( EditAnywhere, Category = "Combat" )
	FName RightHandSocketName;

	/** Socket of the left hand */
	UPROPERTY( EditAnywhere, Category = "Combat" )
	FName LeftHandSocketName;

	/** Socket of the tail */
	UPROPERTY( EditAnywhere, Category = "Combat" )
	FName TailSocketName;

	/** Enemy Character class */
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults" )
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

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

	/** Blood effect in response to getting damaged */
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Combat" )
	TObjectPtr<UNiagaraSystem> BloodEffect;

	/** Sound played when character dies */
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Combat" )
	TObjectPtr<USoundBase> DeathSound;

	/** True if this is dead */
	bool bDead = false;

	/** Base walk speed value */
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Combat" )
	float BaseWalkSpeed = 600.f;

	/**
	 * Minions
	 */

	/** How many minions does this have */
	int32 MinionCount = 0;

	/** Niagara component activated when I get burn debuff */
	UPROPERTY( VisibleAnywhere )
	TObjectPtr<UDebuffNiagaraComponent> BurnDebuffComponent;

	/** Niagara component activated when I get stun debuff */
	UPROPERTY( VisibleAnywhere )
	TObjectPtr<UDebuffNiagaraComponent> StunDebuffComponent;

private:
	/** Set of active ability classes granted to the character at the beginning of the game */
	UPROPERTY( EditAnywhere, Category = "Abilities" )
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	/** Set of passive ability classes granted to the character at the beginning of the game */
	UPROPERTY( EditAnywhere, Category = "Abilities" )
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;

	/** Animation montage played when character is reacting to hit, may be nullptr */
	UPROPERTY( EditAnywhere, Category = "Combat" )
	TObjectPtr<UAnimMontage> HitReactMontage;
};
