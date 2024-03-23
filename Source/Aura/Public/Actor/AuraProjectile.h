// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilityTypes.h"
#include "GameFramework/Actor.h"
#include "AuraProjectile.generated.h"

class UNiagaraSystem;
class UProjectileMovementComponent;
class USphereComponent;

/**
 * base class for projectiles
 */
UCLASS()
class AURA_API AAuraProjectile : public AActor
{
	GENERATED_BODY()

public:
	/**
	 * Sets collision sphere and projectile movement component
	 */
	AAuraProjectile();

	/** Projectile movement component used by this */
	UPROPERTY( VisibleAnywhere )
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	/** Contains all data relevant do damage effect */
	UPROPERTY( BlueprintReadWrite, meta = (ExposeOnSpawn = true) )
	FDamageEffectParams DamageEffectParams;

	/** Scene components used as homing target (To handle garbage collection) */
	UPROPERTY()
	TObjectPtr<USceneComponent> HomingTargetSceneComponent;

protected:
	//~ Begin AActor Interface
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	//~ End AActor Interface

	/** Called when projectile hits something */
	UFUNCTION( BlueprintCallable )
	void OnHit();

	/** On overlap callback */
	UFUNCTION()
	virtual void OnSphereOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                              int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult );

	/** Ture if overlap is valid */
	bool IsValidOverlap( const AActor* OtherActor );

	/** Collision sphere component */
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly )
	TObjectPtr<USphereComponent> Sphere;

	/** Did effects got taken care of */
	bool bHit = false;

private:
	/** Niagara effect for impact */
	UPROPERTY( EditAnywhere )
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	/** Sound for impact */
	UPROPERTY( EditAnywhere )
	TObjectPtr<USoundBase> ImpactSound;

	/** Looping sound when projectile flies */
	UPROPERTY( EditAnywhere )
	TObjectPtr<USoundBase> LoopingSound;

	/** Looping sound component used to stop playing that sound */
	UPROPERTY()
	TObjectPtr<UAudioComponent> LoopingSoundComponent;

	/** LifeSpan of the projectile */
	UPROPERTY( EditDefaultsOnly )
	float LifeSpan = 15.f;
};
