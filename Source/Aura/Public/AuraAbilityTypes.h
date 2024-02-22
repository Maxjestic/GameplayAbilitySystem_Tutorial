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
	/** 
	 * Simple Accessors to Aura related data
	 */
	bool IsBlockedHit() const { return bIsBlockedHit; }
	void SetIsBlockedHit( bool const bInIsBlockedHit ) { bIsBlockedHit = bInIsBlockedHit; }
	
	bool IsCriticalHit() const { return bIsCriticalHit; }
	void SetIsCriticalHit( bool const bInIsCriticalHit ) { bIsCriticalHit = bInIsCriticalHit; }
	
	bool IsSuccessfulDebuff() const { return bIsSuccessfulDebuff; }
	void SetIsSuccessfulDebuff( const bool bInIsSuccessfulDebuff ) { bIsSuccessfulDebuff = bInIsSuccessfulDebuff; }
	
	float GetDebuffDamage() const { return DebuffDamage; }
	void SetDebuffDamage( const float InDebuffDamage ) { DebuffDamage = InDebuffDamage; }
	
	float GetDebuffDuration() const { return DebuffDuration; }
	void SetDebuffDuration( const float InDebuffDuration ) { DebuffDuration = InDebuffDuration; }
	
	float GetDebuffFrequency() const { return DebuffFrequency; }
	void SetDebuffFrequency( const float InDebuffFrequency ) { DebuffFrequency = InDebuffFrequency; }
	
	TSharedPtr<FGameplayTag> GetDamageType() const { return DamageType; }
	void SetDamageType( const TSharedPtr<FGameplayTag>& InDamageType ) { DamageType = InDamageType; }

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

	/** True if Hit was Critical */
	UPROPERTY()
	bool bIsCriticalHit = false;

	/** True if debuff was successful */
	UPROPERTY()
	bool bIsSuccessfulDebuff = false;

	/** Damage dealt by debuff */
	UPROPERTY()
	float DebuffDamage = 0.f;

	/** Duration of the debuff */
	UPROPERTY()
	float DebuffDuration = 0.f;

	/** Frequency of debuff application */
	UPROPERTY()
	float DebuffFrequency = 0.f;

	/** Type of the damage */
	TSharedPtr<FGameplayTag> DamageType = nullptr;
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
