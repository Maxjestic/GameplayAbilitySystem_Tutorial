// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxMana.generated.h"

/**
 * Custom calculation class used to calculate MaxMana Attribute
 */
UCLASS()
class AURA_API UMMC_MaxMana : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	/** Default constructor */
	UMMC_MaxMana();

	//~ Begin UGameplayModMagnitudeCalculation Interface
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
	//~ End UGameplayModMagnitudeCalculation Interface

private:
	/** Vigor attribute to capture for calculation */
	FGameplayEffectAttributeCaptureDefinition IntelligenceDef;
	
};