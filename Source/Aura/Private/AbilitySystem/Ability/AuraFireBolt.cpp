// Copyright Maxjestic


#include "AbilitySystem/Ability/AuraFireBolt.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

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
	FRotator ForwardRotation = (ProjectileTargetLocation - SocketLocation).Rotation();

	if (bOverridePitch)
	{
		ForwardRotation.Pitch = PitchOverride;
	}

	const FVector Forward = ForwardRotation.Vector();
	const int32 EffectiveNumProjectiles = FMath::Min(GetAbilityLevel(), MaxNumProjectiles);
	
	const TArray<FRotator> Rotations = UAuraAbilitySystemLibrary::EvenlySpacedRotators(
		Forward,
		FVector::UpVector,
		ProjectileSpread,
		EffectiveNumProjectiles );

	for (const FRotator& Rotation : Rotations)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation( SocketLocation );
		SpawnTransform.SetRotation( Rotation.Quaternion() );

		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>( ProjectileClass,
		                                                                               SpawnTransform,
		                                                                               GetOwningActorFromActorInfo(),
		                                                                               Cast<APawn>( GetOwningActorFromActorInfo() ),
		                                                                               ESpawnActorCollisionHandlingMethod::AlwaysSpawn );

		Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();

		if (HomingTarget && HomingTarget->Implements<UCombatInterface>())
		{
			Projectile->ProjectileMovement->HomingTargetComponent = HomingTarget->GetRootComponent();
		}
		else
		{
			Projectile->HomingTargetSceneComponent = NewObject<USceneComponent>( USceneComponent::StaticClass() );
			Projectile->HomingTargetSceneComponent->SetWorldLocation( ProjectileTargetLocation );
			Projectile->ProjectileMovement->HomingTargetComponent = Projectile->HomingTargetSceneComponent;
		}
		Projectile->ProjectileMovement->HomingAccelerationMagnitude = FMath::RandRange( HomingAccelerationMin, HomingAccelerationMax );
		Projectile->ProjectileMovement->bIsHomingProjectile = bLaunchHomingProjectiles;

		Projectile->FinishSpawning( SpawnTransform );
	}
}
