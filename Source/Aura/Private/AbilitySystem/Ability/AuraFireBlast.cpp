// Copyright Maxjestic


#include "AbilitySystem/Ability/AuraFireBlast.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraFireBall.h"

FString UAuraFireBlast::GetDescription( const int32 Level, const bool bForNextLevel )
{
	int32 DescriptionLevel;
	FString Title;
	if (bForNextLevel)
	{
		DescriptionLevel = Level + 1;
		Title = FString::Printf( TEXT( "Next Level" ) );
	}
	else
	{
		DescriptionLevel = Level;
		Title = FString::Printf( TEXT( "Fire Blast" ) );
	}
	const float ManaCost = FMath::Abs( GetManaCost( DescriptionLevel ) );
	const float Cooldown = GetCooldown( DescriptionLevel );
	const float ScaledDamage = Damage.GetValueAtLevel( DescriptionLevel );

	return FString::Printf( TEXT(
		// Title
		"<Title>%s</>\n\n"

		// Details
		"<Small>Level: </><Level>%d</>\n"
		"<Small>Cost: </><ManaCost>%.1f</><Small> mana</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</><Small>s</>\n\n"

		// Description
		"<Default>Launches %d fire balls in all directions, each coming back and </>"
		"<Default>exploding upon return causing</><Damage> %.2f </>"
		"<Default>radial fire damage with a chance to burn.</>" ),
	                        *Title,
	                        DescriptionLevel,
	                        ManaCost,
	                        Cooldown,
	                        NumFireBalls,
	                        ScaledDamage );
}

TArray<AAuraFireBall*> UAuraFireBlast::SpawnFireBalls()
{
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	const TArray<FRotator> Rotators = UAuraAbilitySystemLibrary::EvenlySpacedRotators( Forward, FVector::UpVector, 360.f, NumFireBalls );
	TArray<AAuraFireBall*> FireBalls;

	for (const FRotator& Rotator : Rotators)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation( Location );
		SpawnTransform.SetRotation( Rotator.Quaternion() );
		AAuraFireBall* FireBall = GetWorld()->SpawnActorDeferred<AAuraFireBall>( FireBallClass,
		                                                                         SpawnTransform,
		                                                                         GetOwningActorFromActorInfo(),
		                                                                         CurrentActorInfo->PlayerController->GetPawn(),
		                                                                         ESpawnActorCollisionHandlingMethod::AlwaysSpawn );

		FireBall->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		FireBall->ReturnToActor = GetAvatarActorFromActorInfo();
		FireBalls.Add( FireBall );
		
		FireBall->FinishSpawning( SpawnTransform );
	}

	return FireBalls;
}
