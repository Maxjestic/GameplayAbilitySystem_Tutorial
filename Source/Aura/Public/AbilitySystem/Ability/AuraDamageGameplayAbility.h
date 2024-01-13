// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/AuraGameplayAbility.h"
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

	UPROPERTY( EditDefaultsOnly, Category="Damage" )
	TMap<FGameplayTag, FScalableFloat> DamageTypes;
};
