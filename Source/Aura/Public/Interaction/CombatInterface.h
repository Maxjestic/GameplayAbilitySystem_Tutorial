// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

class UNiagaraSystem;
class UAnimMontage;


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
	virtual int32 GetCharacterLevel() const;

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
	virtual void Die() = 0;

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
	FTaggedMontage GetTaggedMontageByTag(const FGameplayTag& MontageTag);

	/***/
	UFUNCTION( BlueprintCallable, BlueprintNativeEvent )
	int32 GetMinionCount();
};
