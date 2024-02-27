// Copyright Maxjestic


#include "AbilitySystem/Ability/AuraFireBolt.h"

#include "Kismet/KismetSystemLibrary.h"

FString UAuraFireBolt::GetDescription( const int32 Level )
{
	const float ManaCost = FMath::Abs( GetManaCost( Level ) );
	const float Cooldown = GetCooldown( Level );
	const FString Bolts = Level == 1
		                      ? FString::Printf( TEXT( "a bolt" ) )
		                      : FString::Printf( TEXT( "%d bolts" ), FMath::Min( Level, MaxNumProjectiles ) );
	const float ScaledDamage = Damage.GetValueAtLevel( Level );

	return FString::Printf( TEXT(
		// Title
		"<Title>Fire Bolt</>\n\n"

		// Details
		"<Small>Level: </><Level>%d</>\n"
		"<Small>Cost: </><ManaCost>%.1f</><Small> mana</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</><Small>s</>\n\n"

		// Description
		"<Default>Launches %s of fire, exploding on impact and dealing: </><Damage>%.2f</>"
		"<Default> fire damage with a chance to burn.</>" ),
	                        Level,
	                        ManaCost,
	                        Cooldown,
	                        *Bolts,
	                        ScaledDamage );
}

FString UAuraFireBolt::GetNextLevelDescription( const int32 Level )
{
	const int32 NextLevel = Level + 1;
	const float ManaCost = FMath::Abs( GetManaCost( NextLevel ) );
	const float Cooldown = GetCooldown( NextLevel );
	const float ScaledDamage = Damage.GetValueAtLevel( Level );

	return FString::Printf( TEXT(
		// Title
		"<Title>Next Level:</>\n\n"

		// Details
		"<Small>Level: </><Level>%d</>\n"
		"<Small>Cost: </><ManaCost>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</><Small>s</>\n\n"

		// Description
		"<Default>Launches %d bolts of fire, exploding on impact and dealing: </><Damage>%.2f</>"
		"<Default> fire damage with a chance to burn.</>" ),
	                        NextLevel,
	                        ManaCost,
	                        Cooldown,
	                        FMath::Min( NextLevel, MaxNumProjectiles ),
	                        ScaledDamage );
}

void UAuraFireBolt::SpawnProjectiles( const FVector& ProjectileTargetLocation, const FGameplayTag SocketTag, const bool bOverridePitch,
                                      const float PitchOverride, AActor* HomingTarget )
{
	if (!GetAvatarActorFromActorInfo()->HasAuthority())
	{
		return;
	}

	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(
		GetAvatarActorFromActorInfo(),
		SocketTag );
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();

	if (bOverridePitch)
	{
		Rotation.Pitch = PitchOverride;
	}

	const FVector Forward = Rotation.Vector();
	const FVector LeftOfSpread = Forward.RotateAngleAxis( -ProjectileSpread/2.f, FVector::UpVector );
	const FVector RightOfSpread = Forward.RotateAngleAxis( ProjectileSpread/2.f, FVector::UpVector );

	//const int32 NumProjectiles = FMath::Min( MaxNumProjectiles, GetAbilityLevel() );
	
	const FVector Start = SocketLocation + FVector(0,0,10);
	if (NumProjectiles > 1)
	{		
		const float DeltaSpread = ProjectileSpread / (NumProjectiles - 1);
		for (int32 i = 0; i < NumProjectiles; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis( DeltaSpread * i, FVector::UpVector );

			UKismetSystemLibrary::DrawDebugArrow( GetAvatarActorFromActorInfo(),
												  Start,
												  Start + Direction * 75.f,
												  5,
												  FLinearColor::Red,
												  120,
												  1 );
		}
	}
	else
	{
		// Single projectile
		UKismetSystemLibrary::DrawDebugArrow( GetAvatarActorFromActorInfo(),
												  Start,
												  Start + Forward * 75.f,
												  5,
												  FLinearColor::Red,
												  120,
												  1 );
	}

	UKismetSystemLibrary::DrawDebugArrow( GetAvatarActorFromActorInfo(),
	                                      SocketLocation,
	                                      SocketLocation + Forward * 100.f,
	                                      5,
	                                      FLinearColor::White,
	                                      120,
	                                      1 );
	UKismetSystemLibrary::DrawDebugArrow( GetAvatarActorFromActorInfo(),
										  SocketLocation,
										  SocketLocation + LeftOfSpread * 100.f,
										  5,
										  FLinearColor::Gray,
										  120,
										  1 );
	UKismetSystemLibrary::DrawDebugArrow( GetAvatarActorFromActorInfo(),
										  SocketLocation,
										  SocketLocation + RightOfSpread * 100.f,
										  5,
										  FLinearColor::Gray,
										  120,
										  1 );
}
