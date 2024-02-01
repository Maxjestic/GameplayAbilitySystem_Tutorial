// Copyright Maxjestic


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/AuraPlayerState.h"

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
	AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>( PlayerState );
	AuraPlayerState->OnExperienceChangedDelegate.AddUObject( this, &UOverlayWidgetController::OnExperienceChanged );

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

void UOverlayWidgetController::OnExperienceChanged( const int32 NewExperience ) const 
{
	const AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>( PlayerState );
	const ULevelUpInfo* LevelUpInfo = AuraPlayerState->LevelUpInfo;

	checkf( LevelUpInfo, TEXT("Unable to fin LevelUpInfo. Please fillout AuraPlayerState Blueprint") )

	const int32 Level = LevelUpInfo->FindLevelForExperience( NewExperience );
	const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num();

	if (Level <= 0 || Level > MaxLevel)
	{
		return;
	}

	const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
	const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformation[Level - 1].LevelUpRequirement;
	const int32 LevelUpRequirementDelta = LevelUpRequirement - PreviousLevelUpRequirement;
	const int32 ExperienceForNextLevel = NewExperience - PreviousLevelUpRequirement;
	
	const float ExperiencePercent = static_cast<float>( ExperienceForNextLevel) / static_cast<float>( LevelUpRequirementDelta);
	
	OnExperiencePercentChangedDelegate.Broadcast( ExperiencePercent );
}
