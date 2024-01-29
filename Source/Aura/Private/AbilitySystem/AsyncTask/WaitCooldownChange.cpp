// Copyright Maxjestic


#include "AbilitySystem/AsyncTask/WaitCooldownChange.h"

#include "AbilitySystemComponent.h"
#include "Aura/AuraLogChannels.h"

UWaitCooldownChange* UWaitCooldownChange::WaitForCooldownChange( UAbilitySystemComponent* InAbilitySystemComponent,
                                                                 const FGameplayTag& InCooldownTag )
{
	UWaitCooldownChange* WaitCooldownChange = NewObject<UWaitCooldownChange>();
	WaitCooldownChange->AbilitySystemComponent = InAbilitySystemComponent;
	WaitCooldownChange->CooldownTag = InCooldownTag;

	if (!IsValid( InAbilitySystemComponent ) || !(InCooldownTag.IsValid()))
	{
		WaitCooldownChange->EndTask();
		return nullptr;
	}

	// To know when Cooldown Gameplay Effect has been applied - cooldown started
	InAbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(
		WaitCooldownChange,
		&UWaitCooldownChange::OnActiveEffectAdded );

	// To know when Cooldown Gameplay Tag has been remove - cooldown ended
	InAbilitySystemComponent->RegisterGameplayTagEvent( InCooldownTag, EGameplayTagEventType::NewOrRemoved ).AddUObject(
		WaitCooldownChange,
		&UWaitCooldownChange::CooldownTagChanged );

	return WaitCooldownChange;
}

void UWaitCooldownChange::EndTask()
{
	if (!IsValid( AbilitySystemComponent ))
	{
		return;
	}

	AbilitySystemComponent->RegisterGameplayTagEvent( CooldownTag, EGameplayTagEventType::NewOrRemoved ).RemoveAll( this );
	SetReadyToDestroy();
	MarkAsGarbage();
}

void UWaitCooldownChange::OnActiveEffectAdded( UAbilitySystemComponent* InAbilitySystemComponent, const FGameplayEffectSpec& AppliedSpec,
                                               FActiveGameplayEffectHandle ActiveEffectHandle )
{
	FGameplayTagContainer AssetTags;
	AppliedSpec.GetAllAssetTags( AssetTags );

	FGameplayTagContainer GrantedTags;
	AppliedSpec.GetAllAssetTags( GrantedTags );

	if (!AssetTags.HasTagExact( CooldownTag ) || !GrantedTags.HasTagExact( CooldownTag ))
	{
		return;
	}

	const FGameplayEffectQuery GameplayEffectQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(
		CooldownTag.GetSingleTagContainer() );
	TArray<float> TimesRemaining = AbilitySystemComponent->GetActiveEffectsTimeRemaining( GameplayEffectQuery );

	if (TimesRemaining.IsEmpty())
	{
		return;
	}

	float TimeRemaining = TimesRemaining[0];
	for (int32 i = 0; i < TimesRemaining.Num(); i++)
	{
		if (TimeRemaining < TimesRemaining[i])
		{
			TimeRemaining = TimesRemaining[i];
		}
	}

	CooldownStart.Broadcast( TimeRemaining );
}

void UWaitCooldownChange::CooldownTagChanged( const FGameplayTag InCooldownTag, int32 NewCount )
{
	if (NewCount == 0)
	{
		CooldownEnd.Broadcast( 0.f );
	}
}
