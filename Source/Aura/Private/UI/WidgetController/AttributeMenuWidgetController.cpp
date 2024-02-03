// Copyright Maxjestic


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Player/AuraPlayerState.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	check( AttributeInfo )

	UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>( AttributeSet );
	for (auto& Pair : AuraAttributeSet->TagsToAttributes)
	{
		BroadcastAttributeInfo( Pair.Key, Pair.Value() );
	}

	const AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>( PlayerState );
	AttributePointsChangedDelegate.Broadcast( AuraPlayerState->GetAttributePoints() );
	SpellPointsChangedDelegate.Broadcast( AuraPlayerState->GetSpellPoints() );
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>( PlayerState );
	
	AuraPlayerState->OnAttributePointsChangedDelegate.AddLambda(
		[this](const int32 NewAttributePoints)
		{
			AttributePointsChangedDelegate.Broadcast( NewAttributePoints );
		} );
	
	AuraPlayerState->OnSpellPointsChangedDelegate.AddLambda(
		[this](const int32 NewSpellPoints)
		{
			SpellPointsChangedDelegate.Broadcast( NewSpellPoints );
		} );
	
	check( AttributeInfo )

	UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>( AttributeSet );
	for (auto& Pair : AuraAttributeSet->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate( Pair.Value() ).AddLambda(
			[this, Pair]( const FOnAttributeChangeData& Data )
			{
				BroadcastAttributeInfo( Pair.Key, Pair.Value() );
			}
		);
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo( const FGameplayTag& AttributeTag,
                                                             const FGameplayAttribute& Attribute ) const
{
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag( AttributeTag );
	Info.AttributeValue = Attribute.GetNumericValue( AttributeSet );
	AttributeInfoDelegate.Broadcast( Info );
}
