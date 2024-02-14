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

public:
	//~ Begin UAuraGameplayAbility Interface
	virtual FString GetDescription( const int32 Level ) override;
	virtual FString GetNextLevelDescription( const int32 Level ) override;
	//~ End UAuraGameplayAbility Interface

protected:
	//~ Begin UGameplayAbility Interface
	virtual void ActivateAbility( const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                              const FGameplayAbilityActivationInfo ActivationInfo,
	                              const FGameplayEventData* TriggerEventData ) override;
	//~ End UGameplayAbility Interface	

	/** Spawns projectile, when it should be delayed */
	UFUNCTION( BlueprintCallable, Category = "Projectile" )
	void SpawnProjectile( const FVector& ProjectileTargetRotation, const FGameplayTag SocketTag, const bool bOverridePitch = false,
	                      const float PitchOverride = 0.f );

	/** Projectile class to spawn */
	UPROPERTY( EditAnywhere, BlueprintReadOnly )
	TSubclassOf<AAuraProjectile> ProjectileClass;

	/** Maximum number of projectiles */
	UPROPERTY( EditDefaultsOnly )
	int32 NumProjectiles = 5;
};
