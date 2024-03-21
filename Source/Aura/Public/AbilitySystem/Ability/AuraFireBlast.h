// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/AuraDamageGameplayAbility.h"
#include "AuraFireBlast.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraFireBlast : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

public:
	//~ Begin UAuraGameplayAbility Interface
	virtual FString GetDescription(const int32 Level, const bool bForNextLevel) override;
	//~ End UAuraGameplayAbility Interface

protected:
	UPROPERTY( EditDefaultsOnly, Category="FireBlast" )
	int32 NumFireBalls = 12;
};
