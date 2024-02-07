// Copyright Maxjestic


#include "UI/WidgetController/AuraWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"

void UAuraWidgetController::SetWidgetControllerParams( const FWidgetControllerParams& WidgetControllerParams )
{
	PlayerController = WidgetControllerParams.PlayerController;
	PlayerState = WidgetControllerParams.PlayerState;
	AbilitySystemComponent = WidgetControllerParams.AbilitySystemComponent;
	AttributeSet = WidgetControllerParams.AttributeSet;
}

void UAuraWidgetController::BroadcastInitialValues()
{
}

void UAuraWidgetController::BindCallbacksToDependencies()
{
}

void UAuraWidgetController::BroadcastAbilityInfo()
{
	if (!GetAuraAbilitySystemComponent()->bStartupAbilitiesGiven)
	{
		return;
	}

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda(
		[this]( const FGameplayAbilitySpec& AbilitySpec )
		{
			FAuraAbilityInfo Info = AbilityInfo->FindAbilityForTag( GetAuraAbilitySystemComponent()->GetAbilityTagFromSpec( AbilitySpec ) );

			// InputTag is not set in editor, it is dynamic so we have to get it and assign
			Info.InputTag = GetAuraAbilitySystemComponent()->GetInputTagFromSpec( AbilitySpec );
			AbilityInfoDelegate.Broadcast( Info );
		} );

	GetAuraAbilitySystemComponent()->ForEachAbility( BroadcastDelegate );
}

AAuraPlayerController* UAuraWidgetController::GetAuraPlayerController()
{
	if (AuraPlayerController == nullptr)
	{
		AuraPlayerController = Cast<AAuraPlayerController>( PlayerController );
	}
	return AuraPlayerController;
}

AAuraPlayerState* UAuraWidgetController::GetAuraPlayerState()
{
	if (AuraPlayerState == nullptr)
	{
		AuraPlayerState = Cast<AAuraPlayerState>( PlayerState );
	}
	return AuraPlayerState;
}

UAuraAbilitySystemComponent* UAuraWidgetController::GetAuraAbilitySystemComponent()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>( AbilitySystemComponent );
	}
	return AuraAbilitySystemComponent;
}

UAuraAttributeSet* UAuraWidgetController::GetAuraAttributeSet()
{
	if (AuraAttributeSet == nullptr)
	{
		AuraAttributeSet = Cast<UAuraAttributeSet>( AttributeSet );
	}
	return AuraAttributeSet;
}
