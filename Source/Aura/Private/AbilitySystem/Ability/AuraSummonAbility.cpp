// Copyright Maxjestic


#include "AbilitySystem/Ability/AuraSummonAbility.h"

TArray<FVector> UAuraSummonAbility::GetSpawnLocations()
{
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const float DeltaSpread = SpawnSpreadAngle / NumMinionsToSpawn;

	const FVector LeftOfSpread = Forward.RotateAngleAxis( -SpawnSpreadAngle / 2.f, FVector::UpVector );
	TArray<FVector> SpawnLocations;

	for (int32 i = 0; i < NumMinionsToSpawn; i++)
	{
		const FVector Direction = LeftOfSpread.RotateAngleAxis( DeltaSpread * i, FVector::UpVector );
		FVector ChosenSpawnLocation = Location + Direction * FMath::FRandRange( MinSpawnDistance, MaxSpawnDistance );

		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel( HitResult,
		                                      ChosenSpawnLocation + FVector( 0.f, 0.f, 400.f ),
		                                      ChosenSpawnLocation - FVector( 0.f, 0.f, 400.f ),
		                                      ECC_Visibility );

		if(HitResult.bBlockingHit)
		{
			ChosenSpawnLocation = HitResult.ImpactPoint;
		}

		SpawnLocations.Add( ChosenSpawnLocation );
	}

	return SpawnLocations;
}

TSubclassOf<APawn> UAuraSummonAbility::GetRandomMinionClass()
{
	const int32 Selection = FMath::RandRange( 0, MinionClasses.Num() - 1 );
	return MinionClasses[Selection];
}
