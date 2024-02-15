// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraGameplayAbility.generated.h"

/**
 * Base Gameplay Ability class in Aura
 */
UCLASS()
class AURA_API UAuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	/** Gameplay tag for startup ability, abilities granted during game can have their tags changed in runtime */
	UPROPERTY( EditDefaultsOnly, Category="Input" )
	FGameplayTag StartupInputTag;

	/** Returns spell description for given level */
	virtual FString GetDescription( const int32 Level );

	/** Returns spell description for next level */
	virtual FString GetNextLevelDescription( const int32 Level );

	/** Returns locked description */
	static FString GetLockedDescription( const int32 Level );

protected:
	// Returns mana cost of the ability
	float GetManaCost( const float InLevel = 1.f ) const;

	// Returns cooldown of the ability
	float GetCooldown( const float InLevel = 1.f ) const;
};
