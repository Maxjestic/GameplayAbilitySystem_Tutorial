// Copyright Maxjestic


#include "AbilitySystem/Ability/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"

void UAuraProjectileSpell::ActivateAbility( const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo,
                                            const FGameplayAbilityActivationInfo ActivationInfo,
                                            const FGameplayEventData* TriggerEventData )
{
	Super::ActivateAbility( Handle, ActorInfo, ActivationInfo, TriggerEventData );
}

void UAuraProjectileSpell::SpawnProjectile( const FVector& ProjectileTargetRotation, const FGameplayTag SocketTag,
                                            const bool bOverridePitch,
                                            const float PitchOverride )
{
	if (!GetAvatarActorFromActorInfo()->HasAuthority()) return;

	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(
		GetAvatarActorFromActorInfo(),
		SocketTag );
	FRotator Rotation = (ProjectileTargetRotation - SocketLocation).Rotation();

	if (bOverridePitch)
	{
		Rotation.Pitch = PitchOverride;
	}

	FTransform SpawnTransform;
	SpawnTransform.SetLocation( SocketLocation );
	SpawnTransform.SetRotation( Rotation.Quaternion() );

	AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>( ProjectileClass,
	                                                                               SpawnTransform,
	                                                                               GetOwningActorFromActorInfo(),
	                                                                               Cast<APawn>( GetOwningActorFromActorInfo() ),
	                                                                               ESpawnActorCollisionHandlingMethod::AlwaysSpawn );

	Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();

	Projectile->FinishSpawning( SpawnTransform );
}
