// Copyright Maxjestic


#include "Checkpoint/Checkpoint.h"

#include "Components/SphereComponent.h"

ACheckpoint::ACheckpoint( const FObjectInitializer& ObjectInitializer )
	: Super( ObjectInitializer )
{
	PrimaryActorTick.bCanEverTick = false;

	CheckpointMesh = CreateDefaultSubobject<UStaticMeshComponent>( "Checkpoint Mesh" );
	CheckpointMesh->SetupAttachment( GetRootComponent() );
	CheckpointMesh->SetCollisionEnabled( ECollisionEnabled::QueryAndPhysics );
	CheckpointMesh->SetCollisionResponseToAllChannels( ECR_Block );
	
	Sphere = CreateDefaultSubobject<USphereComponent>( "Sphere" );
	Sphere->SetupAttachment( CheckpointMesh );
	Sphere->SetCollisionEnabled( ECollisionEnabled::QueryOnly );
	Sphere->SetCollisionResponseToAllChannels( ECR_Ignore );
	Sphere->SetCollisionResponseToChannel( ECC_Pawn, ECR_Overlap );
}

void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic( this, &ThisClass::OnSphereOverlap );
}

void ACheckpoint::OnSphereOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                   int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult )
{
	if(OtherActor->ActorHasTag( FName("Player") ))
	{
		HandleGlowEffects();
	}
}

void ACheckpoint::HandleGlowEffects()
{
	Sphere->SetCollisionEnabled( ECollisionEnabled::NoCollision );
	UMaterialInstanceDynamic* DynamicMaterialInstance = UMaterialInstanceDynamic::Create(CheckpointMesh->GetMaterial(0), this);
	CheckpointMesh->SetMaterial( 0, DynamicMaterialInstance );
	CheckpointReached( DynamicMaterialInstance );
}
