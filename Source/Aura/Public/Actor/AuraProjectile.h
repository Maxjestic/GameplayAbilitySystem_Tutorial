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

protected:
	//~ Begin AActor Interface
	virtual void BeginPlay() override;
	void OnHit();
	virtual void Destroyed() override;
	//~ End AActor Interface

	/** On overlap callback */
	UFUNCTION()
	void OnSphereOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                      int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult );
	
	/** Collision sphere component */
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly )
	TObjectPtr<USphereComponent> Sphere;

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

	/** Did effects got taken care of */
	bool bHit = false;

	/** LifeSpan of the projectile */
	UPROPERTY( EditDefaultsOnly )
	float LifeSpan = 15.f;
};
