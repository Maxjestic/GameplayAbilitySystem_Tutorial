// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "Aura/Aura.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/HighlightInterface.h"
#include "Interaction/SaveInterface.h"
#include "Checkpoint.generated.h"

class USphereComponent;

/**
 * PlayerStart that can save data
 */
UCLASS()
class AURA_API ACheckpoint : public APlayerStart, public ISaveInterface, public IHighlightInterface
{
	GENERATED_BODY()

public:
	/**
	 * Default constructor
	 * sets tick, sphere overlap
	 */
	ACheckpoint( const FObjectInitializer& ObjectInitializer );

	//~ Begin ISave Interface
	virtual bool ShouldLoadTransform_Implementation() override { return false; }
	virtual void LoadActor_Implementation() override;
	//~ End ISave Interface

	//~ Begin IHighlight Interface
	virtual void HighlightActor_Implementation() override;
	virtual void UnHighlightActor_Implementation() override;
	virtual void SetMoveToLocation_Implementation( FVector& OutDestination ) override;
	//~ End IHighlight Interface

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

	/** Component used to get move to location for player when clicked */
	UPROPERTY( VisibleAnywhere )
	TObjectPtr<USceneComponent> MoveToComponent;

	/** Representing checkpoint */
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly )
	TObjectPtr<UStaticMeshComponent> CheckpointMesh;

	/** Highlight color */
	UPROPERTY( EditDefaultsOnly )
	int32 CustomDepthStencilOverride = CUSTOM_DEPTH_TAN;

private:
	/** Used to detect overlap */
	UPROPERTY( VisibleAnywhere )
	TObjectPtr<USphereComponent> Sphere;
};
