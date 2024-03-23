// Copyright Maxjestic


#include "Actor/AuraFireBall.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "GameplayCueManager.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Components/AudioComponent.h"

void AAuraFireBall::BeginPlay()
{
	Super::BeginPlay();
	StartOutgoingTimeline();
}

void AAuraFireBall::OnSphereOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                     int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult )
{
	if (!IsValidOverlap( OtherActor ))
	{
		return;
	}
	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent( OtherActor ))
		{
			const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
			DamageEffectParams.DeathImpulse = DeathImpulse;

			DamageEffectParams.TargetAbilitySystemComponent = TargetAbilitySystemComponent;
			UAuraAbilitySystemLibrary::ApplyDamageEffect( DamageEffectParams );
		}
	}
}

void AAuraFireBall::OnHit()
{
	if(GetOwner())
	{
		FGameplayCueParameters CueParameters;
		CueParameters.Location = GetActorLocation();
		UGameplayCueManager::ExecuteGameplayCue_NonReplicated( GetOwner(), FAuraGameplayTags::Get().GameplayCue_FireBlast, CueParameters );
	}
	
	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}

	bHit = true;
}
