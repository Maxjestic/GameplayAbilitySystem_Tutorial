// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"

class UAttributeSet;

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

	//~ Begin IAbilitySystem Interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ End IAbilitySystem Interface
	
protected:
	/**
	 * AbilitySystemComponent owned by player character
	 * It persists after destruction of the player-controlled pawn and can be reused after creating new one
	 */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	/**
	 * AttributeSet owned by player character
	 * It persists after destruction of the player-controlled pawn and can be reused after creating new one
	 */
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:
	/** Player level */
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level)
	int32 Level = 1;

	/** Level replication function */
	UFUNCTION()
	void OnRep_Level( int32 OldLevel );
};
