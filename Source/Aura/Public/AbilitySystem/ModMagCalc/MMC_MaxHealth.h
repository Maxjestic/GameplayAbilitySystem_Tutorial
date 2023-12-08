// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxHealth.generated.h"

/**
 * Custom calculation class used to calculate MaxHealth Attribute
 */
UCLASS()
class AURA_API UMMC_MaxHealth : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	/**
	 * Sets gameplay effect attribute capture definition
	 */
	UMMC_MaxHealth();

	//~ Begin UGameplayModMagnitudeCalculation Interface
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
	//~ End UGameplayModMagnitudeCalculation Interface
	
private:
	/** Vigor attribute to capture for calculation */
	FGameplayEffectAttributeCaptureDefinition VigorDef;
	
};