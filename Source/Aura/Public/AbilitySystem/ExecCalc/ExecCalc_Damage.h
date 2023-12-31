// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExecCalc_Damage.generated.h"

/**
 * Calculate damage which should be dealt to target
 */
UCLASS()
class AURA_API UExecCalc_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	/**  */
	UExecCalc_Damage();

	//~ Begin UGameplayEffectExecutionCalculation Interface
	virtual void Execute_Implementation( const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	                                     FGameplayEffectCustomExecutionOutput& OutExecutionOutput ) const override;
	//~ End UGameplayEffectExecutionCalculation Interface
};
