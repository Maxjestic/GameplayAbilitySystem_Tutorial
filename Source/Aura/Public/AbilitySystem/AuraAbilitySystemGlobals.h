// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "AuraAbilitySystemGlobals.generated.h"

/**
 * Ability System Globals subclass with Aura related functionalities
 */
UCLASS()
class AURA_API UAuraAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

	//~ Begin UAbilitySystemGlobals Interface
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
	//~ End UAbilitySystemGlobals Interface
};
