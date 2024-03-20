// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/AuraBeamSpell.h"
#include "Electrocute.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UElectrocute : public UAuraBeamSpell
{
	GENERATED_BODY()
	
public:
	//~ Begin UAuraGameplayAbility Interface
	virtual FString GetDescription( const int32 Level ) override;
	virtual FString GetNextLevelDescription( const int32 Level ) override;
	//~ End UAuraGameplayAbility Interface
	
};
