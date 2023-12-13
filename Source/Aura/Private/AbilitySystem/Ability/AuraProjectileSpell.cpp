// Copyright Maxjestic


#include "AbilitySystem/Ability/AuraProjectileSpell.h"

#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"

void UAuraProjectileSpell::ActivateAbility( const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo,
                                            const FGameplayAbilityActivationInfo ActivationInfo,
                                            const FGameplayEventData* TriggerEventData )
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);	
}

void UAuraProjectileSpell::SpawnProjectile() const
{
	
	if (!GetAvatarActorFromActorInfo()->HasAuthority()) return;
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo()))
	{
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		// TODO: Set the Projectile Rotation.

		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass,
																					  SpawnTransform,
																					  GetOwningActorFromActorInfo(),
																					  Cast<APawn>(GetOwningActorFromActorInfo()),
																					  ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		/// TODO: Give the Projectile a Gameplay Effect Spec for causing Damage.

		Projectile->FinishSpawning(SpawnTransform);
	}
}
