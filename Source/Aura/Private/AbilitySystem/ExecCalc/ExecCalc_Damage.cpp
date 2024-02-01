// Copyright Maxjestic


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
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
	/**
	 * Declaration of attributes to capture
	 */
	DECLARE_ATTRIBUTE_CAPTUREDEF( Armor );
	DECLARE_ATTRIBUTE_CAPTUREDEF( ArmorPenetration );
	DECLARE_ATTRIBUTE_CAPTUREDEF( BlockChance );
	DECLARE_ATTRIBUTE_CAPTUREDEF( CriticalHitChance );
	DECLARE_ATTRIBUTE_CAPTUREDEF( CriticalHitDamage );
	DECLARE_ATTRIBUTE_CAPTUREDEF( CriticalHitResistance );

	DECLARE_ATTRIBUTE_CAPTUREDEF( ArcaneResistance );
	DECLARE_ATTRIBUTE_CAPTUREDEF( FireResistance );
	DECLARE_ATTRIBUTE_CAPTUREDEF( LightningResistance );
	DECLARE_ATTRIBUTE_CAPTUREDEF( PhysicalResistance );

	/** Map needed to assign Resistance Tags to Captured Attributes */
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;

	/**
	 * Defines Attributes to capture
	 */
	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet, Armor, Target, false )
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet, ArmorPenetration, Source, false )
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet, BlockChance, Target, false )
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet, CriticalHitChance, Source, false )
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet, CriticalHitDamage, Source, false )
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet, CriticalHitResistance, Target, false )

		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet, ArcaneResistance, Target, false )
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet, FireResistance, Target, false )
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet, LightningResistance, Target, false )
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet, PhysicalResistance, Target, false )

		const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();
		TagsToCaptureDefs.Add( Tags.Attributes_Secondary_Armor, ArmorDef );
		TagsToCaptureDefs.Add( Tags.Attributes_Secondary_ArmorPenetration, ArmorPenetrationDef );
		TagsToCaptureDefs.Add( Tags.Attributes_Secondary_BlockChance, BlockChanceDef );
		TagsToCaptureDefs.Add( Tags.Attributes_Secondary_CriticalHitChance, CriticalHitChanceDef );
		TagsToCaptureDefs.Add( Tags.Attributes_Secondary_CriticalHitDamage, CriticalHitDamageDef );
		TagsToCaptureDefs.Add( Tags.Attributes_Secondary_CriticalHitResistance, CriticalHitResistanceDef );

		TagsToCaptureDefs.Add( Tags.Attributes_Resistance_Arcane, ArcaneResistanceDef );
		TagsToCaptureDefs.Add( Tags.Attributes_Resistance_Fire, FireResistanceDef );
		TagsToCaptureDefs.Add( Tags.Attributes_Resistance_Lightning, LightningResistanceDef );
		TagsToCaptureDefs.Add( Tags.Attributes_Resistance_Physical, PhysicalResistanceDef );
	}
};

/** Creates and returns reference to static AuraDamageStatics */
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

	RelevantAttributesToCapture.Add( DamageStatics().ArcaneResistanceDef );
	RelevantAttributesToCapture.Add( DamageStatics().FireResistanceDef );
	RelevantAttributesToCapture.Add( DamageStatics().LightningResistanceDef );
	RelevantAttributesToCapture.Add( DamageStatics().PhysicalResistanceDef );
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
	int32 SourceLevel = 1;
	if (SourceAvatar->Implements<UCombatInterface>())
	{
		SourceLevel = ICombatInterface::Execute_GetCharacterLevel( SourceAvatar );
	}

	int32 TargetLevel = 1;
	if (TargetAvatar->Implements<UCombatInterface>())
	{
		TargetLevel = ICombatInterface::Execute_GetCharacterLevel( TargetAvatar );
	}

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	// Get Damage Set by Caller Magnitude
	float Damage = 0.f;
	for (const TTuple<FGameplayTag, FGameplayTag>& Pair : FAuraGameplayTags::Get().DamageTypesToResistances)
	{
		const FGameplayTag ResistanceTag = Pair.Value;
		checkf( AuraDamageStatics().TagsToCaptureDefs.Contains( ResistanceTag ),
		        TEXT("TagsToCaptureDefs doesn't contain Tag: [%s] in ExecCalc_Damage."),
		        *ResistanceTag.ToString() );

		const FGameplayTag DamageTypeTag = Pair.Key;
		const FGameplayEffectAttributeCaptureDefinition CaptureDef = AuraDamageStatics().TagsToCaptureDefs[ResistanceTag];

		float DamageTypeValue = Spec.GetSetByCallerMagnitude( DamageTypeTag, false );

		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude( CaptureDef, EvaluateParameters, Resistance );
		Resistance = FMath::Clamp( Resistance, 0.f, 100.f );

		DamageTypeValue *= (100.f - Resistance) * 0.01f;

		Damage += DamageTypeValue;
	}

	// Capture BlockChance on Target, and determine if there was a successful Block
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude( DamageStatics().BlockChanceDef,
	                                                            EvaluateParameters,
	                                                            TargetBlockChance );

	TargetBlockChance = FMath::Max<float>( 0.f, TargetBlockChance );

	// Determine if there was a successful Block
	const bool bBlocked = FMath::RandRange( 1, 100 ) < TargetBlockChance;

	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
	UAuraAbilitySystemLibrary::SetIsBlockedHit( EffectContextHandle, bBlocked );

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
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval( SourceLevel );

	// Source's Armor Penetration ignores a percentage of the Target's Armor
	const float EffectiveArmor = TargetArmor * (100 - SourceArmorPenetration * ArmorPenetrationCoefficient) * 0.01f;

	// Get Effective Armor Coefficient
	const FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(
		FName( "EffectiveArmor" ),
		FString() );
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval( TargetLevel );

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
	const float CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval( TargetLevel );

	// Critical Hit Resistance reduces Critical Hit Chance by a certain percentage
	const float EffectiveCriticalHitChance = SourceCriticalHitChance - (TargetCriticalHitResistance * CriticalHitResistanceCoefficient);

	// Determine if there was successful Critical Hit
	const bool bCriticalHit = FMath::RandRange( 1, 100 ) < EffectiveCriticalHitChance;
	UAuraAbilitySystemLibrary::SetIsCriticalHit( EffectContextHandle, bCriticalHit );

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
