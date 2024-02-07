// Copyright Maxjestic


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Player/AuraPlayerState.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	check( AttributeInfo )

	for (auto& Pair : GetAuraAttributeSet()->TagsToAttributes)
	{
		BroadcastAttributeInfo( Pair.Key, Pair.Value() );
	}

	AttributePointsChangedDelegate.Broadcast( GetAuraPlayerState()->GetAttributePoints() );
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	check( AttributeInfo )
	
	GetAuraPlayerState()->OnAttributePointsChangedDelegate.AddLambda(
		[this]( const int32 NewAttributePoints )
		{
			AttributePointsChangedDelegate.Broadcast( NewAttributePoints );
		} );

	for (auto& Pair : GetAuraAttributeSet()->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate( Pair.Value() ).AddLambda(
			[this, Pair]( const FOnAttributeChangeData& Data )
			{
				BroadcastAttributeInfo( Pair.Key, Pair.Value() );
			}
		);
	}
}

void UAttributeMenuWidgetController::UpgradeAttribute( const FGameplayTag& AttributeTag )
{
	GetAuraAbilitySystemComponent()->UpgradeAttribute( AttributeTag );
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo( const FGameplayTag& AttributeTag,
                                                             const FGameplayAttribute& Attribute ) const
{
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag( AttributeTag );
	Info.AttributeValue = Attribute.GetNumericValue( AttributeSet );
	AttributeInfoDelegate.Broadcast( Info );
}
