#pragma once

#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "AuraAbilityTypes.generated.h"

class UGameplayEffect;

/**
 * Struct with info related to damage effect
 */
USTRUCT( BlueprintType )
struct FDamageEffectParams
{
	GENERATED_BODY()

	FDamageEffectParams()
	{
	}

	UPROPERTY()
	TObjectPtr<UObject> WorldContextObject = nullptr;

	UPROPERTY()
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent;

	UPROPERTY()
	float BaseDamage = 0.f;

	UPROPERTY()
	float AbilityLevel = 1.f;

	UPROPERTY()
	FGameplayTag DamageType = FGameplayTag();

	UPROPERTY()
	float DebuffChance = 0.f;

	UPROPERTY()
	float DebuffDamage = 0.f;

	UPROPERTY()
	float DebuffDuration = 0.f;

	UPROPERTY()
	float DebuffFrequency = 0.f;
};

/**
 * GameplayEffectContext with additional Aura related data
 */
USTRUCT( BlueprintType )
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	/** Simple accessors to Aura related data */
	bool IsBlockedHit() const
	{
		return bIsBlockedHit;
	}

	bool IsCriticalHit() const
	{
		return bIsCriticalHit;
	}

	void SetIsBlockedHit( bool const bInIsBlockedHit )
	{
		bIsBlockedHit = bInIsBlockedHit;
	}

	void SetIsCriticalHit( bool const bInIsCriticalHit )
	{
		bIsCriticalHit = bInIsCriticalHit;
	}

	//~ Begin FGameplayEffectContext Interface
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}

	virtual bool NetSerialize( FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess ) override;
	virtual FAuraGameplayEffectContext* Duplicate() const override;
	//~ End FGameplayEffectContext Interface

protected:
	/** True if Hit was Blocked */
	UPROPERTY()
	bool bIsBlockedHit = false;

	/** true if Hit was Critical */
	UPROPERTY()
	bool bIsCriticalHit = false;
};

template <>
struct TStructOpsTypeTraits<FAuraGameplayEffectContext> : TStructOpsTypeTraitsBase2<FAuraGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};
