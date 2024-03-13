// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagicCircle.generated.h"

/*
 * class for arcane shards ability’s magic circle
 */
UCLASS()
class AURA_API AMagicCircle : public AActor
{
	GENERATED_BODY()

public:
	/** Default constructor */
	AMagicCircle();

	//~ Begin AActor Interface
	virtual void Tick( float DeltaTime ) override;
	//~ End AActor Interface

	/** Decal component for magic circle */
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly )
	TObjectPtr<UDecalComponent> MagicCircleDecal;
	
protected:
	//~ Begin AActor Interface
	virtual void BeginPlay() override;
	//~ End AActor Interface
};
