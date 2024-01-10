// Copyright Maxjestic


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent( CreateDefaultSubobject<USceneComponent>( "SceneRoot" ) );
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraEffectActor::ApplyEffectToTarget( AActor* TargetActor, const FAuraGameplayEffect AuraGameplayEffect )
{
	if (TargetActor->ActorHasTag( FName( "Enemy" ) ) && !bApplyEffectsToEnemies)
	{
		return;
	}

	UAbilitySystemComponent* TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent( TargetActor );
	const TSubclassOf<UGameplayEffect> GameplayEffectClass = AuraGameplayEffect.GameplayEffectClass;

	if (TargetAbilitySystemComponent == nullptr) return;
	check( GameplayEffectClass )

	FGameplayEffectContextHandle EffectContextHandle = TargetAbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject( this );

	const FGameplayEffectSpecHandle EffectSpecHandle =
		TargetAbilitySystemComponent->MakeOutgoingSpec( GameplayEffectClass, ActorLevel, EffectContextHandle );

	const FActiveGameplayEffectHandle ActiveEffectHandle =
		TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf( *EffectSpecHandle.Data.Get() );

	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;

	// If Gameplay Effect is infinite or should be removed OnEndOverlap we store it to remove it
	if (bIsInfinite && AuraGameplayEffect.EffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		ActiveEffectHandles.Add( ActiveEffectHandle, TargetAbilitySystemComponent );
	}

	if (bDestroyOnEffectApplication && !bIsInfinite)
	{
		Destroy();
	}
}

void AAuraEffectActor::RemoveEffectFromTarget( AActor* TargetActor )
{
	if (UAbilitySystemComponent* TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent( TargetActor ))
	{
		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		for (auto HandlePair : ActiveEffectHandles)
		{
			if (TargetAbilitySystemComponent == HandlePair.Value)
			{
				TargetAbilitySystemComponent->RemoveActiveGameplayEffect( HandlePair.Key, 1 );
				HandlesToRemove.Add( HandlePair.Key );
			}
		}
		for (auto& Handle : HandlesToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked( Handle );
		}
	}
}

void AAuraEffectActor::OnOverlap( AActor* TargetActor )
{
	if (TargetActor->ActorHasTag( FName( "Enemy" ) ) && !bApplyEffectsToEnemies)
	{
		return;
	}

	for (const auto Effect : InstantGameplayEffects)
	{
		if (Effect.EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
		{
			ApplyEffectToTarget( TargetActor, Effect );
		}
	}

	for (const auto Effect : InfiniteGameplayEffects)
	{
		if (Effect.EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
		{
			ApplyEffectToTarget( TargetActor, Effect );
		}
	}

	for (const auto Effect : DurationGameplayEffects)
	{
		if (Effect.EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
		{
			ApplyEffectToTarget( TargetActor, Effect );
		}
	}
}

void AAuraEffectActor::OnEndOverlap( AActor* TargetActor )
{
	if (TargetActor->ActorHasTag( FName( "Enemy" ) ) && !bApplyEffectsToEnemies)
	{
		return;
	}

	for (const auto Effect : InstantGameplayEffects)
	{
		if (Effect.EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
		{
			ApplyEffectToTarget( TargetActor, Effect );
		}
	}

	for (const auto Effect : InfiniteGameplayEffects)
	{
		if (Effect.EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
		{
			ApplyEffectToTarget( TargetActor, Effect );
		}
		if (Effect.EffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
		{
			RemoveEffectFromTarget( TargetActor );
		}
	}

	for (const auto Effect : DurationGameplayEffects)
	{
		if (Effect.EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
		{
			ApplyEffectToTarget( TargetActor, Effect );
		}
	}
}
