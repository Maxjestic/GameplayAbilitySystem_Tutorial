// Copyright Maxjestic


#include "AbilitySystem/Ability/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"

FString UAuraProjectileSpell::GetDescription( const int32 Level )
{
	const FString Bolts = Level == 1
		                      ? FString::Printf( TEXT( "a bolt" ) )
		                      : FString::Printf( TEXT( "%d bolts" ), FMath::Min( Level, 5 ) );

	const int32 Damage = DamageTypes[FAuraGameplayTags::Get().Damage_Fire].GetValueAtLevel( Level );

	return FString::Printf( TEXT( "<Title>Fire Bolt</> <Small>Level: </><Level>%d</>\n\n"
		"<Default>Launches %s of fire, exploding on impact and dealing: </><Damage>%d</>"
		"<Default> fire damage with a chance to burn.</>" ),
	                        Level,
	                        *Bolts,
	                        Damage );
}

FString UAuraProjectileSpell::GetNextLevelDescription( const int32 Level )
{
	const int32 NextLevel = Level + 1;
	const FString Bolts = FString::Printf( TEXT( "%d bolts" ), FMath::Min( NextLevel, 5 ) );

	const int32 Damage = DamageTypes[FAuraGameplayTags::Get().Damage_Fire].GetValueAtLevel( NextLevel );

	return FString::Printf( TEXT( "<Title>Fire Bolt</> <Small>Level: </><Level>%d</>\n\n"
		"<Default>Launches %s of fire, exploding on impact and dealing: </><Damage>%d</>"
		"<Default> fire damage with a chance to burn.</>" ),
	                        NextLevel,
	                        *Bolts,
	                        Damage );
}

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

	// Setting up gameplay effect for ability (projectile)
	const UAbilitySystemComponent* SourceAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(
		GetAvatarActorFromActorInfo() );
	FGameplayEffectContextHandle EffectContextHandle = SourceAbilitySystemComponent->MakeEffectContext();

	EffectContextHandle.SetAbility( this );
	EffectContextHandle.AddSourceObject( Projectile );
	TArray<TWeakObjectPtr<AActor>> Actors;
	Actors.Add( Projectile );
	EffectContextHandle.AddActors( Actors );
	FHitResult HitResult;
	HitResult.Location = ProjectileTargetRotation;
	EffectContextHandle.AddHitResult( HitResult );

	const FGameplayEffectSpecHandle SpecHandle = SourceAbilitySystemComponent->MakeOutgoingSpec( DamageEffectClass,
		GetAbilityLevel(),
		EffectContextHandle );

	for (auto& Pair : DamageTypes)
	{
		const float ScaledDamage = Pair.Value.GetValueAtLevel( GetAbilityLevel() );
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude( SpecHandle, Pair.Key, ScaledDamage );
	}

	Projectile->DamageEffectSpecHandle = SpecHandle;
	Projectile->FinishSpawning( SpawnTransform );
}
