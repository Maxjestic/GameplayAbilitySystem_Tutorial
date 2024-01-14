// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
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

	/** Spec handle for damaging gameplay effect */
	UPROPERTY( BlueprintReadWrite, meta = (ExposeOnSpawn) )
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

protected:
	//~ Begin AActor Interface
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	//~ End AActor Interface

	/** On overlap callback */
	UFUNCTION()
	void OnSphereOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                      int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult );

private:
	/** Collision sphere component */
	UPROPERTY( VisibleAnywhere )
	TObjectPtr<USphereComponent> Sphere;

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
