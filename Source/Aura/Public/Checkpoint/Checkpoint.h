// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/SaveInterface.h"
#include "Checkpoint.generated.h"

class USphereComponent;

/**
 * PlayerStart that can save data
 */
UCLASS()
class AURA_API ACheckpoint : public APlayerStart, public ISaveInterface
{
	GENERATED_BODY()

public:
	/**
	 * Default constructor
	 * sets tick, sphere overlap
	 */
	ACheckpoint( const FObjectInitializer& ObjectInitializer );

	//~ Begin ISaveInterface
	virtual bool ShouldLoadTransform_Implementation() override { return false; }
	virtual void LoadActor_Implementation() override;
	//~ End ISaveInterface

	/** True if player reached this checkpoint already */
	UPROPERTY( BlueprintReadOnly, SaveGame )
	bool bReached = false;

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

	/** Representing checkpoint */
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly )
	TObjectPtr<UStaticMeshComponent> CheckpointMesh;

private:
	/** Used to detect overlap */
	UPROPERTY( VisibleAnywhere )
	TObjectPtr<USphereComponent> Sphere;
};
