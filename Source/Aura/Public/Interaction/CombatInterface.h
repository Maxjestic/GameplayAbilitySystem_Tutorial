// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

class UAbilitySystemComponent;
class UNiagaraSystem;
class UAnimMontage;

/** Delegate used to broadcast ability system component as soon as it is valid */
DECLARE_MULTICAST_DELEGATE_OneParam( FOnAbilitySystemComponentRegisteredSignature, UAbilitySystemComponent* /*AbilitySystemComponent*/ );

/** Delegate used on actor's death */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnDeathSignature, AActor*, DeadActor );

/**
 * Contains montage with corresponding gameplay tag and sound
 */
USTRUCT( BlueprintType )
struct FTaggedMontage
{
	GENERATED_BODY()

	/** Montage to play */
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	UAnimMontage* Montage = nullptr;

	/** Montage Gameplay tag associated with Montage*/
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	FGameplayTag MontageTag;

	/** Combat Socket Gameplay Tag associated with Montage*/
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	FGameplayTag SocketTag;

	/** Sound played on impact */
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	USoundBase* ImpactSound = nullptr;
};

// This class does not need to be modified.
UINTERFACE( MinimalAPI, BlueprintType )
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Combat interaction interface
 */
class AURA_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/** Returns owner level */
	UFUNCTION( BlueprintNativeEvent )
	int32 GetCharacterLevel() const;

	/** Returns a socket from weapon that may be used to for example spawn a projectile */
	UFUNCTION( BlueprintCallable, BlueprintNativeEvent )
	FVector GetCombatSocketLocation( const FGameplayTag& CombatSocketTag );

	/** Blueprint function to set warp target */
	UFUNCTION( BlueprintImplementableEvent, BlueprintCallable )
	void UpdateFacingTarget( const FVector& TargetLocation );

	/** Returns a animation montage that should be played when reacting to hit */
	UFUNCTION( BlueprintCallable, BlueprintNativeEvent )
	UAnimMontage* GetHitReactMontage();

	/** Function called when owner's health reaches 0, called on the server */
	virtual void Die( const FVector& DeathImpulse ) = 0;

	/** Returns FOnDeathSignature delegate */
	virtual FOnDeathSignature& GetOnDeathDelegate() = 0;

	/** Returns true if owner is dead */
	UFUNCTION( BlueprintCallable, BlueprintNativeEvent )
	bool IsDead() const;

	/** Returns avatar */
	UFUNCTION( BlueprintCallable, BlueprintNativeEvent )
	AActor* GetAvatar();

	/** Returns owner's tagged attack montages */
	UFUNCTION( BlueprintCallable, BlueprintNativeEvent )
	TArray<FTaggedMontage> GetAttackMontages();

	/** Returns owner's blood effect */
	UFUNCTION( BlueprintCallable, BlueprintNativeEvent )
	UNiagaraSystem* GetBloodEffect();

	/** Returns Tagged Montage for given Tag */
	UFUNCTION( BlueprintCallable, BlueprintNativeEvent )
	FTaggedMontage GetTaggedMontageByTag( const FGameplayTag& MontageTag );

	/** Returns number of minions that this has */
	UFUNCTION( BlueprintCallable, BlueprintNativeEvent )
	int32 GetMinionCount();

	/** Updates number of minions that this has by given number */
	UFUNCTION( BlueprintCallable, BlueprintNativeEvent )
	void UpdateMinionCount( const int32 Amount );

	/** Returns character class of this */
	UFUNCTION( BlueprintCallable, BlueprintNativeEvent )
	ECharacterClass GetCharacterClass();

	/** Returns FOnAbilitySystemComponentRegisteredSignature delegate */
	virtual FOnAbilitySystemComponentRegisteredSignature& GetOnAbilitySystemComponentDelegate() = 0;

	/** Sets shock loop state */
	UFUNCTION( BlueprintImplementableEvent, BlueprintCallable )
	void SetInShockLoop( bool bInLoop );

	/** Returns weapon skeletal mesh component */
	UFUNCTION( BlueprintCallable, BlueprintNativeEvent )
	USkeletalMeshComponent* GetWeapon();

	/** Returns true if owner is being shocked */
	UFUNCTION( BlueprintCallable, BlueprintNativeEvent )
	bool IsBeingShocked() const;

	/** Sets if owner is in shock loop */
	UFUNCTION( BlueprintCallable, BlueprintNativeEvent )
	void SetIsBeingShocked( bool bInIsBeingShocked );
};
