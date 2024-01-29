// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "WaitCooldownChange.generated.h"

struct FGameplayEffectSpec;
class UAbilitySystemComponent;

/** Broadcasts cooldown duration */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FCooldownChangeSignature, float, TimeRemaining );

/**
 * Waits for cooldown
 * Informs when cooldown begins and ends
 */
UCLASS( BlueprintType, meta = (ExposedAsyncProxy = "AsyncTask") )
class AURA_API UWaitCooldownChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	/** Executed when cooldown starts */
	UPROPERTY( BlueprintAssignable )
	FCooldownChangeSignature CooldownStart;

	/** Executed when cooldown ends */
	UPROPERTY( BlueprintAssignable )
	FCooldownChangeSignature CooldownEnd;

	/** Factory that construct this */
	UFUNCTION( BlueprintCallable, meta = (BlueprintInternalUseOnly = "true") )
	static UWaitCooldownChange* WaitForCooldownChange( UAbilitySystemComponent* InAbilitySystemComponent,
	                                                   const FGameplayTag& InCooldownTag );

	/** Cleans up any resources when task is done, widget destroyed, ability system component or gameplay tag are not valid */
	UFUNCTION( BlueprintCallable )
	void EndTask();

protected:
	/** Stores received ability system component */
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	/** Stores received gameplay tag */
	FGameplayTag CooldownTag;

	/** Responses to OnActiveGameplayEffectAddedDelegateToSelf */
	void OnActiveEffectAdded( UAbilitySystemComponent* InAbilitySystemComponent, const FGameplayEffectSpec& AppliedSpec,
	                          FActiveGameplayEffectHandle ActiveEffectHandle );

	/** Responses to cooldown gameplay tag change event */
	void CooldownTagChanged( const FGameplayTag InCooldownTag, int32 NewCount );
};
