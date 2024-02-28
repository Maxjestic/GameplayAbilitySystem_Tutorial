// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/AuraProjectileSpell.h"
#include "AuraFireBolt.generated.h"

/**
 * Fire bolt spell - spawning number of homing projectiles
 * deal damage, can apply burn or knock back 
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

	/**
	 * Spawns number of projectiles from given socket
	 * projectiles homing to homing target
	 * pitch can be overriden
	 */
	UFUNCTION( BlueprintCallable )
	void SpawnProjectiles( const FVector& ProjectileTargetLocation, const FGameplayTag SocketTag, const bool bOverridePitch,
	                       const float PitchOverride, AActor* HomingTarget );

protected:
	/** Angle of a cone in which projectiles are spawned */
	UPROPERTY( EditDefaultsOnly, Category="FireBolt" )
	float ProjectileSpread = 90.f;

	/** Decide if projectiles are homing */
	UPROPERTY( EditDefaultsOnly, Category="FireBolt" )
	bool bLaunchHomingProjectiles = true;

	/** Minimum value of homing acceleration */
	UPROPERTY( EditDefaultsOnly, Category="FireBolt" )
	float HomingAccelerationMin = 1600.f;

	/** Maximum value of homing acceleration */
	UPROPERTY( EditDefaultsOnly, Category="FireBolt" )
	float HomingAccelerationMax = 3200.f;
};
