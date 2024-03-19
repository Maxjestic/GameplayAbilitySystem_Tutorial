// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilityTypes.h"
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

	/** Creates FDamageEffectParams struct with all relevant data */
	UFUNCTION( BlueprintPure )
	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults( AActor* TargetActor = nullptr,
	                                                             const FVector InRadialDamageOrigin = FVector::ZeroVector ) const;

	/** Returns amount of damage for current ability level */
	UFUNCTION( BlueprintPure )
	float GetDamageAtLevel() const;

protected:
	/** Returns random attack montage from montages */
	UFUNCTION( BlueprintPure )
	FTaggedMontage GetRandomTaggedMontageFromArray( const TArray<FTaggedMontage>& TaggedMontages ) const;

	/** Class of gameplay effect that this applies */
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	/** Type of damage dealt by this */
	UPROPERTY( EditDefaultsOnly, Category="Damage" )
	FGameplayTag DamageType;

	/** Amount of damage dealt by this */
	UPROPERTY( EditDefaultsOnly, Category="Damage" )
	FScalableFloat Damage;

	/** Chance to apply a debuff (%) */
	UPROPERTY( EditDefaultsOnly, Category="Damage" )
	float DebuffChance = 20.f;

	/** Damage applied by debuff */
	UPROPERTY( EditDefaultsOnly, Category="Damage" )
	float DebuffDamage = 5.f;

	/** How frequently - period - debuff will be applied (per second) */
	UPROPERTY( EditDefaultsOnly, Category="Damage" )
	float DebuffFrequency = 1.f;

	/** For how long debuff will be applied (seconds) */
	UPROPERTY( EditDefaultsOnly, Category="Damage" )
	float DebuffDuration = 5.f;

	/** Force applied to target on death */
	UPROPERTY( EditDefaultsOnly, Category="Damage" )
	float DeathImpulseMagnitude = 1000.f;

	/** Chance to knockback target (%) */
	UPROPERTY( EditDefaultsOnly, Category="Damage" )
	float KnockbackChance = 20.f;

	/** Force applied to target on knockback */
	UPROPERTY( EditDefaultsOnly, Category="Damage" )
	float KnockbackForceMagnitude = 1000.f;

	UPROPERTY( EditDefaultsOnly, Category="Damage" )
	bool bIsRadialDamage = false;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category="Damage" )
	float RadialDamageInnerRadius = 0.f;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category="Damage" )
	float RadialDamageOuterRadius = 0.f;
};
