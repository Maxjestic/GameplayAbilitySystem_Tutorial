// Copyright Maxjestic


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"

/**
 * Stores all needed Attributes, internal to cpp file
 */
struct AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF( Armor );
	DECLARE_ATTRIBUTE_CAPTUREDEF( ArmorPenetration );
	DECLARE_ATTRIBUTE_CAPTUREDEF( BlockChance );
	DECLARE_ATTRIBUTE_CAPTUREDEF( CriticalHitChance );
	DECLARE_ATTRIBUTE_CAPTUREDEF( CriticalHitDamage );
	DECLARE_ATTRIBUTE_CAPTUREDEF( CriticalHitResistance );

	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet, Armor, Target, false )
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet, ArmorPenetration, Source, false )
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet, BlockChance, Target, false )
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet, CriticalHitChance, Source, false )
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet, CriticalHitDamage, Source, false )
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet, CriticalHitResistance, Target, false )
	}
};

static const AuraDamageStatics& DamageStatics()
{
	static AuraDamageStatics DamageStatics;
	return DamageStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add( DamageStatics().ArmorDef );
	RelevantAttributesToCapture.Add( DamageStatics().ArmorPenetrationDef );
	RelevantAttributesToCapture.Add( DamageStatics().BlockChanceDef );
	RelevantAttributesToCapture.Add( DamageStatics().CriticalHitChanceDef );
	RelevantAttributesToCapture.Add( DamageStatics().CriticalHitDamageDef );
	RelevantAttributesToCapture.Add( DamageStatics().CriticalHitResistanceDef );
}

void UExecCalc_Damage::Execute_Implementation( const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                               FGameplayEffectCustomExecutionOutput& OutExecutionOutput ) const
{
	const UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();

	const AActor* SourceAvatar = SourceAbilitySystemComponent
		                             ? SourceAbilitySystemComponent->GetAvatarActor()
		                             : nullptr;
	const AActor* TargetAvatar = TargetAbilitySystemComponent
		                             ? TargetAbilitySystemComponent->GetAvatarActor()
		                             : nullptr;

	const ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>( SourceAvatar );
	const ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>( TargetAvatar );

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	// Get Damage Set by Caller Magnitude
	float Damage = Spec.GetSetByCallerMagnitude( FAuraGameplayTags::Get().Damage );

	// Capture BlockChance on Target, and determine if there was a successful Block
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude( DamageStatics().BlockChanceDef,
	                                                            EvaluateParameters,
	                                                            TargetBlockChance );

	TargetBlockChance = FMath::Max<float>( 0.f, TargetBlockChance );

	// Determine if there was a successful Block
	const bool bBlocked = FMath::RandRange( 1, 100 ) < TargetBlockChance;

	// If Block was successful halve the Damage
	Damage = bBlocked ? Damage * 0.5f : Damage;

	// Capture Armor on Target
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude( DamageStatics().ArmorDef,
	                                                            EvaluateParameters,
	                                                            TargetArmor );

	TargetArmor = FMath::Max<float>( 0.f, TargetArmor );

	// Capture Armor Penetration on Source
	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude( DamageStatics().ArmorPenetrationDef,
	                                                            EvaluateParameters,
	                                                            SourceArmorPenetration );

	SourceArmorPenetration = FMath::Max<float>( 0.f, SourceArmorPenetration );

	const UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo( SourceAvatar );

	// Get Armor Penetration Coefficient
	const FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(
		FName( "ArmorPenetration" ),
		FString() );
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval( SourceCombatInterface->GetCharacterLevel() );

	// Source's Armor Penetration ignores a percentage of the Target's Armor
	const float EffectiveArmor = TargetArmor * (100 - SourceArmorPenetration * ArmorPenetrationCoefficient) * 0.01f;

	// Get Effective Armor Coefficient
	const FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(
		FName( "EffectiveArmor" ),
		FString() );
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval( TargetCombatInterface->GetCharacterLevel() );

	// Target's Armor ignores a percentage of incoming Damage
	Damage *= (100 - EffectiveArmor * EffectiveArmorCoefficient) * 0.01f;

	// Capture Critical Hit Resistance on Target
	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude( DamageStatics().CriticalHitResistanceDef,
	                                                            EvaluateParameters,
	                                                            TargetCriticalHitResistance );
	TargetCriticalHitResistance = FMath::Max<float>( 0.f, TargetCriticalHitResistance );

	// Capture Critical Hit Chance on Source
	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude( DamageStatics().CriticalHitChanceDef,
	                                                            EvaluateParameters,
	                                                            SourceCriticalHitChance );
	SourceCriticalHitChance = FMath::Max<float>( 0.f, SourceCriticalHitChance );

	// Get Critical Hit Resistance Coefficient
	const FRealCurve* CriticalHitResistanceCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(
		FName( "CriticalHitResistance" ),
		FString() );
	const float CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval( TargetCombatInterface->GetCharacterLevel() );

	// Critical Hit Resistance reduces Critical Hit Chance by a certain percentage
	const float EffectiveCriticalHitChance = SourceCriticalHitChance - (TargetCriticalHitResistance * CriticalHitResistanceCoefficient);

	// Determine if there was successful Critical Hit
	const bool bCriticalHit = FMath::RandRange( 1, 100 ) < EffectiveCriticalHitChance;

	if (bCriticalHit)
	{
		float SourceCriticalHitDamage = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude( DamageStatics().CriticalHitDamageDef,
		                                                            EvaluateParameters,
		                                                            SourceCriticalHitDamage );
		SourceCriticalHitDamage = FMath::Max<float>( 0.f, SourceCriticalHitDamage );

		// Double the Damage and add Critical Hit Damage
		Damage = Damage * 2.f + SourceCriticalHitDamage;
	}


	const FGameplayModifierEvaluatedData EvaluatedData( UAuraAttributeSet::GetIncomingDamageAttribute(),
	                                                    EGameplayModOp::Additive,
	                                                    Damage );

	OutExecutionOutput.AddOutputModifier( EvaluatedData );
}
