// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameFramework/Actor.h"
#include "AuraEffectActor.generated.h"

class UGameplayEffect;

/** Determines Application Policy of given Gameplay Effect */
UENUM( BlueprintType )
enum class EEffectApplicationPolicy
{
	// Apply Gameplay Effect when Effect Actor is overlapped
	ApplyOnOverlap,
	// Apply Gameplay Effect when Effect Actor is no longer overlapped
	ApplyOnEndOverlap,
	// Default - Gameplay Effect is not applied
	DoNotApply
};

/** Determines Removal Policy of given Infinite Gameplay Effect */
UENUM( BlueprintType )
enum class EEffectRemovalPolicy
{
	// Remove Gameplay Effect when Effect Actor is no longer overlapped
	RemoveOnEndOverlap,
	// Default - Gameplay Effect is not removed manually
	DoNotRemove
};

/** Holds needed information for each Gameplay Effect used by Effect Actor */
USTRUCT( BlueprintType )
struct FAuraGameplayEffect
{
	GENERATED_BODY()

	// Class of Gameplay Effect
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Effect Properties" )
	TSubclassOf<UGameplayEffect> GameplayEffectClass;

	// Type of Application Policy
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Effect Properties" )
	EEffectApplicationPolicy EffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	// Type of Removal Policy
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Effect Properties" )
	EEffectRemovalPolicy EffectRemovalPolicy = EEffectRemovalPolicy::DoNotRemove;
};

/**
 * Generic class applying gameplay effect
 * Collision component and responses to overlap events should be added in blueprint
 */
UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()

public:
	/**
	 *  sets RootComponent
	 */
	AAuraEffectActor();

protected:
	//~ Begin AActor Interface
	virtual void BeginPlay() override;
	//~ End AActor Interface

	/** Function which applies AuraGameplayEffect on TargetActor */
	UFUNCTION( BlueprintCallable )
	void ApplyEffectToTarget( AActor* TargetActor, const FAuraGameplayEffect AuraGameplayEffect );
	//void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	/** Function which removes Effect from TargetActor */
	UFUNCTION( BlueprintCallable )
	void RemoveEffectFromTarget( AActor* TargetActor );

	/** Function meant to handle OnBeginOverlapEvent to apply effects */
	UFUNCTION( BlueprintCallable )
	void OnOverlap( AActor* TargetActor );

	/** Function meant to handle OnEndOverlapEvent to apply or remove effects */
	UFUNCTION( BlueprintCallable )
	void OnEndOverlap( AActor* TargetActor );

	/** Decide if the this should be removed after the Effect is removed */
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Applicable Effects" )
	bool bDestroyOnEffectApplication = false;

	/** Decide if the this should be applied to enemies */
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Applicable Effects" )
	bool bApplyEffectsToEnemies = false;

	/** Array of Instant Gameplay Effects applicable by EffectActor */
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Applicable Effects" )
	TArray<FAuraGameplayEffect> InstantGameplayEffects;

	/** Array of Infinite Gameplay Effects applicable by EffectActor */
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Applicable Effects" )
	TArray<FAuraGameplayEffect> InfiniteGameplayEffects;

	/** Array of Duration Gameplay Effects applicable by EffectActor */
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Applicable Effects" )
	TArray<FAuraGameplayEffect> DurationGameplayEffects;

	/** Map containing information about which effects are active on given ASCs used to remove effects */
	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles;

	/** Level used to deduce magnitude of the effects */
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Applicable Effects" )
	float ActorLevel = 1.f;
};
