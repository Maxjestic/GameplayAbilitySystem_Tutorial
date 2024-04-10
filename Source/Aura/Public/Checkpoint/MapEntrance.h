// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "Checkpoint/Checkpoint.h"
#include "MapEntrance.generated.h"

/**
 * Takes care of traveling between maps
 */
UCLASS()
class AURA_API AMapEntrance : public ACheckpoint
{
	GENERATED_BODY()

public:
	/**
	 * Default constructor
	 * Attaches sphere to move to component
	 */
	AMapEntrance( const FObjectInitializer& ObjectInitializer );

	//~ Begin ISave Interface
	virtual void LoadActor_Implementation() override;
	//~ End ISave Interface

	//~ Begin IHighlight Interface
	virtual void HighlightActor_Implementation() override;
	//~ End IHighlight Interface

	/** Map to witch I lead */
	UPROPERTY( EditAnywhere )
	TSoftObjectPtr<UWorld> DestinationMap;

	/** Player start tag leading to start of next map */
	UPROPERTY( EditAnywhere )
	FName DestinationPlayerStartTag;

protected:
	/** On overlap callback */
	virtual void OnSphereOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                              int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult ) override;
};
