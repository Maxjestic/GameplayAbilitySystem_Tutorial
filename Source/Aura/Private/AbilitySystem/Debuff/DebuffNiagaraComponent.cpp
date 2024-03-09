// Copyright Maxjestic


#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"

UDebuffNiagaraComponent::UDebuffNiagaraComponent()
{
	bAutoActivate = false;
}

void UDebuffNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();
	ICombatInterface* CombatInterface = Cast<ICombatInterface>( GetOwner() );
	if (UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent( GetOwner() ))
	{
		AbilitySystemComponent->RegisterGameplayTagEvent( DebuffTag, EGameplayTagEventType::NewOrRemoved ).AddUObject(
			this,
			&UDebuffNiagaraComponent::DebuffTagChanged );
	}
	else if (CombatInterface)
	{
		CombatInterface->GetOnAbilitySystemComponentDelegate().AddWeakLambda( this,
		                                                                      [this]( UAbilitySystemComponent* InAbilitySystemComponent )
		                                                                      {
			                                                                      InAbilitySystemComponent->RegisterGameplayTagEvent(
				                                                                      DebuffTag,
				                                                                      EGameplayTagEventType::NewOrRemoved ).AddUObject(
				                                                                      this,
				                                                                      &UDebuffNiagaraComponent::DebuffTagChanged );
		                                                                      } );
	}
}

void UDebuffNiagaraComponent::DebuffTagChanged( const FGameplayTag CallbackTag, int32 NewCount )
{
	const bool bOwnerValid = IsValid( GetOwner() );
	const bool bOwnerAlive = GetOwner()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead( GetOwner() );
	if (NewCount > 0 && bOwnerValid && bOwnerAlive)
	{
		Activate();
		return;
	}
	Deactivate();
}
