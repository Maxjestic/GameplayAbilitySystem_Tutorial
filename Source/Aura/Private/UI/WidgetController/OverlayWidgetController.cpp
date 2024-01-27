// Copyright Maxjestic


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>( AttributeSet );

	OnHealthChanged.Broadcast( AuraAttributeSet->GetHealth() );
	OnMaxHealthChanged.Broadcast( AuraAttributeSet->GetMaxHealth() );
	OnManaChanged.Broadcast( AuraAttributeSet->GetMana() );
	OnMaxManaChanged.Broadcast( AuraAttributeSet->GetMaxMana() );
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>( AttributeSet );

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetHealthAttribute() ).AddLambda(
		[this]( const FOnAttributeChangeData& Data )
		{
			OnHealthChanged.Broadcast( Data.NewValue );
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetMaxHealthAttribute() ).AddLambda(
		[this]( const FOnAttributeChangeData& Data )
		{
			OnMaxHealthChanged.Broadcast( Data.NewValue );
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetManaAttribute() ).AddLambda(
		[this]( const FOnAttributeChangeData& Data )
		{
			OnManaChanged.Broadcast( Data.NewValue );
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetMaxManaAttribute() ).AddLambda(
		[this]( const FOnAttributeChangeData& Data )
		{
			OnMaxManaChanged.Broadcast( Data.NewValue );
		}
	);

	if (UAuraAbilitySystemComponent* AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>( AbilitySystemComponent ))
	{
		AuraAbilitySystemComponent->EffectAssetTagsDelegate.AddLambda(
			[this]( const FGameplayTagContainer& AssetTags )
			{
				/** We want to display a message when player picks up something.
				 *  Here we go through GameplayTags associated with applied Effect
				 *  and look for Tag of "Message" Hierarchy, if we find it we broadcast
				 *  it so Overlay Widget can display correct message to the screen */
				for (const FGameplayTag Tag : AssetTags)
				{
					// For example, say that Tag = Message.HealthPotion				
					// "Message.HealthPotion".MatchesTag("Message") will return True, "Message".MatchesTag("Message.HealthPotion") will return False
					if (FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag( FName( "Message" ) ); Tag.MatchesTag( MessageTag ))
					{
						const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>( MessageWidgetDataTable, Tag );
						MessageWidgetRow.Broadcast( *Row );
					}
				}
			}
		);

		// Check if Startup Abilities have been given before binding to the delegate
		if (AuraAbilitySystemComponent->bStartupAbilitiesGiven)
		{
			OnInitializeStartupAbilities( AuraAbilitySystemComponent );
		}
		else
		{
			AuraAbilitySystemComponent->AbilitiesGivenDelegate.AddUObject( this, &UOverlayWidgetController::OnInitializeStartupAbilities );
		}
	}
}

void UOverlayWidgetController::OnInitializeStartupAbilities( UAuraAbilitySystemComponent* AuraAbilitySystemComponent )
{
	//TODO: Get information about all given abilities, look up their Ability Info and broadcast it to widgets.
	if (!AuraAbilitySystemComponent->bStartupAbilitiesGiven)
	{
		return;
	}

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda(
		[this, AuraAbilitySystemComponent]( const FGameplayAbilitySpec& AbilitySpec )
		{
			FAuraAbilityInfo Info = AbilityInfo->FindAbilityForTag( AuraAbilitySystemComponent->GetAbilityTagFromSpec( AbilitySpec ) );
			
			// InputTag is not set in editor, it is dynamic so we have to get it and assign
			Info.InputTag = AuraAbilitySystemComponent->GetInputTagFromSpec( AbilitySpec );
			AbilityInfoDelegate.Broadcast( Info );
		} );

	AuraAbilitySystemComponent->ForEachAbility( BroadcastDelegate );
}
