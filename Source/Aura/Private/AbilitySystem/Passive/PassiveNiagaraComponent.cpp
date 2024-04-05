// Copyright Maxjestic


#include "AbilitySystem/Passive/PassiveNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"

UPassiveNiagaraComponent::UPassiveNiagaraComponent()
{
	bAutoActivate = false;
}

void UPassiveNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UAuraAbilitySystemComponent* AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent( GetOwner() ) ))
	{
		AuraAbilitySystemComponent->ActivatePassiveEffect.AddUObject( this, &UPassiveNiagaraComponent::OnPassiveActivate );
		ActivateIfEquipped(AuraAbilitySystemComponent);
	}
	else if (ICombatInterface* CombatInterface = Cast<ICombatInterface>( GetOwner() ))
	{
		CombatInterface->GetOnAbilitySystemComponentRegistered().AddLambda( [this]( UAbilitySystemComponent* AbilitySystemComponent )
		{
			if (UAuraAbilitySystemComponent* AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(
				UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent( GetOwner() ) ))
			{
				AuraAbilitySystemComponent->ActivatePassiveEffect.AddUObject( this, &UPassiveNiagaraComponent::OnPassiveActivate );
				ActivateIfEquipped(AuraAbilitySystemComponent);
			}
		} );
	}
}

void UPassiveNiagaraComponent::OnPassiveActivate( const FGameplayTag& AbilityTag, const bool bActivate )
{
	if (AbilityTag.MatchesTagExact( PassiveSpellTag ))
	{
		if (bActivate && !IsActive())
		{
			Activate();
		}
		else
		{
			Deactivate();
		}
	}
}

void UPassiveNiagaraComponent::ActivateIfEquipped( UAuraAbilitySystemComponent* AuraAbilitySystemComponent )
{
	if (AuraAbilitySystemComponent->bStartupAbilitiesGiven)
	{
		if (AuraAbilitySystemComponent->GetStatusTagFromAbilityTag( PassiveSpellTag )
			== FAuraGameplayTags::Get().Abilities_Status_Equipped)
		{
			Activate();
		}
	}
}
