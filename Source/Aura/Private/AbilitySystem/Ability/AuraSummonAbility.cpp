// Copyright Maxjestic


#include "AbilitySystem/Ability/AuraSummonAbility.h"

#include "Kismet/KismetSystemLibrary.h"

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
		const FVector ChosenSpawnLocation = Location + Direction * FMath::FRandRange( MinSpawnDistance, MaxSpawnDistance );
		SpawnLocations.Add(ChosenSpawnLocation);
		DrawDebugSphere( GetWorld(), ChosenSpawnLocation, 18.f, 12, FColor::Cyan, false, 3.f );

		UKismetSystemLibrary::DrawDebugArrow( GetAvatarActorFromActorInfo(),
											  Location + Direction * MinSpawnDistance,
											  Location + Direction * MaxSpawnDistance,
											  4.f,
											  FLinearColor::Green,
											  3.f );
		DrawDebugSphere( GetWorld(), Location + Direction * MinSpawnDistance, 5.f, 12, FColor::Red, false, 3.f );
		DrawDebugSphere( GetWorld(), Location + Direction * MaxSpawnDistance, 5.f, 12, FColor::Red, false, 3.f );
	}
	return SpawnLocations;
}
