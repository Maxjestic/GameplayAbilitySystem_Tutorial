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
	virtual FString GetDescription( const int32 Level ) override;
	virtual FString GetNextLevelDescription( const int32 Level ) override;
	//~ End UAuraGameplayAbility Interface

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	int32 MaxNumShards = 11;
};
