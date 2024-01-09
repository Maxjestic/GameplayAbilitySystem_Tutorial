// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/AuraDamageGameplayAbility.h"
#include "AuraProjectileSpell.generated.h"

class AAuraProjectile;

/**
 * Spell spawning projectile
 */
UCLASS()
class AURA_API UAuraProjectileSpell : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

protected:
	//~ Begin UGameplayAbility Interface
	virtual void ActivateAbility( const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                              const FGameplayAbilityActivationInfo ActivationInfo,
	                              const FGameplayEventData* TriggerEventData ) override;
	//~ End UGameplayAbility Interface

	/** Spawns projectile, when it should be delayed */
	UFUNCTION( BlueprintCallable, Category = "Projectile" )
	void SpawnProjectile( const FVector& ProjectileTargetRotation );

	/** Projectile class to spawn */
	UPROPERTY( EditAnywhere, BlueprintReadOnly )
	TSubclassOf<AAuraProjectile> ProjectileClass;
	
};
