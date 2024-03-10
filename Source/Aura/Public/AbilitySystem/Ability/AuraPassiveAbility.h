// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/AuraGameplayAbility.h"
#include "AuraPassiveAbility.generated.h"

/**
 * Base class for passive abilities
 * Contains simple activation and deactivation logic
 */
UCLASS()
class AURA_API UAuraPassiveAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

public:
	//~ Begin UGameplayAbility Interface
	virtual void ActivateAbility( const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                              const FGameplayAbilityActivationInfo ActivationInfo,
	                              const FGameplayEventData* TriggerEventData ) override;
	//~ End UGameplayAbility Interface

	/** Callback to when ability gets deactivated */
	void ReceiveDeactivate( const FGameplayTag& AbilityTag );
};
