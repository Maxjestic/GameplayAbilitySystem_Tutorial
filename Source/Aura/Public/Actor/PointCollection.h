// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PointCollection.generated.h"

UCLASS()
class AURA_API APointCollection : public AActor
{
	GENERATED_BODY()

public:
	/**
	 * Default constructor
	 * Creates all points and adds them to the array
	 */
	APointCollection();

	/** Returns given number of points on the ground corresponding to points stored by me */
	UFUNCTION( BlueprintPure )
	TArray<USceneComponent*> GetGroundPoints( const FVector& GroundLocation, int32 NumberOfPoints, float YawOverride = 0 );

protected:
	//~ Begin AActor Interface
	virtual void BeginPlay() override;
	//~ End AActor Interface

	/** Array of points, which wont be changing their position*/
	UPROPERTY( BlueprintReadOnly, VisibleAnywhere )
	TArray<USceneComponent*> ImmutablePoints;

	/*
	 * Scene components representing points
	 * Point_1 - Root component
	 */

	UPROPERTY( BlueprintReadOnly, VisibleAnywhere )
	TObjectPtr<USceneComponent> Point_0;

	UPROPERTY( BlueprintReadOnly, VisibleAnywhere )
	TObjectPtr<USceneComponent> Point_1;

	UPROPERTY( BlueprintReadOnly, VisibleAnywhere )
	TObjectPtr<USceneComponent> Point_2;

	UPROPERTY( BlueprintReadOnly, VisibleAnywhere )
	TObjectPtr<USceneComponent> Point_3;

	UPROPERTY( BlueprintReadOnly, VisibleAnywhere )
	TObjectPtr<USceneComponent> Point_4;

	UPROPERTY( BlueprintReadOnly, VisibleAnywhere )
	TObjectPtr<USceneComponent> Point_5;

	UPROPERTY( BlueprintReadOnly, VisibleAnywhere )
	TObjectPtr<USceneComponent> Point_6;

	UPROPERTY( BlueprintReadOnly, VisibleAnywhere )
	TObjectPtr<USceneComponent> Point_7;

	UPROPERTY( BlueprintReadOnly, VisibleAnywhere )
	TObjectPtr<USceneComponent> Point_8;

	UPROPERTY( BlueprintReadOnly, VisibleAnywhere )
	TObjectPtr<USceneComponent> Point_9;

	UPROPERTY( BlueprintReadOnly, VisibleAnywhere )
	TObjectPtr<USceneComponent> Point_10;
};
