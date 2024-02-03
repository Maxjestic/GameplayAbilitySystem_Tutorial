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
	void AddToLevel( const int32 InLevel );

	/** Broadcast Experience amount on change */
	FOnPlayerStatChanged OnLevelChangedDelegate;


	/** Player Experience getter */
	FORCEINLINE int32 GetPlayerExperience() const { return Experience; }

	/** Sets Experience to given amount */
	void SetExperience( const int32 InExperience );

	/** Adds given amount to current experience amount */
	void AddToExperience( const int32 InExperience );

	/** Broadcast Experience progress percent on change */
	FOnPlayerStatChanged OnExperienceChangedDelegate;
	

	/** Player attribute points getter */
	FORCEINLINE int32 GetAttributePoints() const { return AttributePoints; }

	/** Sets Attribute Points to given amount */
	void SetAttributePoints( const int32 InAttributePoints );

	/** Adds given amount to Attribute Points */
	void AddToAttributePoints( int32 InAttributePoints );

	/** Broadcasts new amount of Attribute Points */
	FOnPlayerStatChanged OnAttributePointsChangedDelegate;
	

	/** Player Spell Points getter */
	FORCEINLINE int32 GetSpellPoints() const { return SpellPoints; }

	/** Sets Spell Points to given amount */
	void SetSpellPoints( const int32 InSpellPoints );

	/** Adds given amount to Spell Points */
	void AddToSpellPoints( int32 InSpellPoints );

	/** Broadcasts new amount of Spell Points */
	FOnPlayerStatChanged OnSpellPointsChangedDelegate;


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

	/** Points that can be spend to increase attributes */
	UPROPERTY( VisibleAnywhere, ReplicatedUsing = OnRep_AttributePoints )
	int32 AttributePoints = 0;

	/** Points that can be spend to unlock/upgrade spells */
	UPROPERTY( VisibleAnywhere, ReplicatedUsing = OnRep_SpellPoints )
	int32 SpellPoints = 0;

	/**
	 * Replication functions 
	 */
	UFUNCTION()
	void OnRep_Level( int32 OldLevel );

	UFUNCTION()
	void OnRep_Experience( int32 OldExperience );

	UFUNCTION()
	void OnRep_AttributePoints( int32 OldAttributePoints );

	UFUNCTION()
	void OnRep_SpellPoints( int32 OldSpellPoints );
};
