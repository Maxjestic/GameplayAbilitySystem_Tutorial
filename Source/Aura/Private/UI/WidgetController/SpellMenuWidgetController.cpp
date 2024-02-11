// Copyright Maxjestic


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerState.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();

	SpellPointsChangedDelegate.Broadcast( GetAuraPlayerState()->GetSpellPoints() );
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	GetAuraPlayerState()->OnSpellPointsChangedDelegate.AddLambda(
		[this]( const int32 NewSpellPoints )
		{
			SpellPointsChangedDelegate.Broadcast( NewSpellPoints );
		} );

	GetAuraAbilitySystemComponent()->AbilityStatusChanged.AddLambda(
		[this]( const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag )
		{
			if (AbilityInfo)
			{
				FAuraAbilityInfo Info = AbilityInfo->FindAbilityForTag( AbilityTag );
				Info.StatusTag = StatusTag;
				AbilityInfoDelegate.Broadcast( Info );
			}
		} );
}
