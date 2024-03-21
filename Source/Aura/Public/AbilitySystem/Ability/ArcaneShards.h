// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/AuraDamageGameplayAbility.h"
#include "ArcaneShards.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UArcaneShards : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

public:
	//~ Begin UAuraGameplayAbility Interface
	virtual FString GetDescription(const int32 Level, const bool bForNextLevel) override;
	//~ End UAuraGameplayAbility Interface

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	int32 MaxNumShards = 11;
};
