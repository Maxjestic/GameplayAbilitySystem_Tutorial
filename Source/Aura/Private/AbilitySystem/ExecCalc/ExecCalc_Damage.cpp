// Copyright Maxjestic


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"

/**
 * Stores all needed Attributes, internal to cpp file
 */
struct AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF( Armor );
	DECLARE_ATTRIBUTE_CAPTUREDEF( ArmorPenetration );
	DECLARE_ATTRIBUTE_CAPTUREDEF( BlockChance );

	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet, Armor, Target, false )
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet, ArmorPenetration, Source, false )
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet, BlockChance, Target, false )
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
}

void UExecCalc_Damage::Execute_Implementation( const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                               FGameplayEffectCustomExecutionOutput& OutExecutionOutput ) const
{
	const UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();

	const AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->GetAvatarActor() : nullptr;
	const AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor() : nullptr;

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

	// Source's Armor Penetration ignores a percentage of the Target's Armor
	const float EffectiveArmor = TargetArmor *= (100 - SourceArmorPenetration * 0.25f) * 0.01f;

	// Target's Armor ignores a percentage of incoming Damage
	Damage *= (100 - EffectiveArmor * 0.333f) * 0.01f;

	const FGameplayModifierEvaluatedData EvaluatedData( UAuraAttributeSet::GetIncomingDamageAttribute(),
	                                                    EGameplayModOp::Additive,
	                                                    Damage );
	
	OutExecutionOutput.AddOutputModifier( EvaluatedData );
}
