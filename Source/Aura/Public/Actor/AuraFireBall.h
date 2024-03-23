// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "Actor/AuraProjectile.h"
#include "AuraFireBall.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraFireBall : public AAuraProjectile
{
	GENERATED_BODY()

public:
	/** Starts timeline to go out */
	UFUNCTION( BlueprintImplementableEvent )
	void StartOutgoingTimeline();

	/** Actor to which I return */
	UPROPERTY( BlueprintReadOnly )
	TObjectPtr<AActor> ReturnToActor;

	/** Damage effect params for explosion of the projectile */
	UPROPERTY( BlueprintReadWrite )
	FDamageEffectParams ExplosionDamageParams;

protected:
	//~ Begin AActor Interface
	virtual void BeginPlay() override;
	//~ End AActor Interface

	//~ Begin AuraProjectile Interface
	virtual void OnSphereOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                              int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult ) override;
	//~ End AuraProjectile Interface
};
