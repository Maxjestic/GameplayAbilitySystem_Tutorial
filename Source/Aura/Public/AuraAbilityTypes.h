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

	UPROPERTY( BlueprintReadWrite )
	TObjectPtr<UObject> WorldContextObject = nullptr;

	UPROPERTY( BlueprintReadWrite )
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;

	UPROPERTY( BlueprintReadWrite )
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent;

	UPROPERTY( BlueprintReadWrite )
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent;

	UPROPERTY( BlueprintReadWrite )
	float BaseDamage = 0.f;

	UPROPERTY( BlueprintReadWrite )
	float AbilityLevel = 1.f;

	UPROPERTY( BlueprintReadWrite )
	FGameplayTag DamageType = FGameplayTag();

	UPROPERTY( BlueprintReadWrite )
	float DebuffChance = 0.f;

	UPROPERTY( BlueprintReadWrite )
	float DebuffDamage = 0.f;

	UPROPERTY( BlueprintReadWrite )
	float DebuffDuration = 0.f;

	UPROPERTY( BlueprintReadWrite )
	float DebuffFrequency = 0.f;

	UPROPERTY( BlueprintReadWrite )
	float DeathImpulseMagnitude = 0.f;

	UPROPERTY( BlueprintReadWrite )
	FVector DeathImpulse = FVector::ZeroVector;

	UPROPERTY( BlueprintReadWrite )
	float KnockbackChance = 0.f;

	UPROPERTY( BlueprintReadWrite )
	float KnockbackForceMagnitude = 0.f;

	UPROPERTY( BlueprintReadWrite )
	FVector KnockbackForce = FVector::ZeroVector;

	UPROPERTY( BlueprintReadWrite )
	bool bIsRadialDamage = false;

	UPROPERTY( BlueprintReadWrite )
	float RadialDamageInnerRadius = 0.f;

	UPROPERTY( BlueprintReadWrite )
	float RadialDamageOuterRadius = 0.f;

	UPROPERTY( BlueprintReadWrite )
	FVector RadialDamageOrigin = FVector::ZeroVector;
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
	 * Simple Accessors to Damage Effect related data
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

	FVector GetDeathImpulse() const { return DeathImpulse; }
	void SetDeathImpulse( const FVector& InDeathImpulse ) { DeathImpulse = InDeathImpulse; }

	FVector GetKnockbackForce() const { return KnockbackForce; }
	void SetKnockbackForce( const FVector& InKnockbackForce ) { KnockbackForce = InKnockbackForce; }

	bool IsRadialDamage() const { return bIsRadialDamage; }
	void SetIsRadialDamage( const bool bInIsRadialDamage ) { bIsRadialDamage = bInIsRadialDamage; }

	float GetRadialDamageInnerRadius() const { return RadialDamageInnerRadius; }
	void SetRadialDamageInnerRadius( const float InRadialDamageInnerRadius ) { RadialDamageInnerRadius = InRadialDamageInnerRadius; }

	float GetRadialDamageOuterRadius() const { return RadialDamageOuterRadius; }
	void SetRadialDamageOuterRadius( const float InRadialDamageOuterRadius ) { RadialDamageOuterRadius = InRadialDamageOuterRadius; }

	FVector GetRadialDamageOrigin() const { return RadialDamageOrigin; }
	void SetRadialDamageOrigin( const FVector& InRadialDamageOrigin ) { RadialDamageOrigin = InRadialDamageOrigin; }

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

	/** Direction of force applied to target on its death */
	UPROPERTY()
	FVector DeathImpulse = FVector::ZeroVector;

	/** Direction of force applied to target on knockback */
	UPROPERTY()
	FVector KnockbackForce = FVector::ZeroVector;

	/** Indicates if damage is radial */
	UPROPERTY()
	bool bIsRadialDamage = false;

	/** Inner radius of radial damage */
	UPROPERTY()
	float RadialDamageInnerRadius = 0.f;

	/** Outer radius of radial damage */
	UPROPERTY()
	float RadialDamageOuterRadius = 0.f;

	/** Origin of radial damage */
	UPROPERTY()
	FVector RadialDamageOrigin = FVector::ZeroVector;
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
