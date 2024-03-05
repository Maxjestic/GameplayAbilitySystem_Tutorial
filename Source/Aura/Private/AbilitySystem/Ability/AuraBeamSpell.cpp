// Copyright Maxjestic


#include "AbilitySystem/Ability/AuraBeamSpell.h"

#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

void UAuraBeamSpell::StoreMouseDataInfo( const FHitResult& HitResult )
{
	if (HitResult.bBlockingHit)
	{
		MouseHitLocation = HitResult.ImpactPoint;
		MouseHitActor = HitResult.GetActor();
	}
	else
	{
		CancelAbility( CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true );
	}
}

void UAuraBeamSpell::StoreOwnerVariables()
{
	if (CurrentActorInfo)
	{
		OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
		OwnerCharacter = Cast<ACharacter>( CurrentActorInfo->AvatarActor );
	}
}

void UAuraBeamSpell::TraceFirstTarget( const FVector& BeamTargetLocation )
{
	check( OwnerCharacter )
	if (OwnerCharacter->Implements<UCombatInterface>())
	{
		if (const USkeletalMeshComponent* Weapon = ICombatInterface::Execute_GetWeapon( OwnerCharacter ))
		{
			TArray<AActor*> ActorsToIgnore;
			FHitResult HitResult;
			ActorsToIgnore.Add( OwnerCharacter );
			const FVector SocketLocation = Weapon->GetSocketLocation( FName( "TipSocket" ) );
			UKismetSystemLibrary::SphereTraceSingle( OwnerCharacter,
			                                         SocketLocation,
			                                         BeamTargetLocation,
			                                         10.f,
			                                         TraceTypeQuery1,
			                                         false,
			                                         ActorsToIgnore,
			                                         EDrawDebugTrace::None,
			                                         HitResult,
			                                         true);
			if(HitResult.bBlockingHit)
			{
				MouseHitLocation = HitResult.ImpactPoint;
				MouseHitActor = HitResult.GetActor();
			}
		}
	}
}
