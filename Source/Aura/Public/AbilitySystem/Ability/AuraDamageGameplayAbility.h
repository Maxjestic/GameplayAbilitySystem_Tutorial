// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/AuraGameplayAbility.h"
#include "Interaction/CombatInterface.h"
#include "AuraDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

public:
	/** Loops through all damage types and apply gameplay effect to given target */
	UFUNCTION( BlueprintCallable )
	void CauseDamage( AActor* TargetActor );

protected:
	/** Class of gameplay effect that this applies */
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	/** Damage type to damage amount */
	UPROPERTY( EditDefaultsOnly, Category="Damage" )
	TMap<FGameplayTag, FScalableFloat> DamageTypes;

	/** Returns random attack montage from montages */
	UFUNCTION( BlueprintPure )
	FTaggedMontage GetRandomTaggedMontageFromArray( const TArray<FTaggedMontage>& TaggedMontages ) const;
};
