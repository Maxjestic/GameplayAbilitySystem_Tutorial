// Copyright Maxjestic


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/AuraPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	OnHealthChanged.Broadcast( GetAuraAttributeSet()->GetHealth() );
	OnMaxHealthChanged.Broadcast( GetAuraAttributeSet()->GetMaxHealth() );
	OnManaChanged.Broadcast( GetAuraAttributeSet()->GetMana() );
	OnMaxManaChanged.Broadcast( GetAuraAttributeSet()->GetMaxMana() );
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	GetAuraPlayerState()->OnExperienceChangedDelegate.AddUObject( this, &UOverlayWidgetController::OnExperienceChanged );
	GetAuraPlayerState()->OnLevelChangedDelegate.AddLambda(
		[this](const int32 NewLevel)
		{
			OnPlayerLevelChangedDelegate.Broadcast( NewLevel );
		} );

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAuraAttributeSet()->GetHealthAttribute() ).AddLambda(
		[this]( const FOnAttributeChangeData& Data )
		{
			OnHealthChanged.Broadcast( Data.NewValue );
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAuraAttributeSet()->GetMaxHealthAttribute() ).AddLambda(
		[this]( const FOnAttributeChangeData& Data )
		{
			OnMaxHealthChanged.Broadcast( Data.NewValue );
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAuraAttributeSet()->GetManaAttribute() ).AddLambda(
		[this]( const FOnAttributeChangeData& Data )
		{
			OnManaChanged.Broadcast( Data.NewValue );
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAuraAttributeSet()->GetMaxManaAttribute() ).AddLambda(
		[this]( const FOnAttributeChangeData& Data )
		{
			OnMaxManaChanged.Broadcast( Data.NewValue );
		}
	);

	if (GetAuraAbilitySystemComponent())
	{
		GetAuraAbilitySystemComponent()->EffectAssetTagsDelegate.AddLambda(
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
		if (GetAuraAbilitySystemComponent()->bStartupAbilitiesGiven)
		{
			BroadcastAbilityInfo();
		}
		else
		{
			GetAuraAbilitySystemComponent()->AbilitiesGivenDelegate.AddUObject( this, &UOverlayWidgetController::BroadcastAbilityInfo );
		}
	}
}

void UOverlayWidgetController::OnExperienceChanged( const int32 NewExperience ) 
{
	const ULevelUpInfo* LevelUpInfo = GetAuraPlayerState()->LevelUpInfo;

	checkf( LevelUpInfo, TEXT("Unable to find LevelUpInfo. Please fillout AuraPlayerState Blueprint") )

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
