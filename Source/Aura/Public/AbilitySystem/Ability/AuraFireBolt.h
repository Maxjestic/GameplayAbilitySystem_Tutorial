// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/AuraProjectileSpell.h"
#include "AuraFireBolt.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraFireBolt : public UAuraProjectileSpell
{
	GENERATED_BODY()
	
public:
	//~ Begin UAuraGameplayAbility Interface
	virtual FString GetDescription( const int32 Level ) override;
	virtual FString GetNextLevelDescription( const int32 Level ) override;
	//~ End UAuraGameplayAbility Interface
};
