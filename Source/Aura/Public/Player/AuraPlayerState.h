// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"

class ULevelUpInfo;
class UAttributeSet;

/** Broadcasts single value related to player stats */
DECLARE_MULTICAST_DELEGATE_OneParam( FOnPlayerStatChanged, int32 /*StatValue*/ );

/**
 * Player State class used as default player state in game
 */
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	/**
	 * Sets NetUpdateFrequency, AbilitySystemComponent (+replication), AttributeSet
	 */
	AAuraPlayerState();

	//~ Begin UObject Interface
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override;
	//~ End UObject Interface

	/** AttributeSet getter */
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }


	/** Player level getter */
	FORCEINLINE int32 GetPlayerLevel() const { return Level; }

	/** Sets Level to given value */
	void SetLevel( const int32 InLevel );

	/** Adds given number to current Level */
	void AddToLevel( int32 InLevel );

	/** Broadcast Experience amount on change */
	FOnPlayerStatChanged OnLevelChangedDelegate;


	/** Player Experience getter */
	FORCEINLINE int32 GetPlayerExperience() const { return Experience; }

	/** Sets Experience to given amount */
	void SetExperience( int32 InExperience );

	/** Adds given amount to current experience amount */
	void AddToExperience( int32 InExperience );

	/** Broadcast Experience amount on change */
	FOnPlayerStatChanged OnExperienceChangedDelegate;


	//~ Begin IAbilitySystem Interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ End IAbilitySystem Interface

	/** Data asset containing level related information */
	UPROPERTY( EditDefaultsOnly )
	TObjectPtr<ULevelUpInfo> LevelUpInfo;

protected:
	/**
	 * AbilitySystemComponent owned by player character
	 * It persists after destruction of the player-controlled pawn and can be reused after creating new one
	 */
	UPROPERTY( VisibleAnywhere )
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	/**
	 * AttributeSet owned by player character
	 * It persists after destruction of the player-controlled pawn and can be reused after creating new one
	 */
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:
	/** Player level based on experience points */
	UPROPERTY( VisibleAnywhere, ReplicatedUsing = OnRep_Level )
	int32 Level = 1;

	/** Total amount of experience Points, does not reset on level up */
	UPROPERTY( VisibleAnywhere, ReplicatedUsing = OnRep_Experience )
	int32 Experience = 0;

	/** Level replication function */
	UFUNCTION()
	void OnRep_Level( int32 OldLevel );

	/** Experience replication function */
	UFUNCTION()
	void OnRep_Experience( int32 OldExperience );
};
