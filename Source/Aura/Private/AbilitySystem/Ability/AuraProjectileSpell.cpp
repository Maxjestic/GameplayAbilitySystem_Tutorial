// Copyright Maxjestic


#include "AbilitySystem/Ability/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"

void UAuraProjectileSpell::ActivateAbility( const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo,
                                            const FGameplayAbilityActivationInfo ActivationInfo,
                                            const FGameplayEventData* TriggerEventData )
{
	Super::ActivateAbility( Handle, ActorInfo, ActivationInfo, TriggerEventData );
}

void UAuraProjectileSpell::SpawnProjectile( const FVector& ProjectileTargetRotation )
{
	if (!GetAvatarActorFromActorInfo()->HasAuthority()) return;
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>( GetAvatarActorFromActorInfo() ))
	{
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
		FRotator Rotation = (ProjectileTargetRotation - SocketLocation).Rotation();
		// We want projectile to move parallel to the ground
		Rotation.Pitch = 0.f;

		FTransform SpawnTransform;
		SpawnTransform.SetLocation( SocketLocation );
		SpawnTransform.SetRotation( Rotation.Quaternion() );

		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>( ProjectileClass,
		                                                                               SpawnTransform,
		                                                                               GetOwningActorFromActorInfo(),
		                                                                               Cast<APawn>( GetOwningActorFromActorInfo() ),
		                                                                               ESpawnActorCollisionHandlingMethod::AlwaysSpawn );

		// Setting up gameplay effect for ability (projectile)
		const UAbilitySystemComponent* SourceAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent( GetAvatarActorFromActorInfo() );
		FGameplayEffectContextHandle EffectContextHandle = SourceAbilitySystemComponent->MakeEffectContext();
		
		EffectContextHandle.SetAbility( this );
		EffectContextHandle.AddSourceObject( Projectile );
		TArray<TWeakObjectPtr<AActor>> Actors;
		Actors.Add(Projectile);
		EffectContextHandle.AddActors( Actors );
		FHitResult HitResult;
		HitResult.Location = ProjectileTargetRotation;
		EffectContextHandle.AddHitResult( HitResult );
		
		const FGameplayEffectSpecHandle SpecHandle = SourceAbilitySystemComponent->MakeOutgoingSpec( DamageEffectClass,
																									 GetAbilityLevel(),
																									 EffectContextHandle );

		const float ScaledDamage = Damage.GetValueAtLevel( 10 );
		const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();

		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude( SpecHandle, GameplayTags.Damage, ScaledDamage );
		Projectile->DamageEffectSpecHandle = SpecHandle;

		Projectile->FinishSpawning( SpawnTransform );
	}
}
