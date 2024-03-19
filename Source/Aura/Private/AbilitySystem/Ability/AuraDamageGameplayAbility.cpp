// Copyright Maxjestic


#include "AbilitySystem/Ability/AuraDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UAuraDamageGameplayAbility::CauseDamage( AActor* TargetActor )
{
	const FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec( DamageEffectClass, 1.f );
	const float ScaledDamage = Damage.GetValueAtLevel( GetAbilityLevel() );
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude( DamageSpecHandle, DamageType, ScaledDamage );
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget( *DamageSpecHandle.Data.Get(),
	                                                                           UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(
		                                                                           TargetActor ) );
}

FDamageEffectParams UAuraDamageGameplayAbility::MakeDamageEffectParamsFromClassDefaults( AActor* TargetActor, const FVector InRadialDamageOrigin ) const
{
	FDamageEffectParams Params;
	Params.WorldContextObject = GetAvatarActorFromActorInfo();
	Params.DamageGameplayEffectClass = DamageEffectClass;
	Params.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	Params.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent( TargetActor );
	Params.BaseDamage = Damage.GetValueAtLevel( GetAbilityLevel() );
	Params.AbilityLevel = GetAbilityLevel();
	Params.DamageType = DamageType;
	Params.DebuffChance = DebuffChance;
	Params.DebuffDamage = DebuffDamage;
	Params.DebuffDuration = DebuffDuration;
	Params.DebuffFrequency = DebuffFrequency;
	Params.DeathImpulseMagnitude = DeathImpulseMagnitude;
	Params.KnockbackChance = KnockbackChance;
	Params.KnockbackForceMagnitude = KnockbackForceMagnitude;
	if (IsValid( TargetActor ))
	{
		FRotator Rotation = (TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation();
		Rotation.Pitch = 45.f;

		const FVector ToTarget = Rotation.Vector();
		Params.DeathImpulse = ToTarget * DeathImpulseMagnitude;
		Params.KnockbackForce = ToTarget * KnockbackForceMagnitude;
	}
	if(bIsRadialDamage)
	{		
		Params.bIsRadialDamage = bIsRadialDamage;
		Params.RadialDamageInnerRadius = RadialDamageInnerRadius;
		Params.RadialDamageOuterRadius = RadialDamageOuterRadius;
		Params.RadialDamageOrigin = InRadialDamageOrigin;
	}
	return Params;
}

float UAuraDamageGameplayAbility::GetDamageAtLevel() const
{
	return Damage.GetValueAtLevel( GetAbilityLevel() );
}

FTaggedMontage UAuraDamageGameplayAbility::GetRandomTaggedMontageFromArray( const TArray<FTaggedMontage>& TaggedMontages ) const
{
	if (TaggedMontages.Num() <= 0)
	{
		return FTaggedMontage();
	}
	const int32 Selection = FMath::RandRange( 0, TaggedMontages.Num() - 1 );
	return TaggedMontages[Selection];
}
