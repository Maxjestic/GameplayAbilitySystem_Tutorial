// Copyright Maxjestic


#include "Checkpoint/MapEntrance.h"

#include "Components/SphereComponent.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"

AMapEntrance::AMapEntrance( const FObjectInitializer& ObjectInitializer )
	: Super( ObjectInitializer )
{
	Sphere->SetupAttachment( MoveToComponent );
}

void AMapEntrance::LoadActor_Implementation()
{
	// Do nothing when loading a map entrance
}

void AMapEntrance::HighlightActor_Implementation()
{
	CheckpointMesh->SetRenderCustomDepth( true );
}

void AMapEntrance::OnSphereOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult )
{
	if (OtherActor->Implements<UPlayerInterface>())
	{
		bReached = true;

		if (const AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>( UGameplayStatics::GetGameMode( this ) ))
		{
			AuraGameMode->SaveWorldState( GetWorld(), DestinationMap.ToSoftObjectPath().GetAssetName() );
		}

		IPlayerInterface::Execute_SaveProgress( OtherActor, DestinationPlayerStartTag );

		UGameplayStatics::OpenLevelBySoftObjectPtr( this, DestinationMap );
	}
}
