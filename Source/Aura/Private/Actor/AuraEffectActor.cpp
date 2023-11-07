// Copyright Maxjestic


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraEffectActor::ApplyEffectToTarget( AActor* TargetActor, const FAuraGameplayEffect AuraGameplayEffect )
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	const TSubclassOf<UGameplayEffect> GameplayEffectClass = AuraGameplayEffect.GameplayEffectClass;
	
	if (TargetASC == nullptr) return;
	check(GameplayEffectClass)

	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(
		GameplayEffectClass, 1.f, EffectContextHandle);
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(
		*EffectSpecHandle.Data.Get());

	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfinite && AuraGameplayEffect.EffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
	}
}

void AAuraEffectActor::RemoveEffectFromTarget( AActor* TargetActor )
{
	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
	{
		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		for (auto HandlePair : ActiveEffectHandles)
		{
			if (TargetASC == HandlePair.Value)
			{
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
				HandlesToRemove.Add(HandlePair.Key);
			}
		}
		for (auto& Handle : HandlesToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}
	}
}

void AAuraEffectActor::OnOverlap( AActor* TargetActor )
{
	for (const auto Effect : InstantGameplayEffects)
	{
		if (Effect.EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
		{
			ApplyEffectToTarget(TargetActor, Effect);
		}
	}

	for (const auto Effect : InfiniteGameplayEffects)
	{
		if (Effect.EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
		{
			ApplyEffectToTarget(TargetActor, Effect);
		}
	}

	for (const auto Effect : DurationGameplayEffects)
	{
		if (Effect.EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
		{
			ApplyEffectToTarget(TargetActor, Effect);
		}
	}
}

void AAuraEffectActor::OnEndOverlap( AActor* TargetActor )
{
	for (const auto Effect : InstantGameplayEffects)
	{
		if (Effect.EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
		{
			ApplyEffectToTarget(TargetActor, Effect);
		}
	}

	for (const auto Effect : InfiniteGameplayEffects)
	{
		if (Effect.EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
		{
			ApplyEffectToTarget(TargetActor, Effect);
		}
		if (Effect.EffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
		{
			RemoveEffectFromTarget(TargetActor);
		}
	}

	for (const auto Effect : DurationGameplayEffects)
	{
		if (Effect.EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
		{
			ApplyEffectToTarget(TargetActor, Effect);
		}
	}
}
