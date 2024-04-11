// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/SaveInterface.h"
#include "AuraEnemySpawnVolume.generated.h"

class AAuraEnemySpawnPoint;
class UBoxComponent;

UCLASS()
class AURA_API AAuraEnemySpawnVolume : public AActor, public ISaveInterface
{
	GENERATED_BODY()

public:
	/** Default constructor */
	AAuraEnemySpawnVolume();

	//~ Begin ISave Interface
	virtual void LoadActor_Implementation() override;
	//~ End ISave Interface

	UPROPERTY( BlueprintReadOnly, SaveGame )
	bool bReached = false;

protected:
	//~ Begin AActor Interface
	virtual void BeginPlay() override;
	//~ End AActor Interface

	/** On overlap callback */
	UFUNCTION()
	virtual void OnBoxOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                           int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult );

	/** Points on which enemies will be spawned */
	UPROPERTY( EditAnywhere )
	TArray<AAuraEnemySpawnPoint*> SpawnPoints;

private:
	/** Volume box */
	UPROPERTY( VisibleAnywhere )
	TObjectPtr<UBoxComponent> Box;
};
