// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Checkpoint.generated.h"

class USphereComponent;
/**
 * PlayerStart that can save data
 */
UCLASS()
class AURA_API ACheckpoint : public APlayerStart
{
	GENERATED_BODY()

public:
	/**
	 * Default constructor
	 * sets tick, sphere overlap
	 */
	ACheckpoint( const FObjectInitializer& ObjectInitializer );

protected:
	//~ Begin AActor Interface
	virtual void BeginPlay() override;
	//~ End AActor Interface

	/** On overlap callback */
	UFUNCTION()
	virtual void OnSphereOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                              int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult );

	/** Called when players enters checkpoint */
	UFUNCTION( BlueprintImplementableEvent )
	void CheckpointReached( UMaterialInstanceDynamic* DynamicMaterialInstance );

	/** Handles glow effect of the static mesh */
	void HandleGlowEffects();
private:
	/** Representing checkpoint */
	UPROPERTY( VisibleAnywhere )
	TObjectPtr<UStaticMeshComponent> CheckpointMesh;

	/** Used to detect overlap */
	UPROPERTY( VisibleAnywhere )
	TObjectPtr<USphereComponent> Sphere;
};