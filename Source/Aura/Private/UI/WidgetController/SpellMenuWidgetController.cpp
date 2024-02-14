// Copyright Maxjestic


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AuraGameplayTags.h"
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

			CurrentSpellPoints = NewSpellPoints;
			
			bool bSpendPointsEnabled = false;
			bool bEquipEnabled = false;
			ShouldEnableButtons( SelectedAbility.StatusTag, CurrentSpellPoints, bSpendPointsEnabled, bEquipEnabled );

			FString Description = FString();
			FString NextLevelDescription = FString();
			GetAuraAbilitySystemComponent()->GetDescriptionsByAbilityTag( SelectedAbility.AbilityTag, Description, NextLevelDescription );
			
			SpellGlobeSelectedDelegate.Broadcast( bSpendPointsEnabled, bEquipEnabled, Description, NextLevelDescription);
		} );

	GetAuraAbilitySystemComponent()->AbilityStatusChanged.AddLambda(
		[this]( const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const int32 AbilityLevel )
		{
			if (SelectedAbility.AbilityTag.MatchesTagExact( AbilityTag ))
			{
				SelectedAbility.StatusTag = StatusTag;
				
				bool bSpendPointsEnabled = false;
				bool bEquipEnabled = false;
				ShouldEnableButtons( StatusTag, CurrentSpellPoints, bSpendPointsEnabled, bEquipEnabled );

				FString Description = FString();
				FString NextLevelDescription = FString();
				GetAuraAbilitySystemComponent()->GetDescriptionsByAbilityTag( AbilityTag, Description, NextLevelDescription );
				
				SpellGlobeSelectedDelegate.Broadcast( bSpendPointsEnabled, bEquipEnabled, Description, NextLevelDescription);
			}

			if (AbilityInfo)
			{
				FAuraAbilityInfo Info = AbilityInfo->FindAbilityForTag( AbilityTag );
				Info.StatusTag = StatusTag;
				AbilityInfoDelegate.Broadcast( Info );
			}
		} );
}

void USpellMenuWidgetController::SpellGlobeSelected( const FGameplayTag& AbilityTag )
{
	const bool bAbilityTagValid = AbilityTag.IsValid();
	const bool bNoneTag = AbilityTag.MatchesTag( FAuraGameplayTags::Get().Abilities_None );
	const FGameplayAbilitySpec* AbilitySpec = GetAuraAbilitySystemComponent()->GetSpecFromAbilityTag( AbilityTag );
	const bool bAbilitySpecValid = AbilitySpec != nullptr;
	FGameplayTag AbilityStatus;
	// Check for inappropriate data
	if (!bAbilityTagValid || bNoneTag || !bAbilitySpecValid)
	{
		AbilityStatus = FAuraGameplayTags::Get().Abilities_Status_Locked;
	}
	else
	{
		AbilityStatus = GetAuraAbilitySystemComponent()->GetStatusTagFromSpec( *AbilitySpec );
	}

	const int32 SpellPoints = GetAuraPlayerState()->GetSpellPoints();

	SelectedAbility.AbilityTag = AbilityTag;
	SelectedAbility.StatusTag = AbilityStatus;
	bool bSpendPointsEnabled = false;
	bool bEquipEnabled = false;

	ShouldEnableButtons( AbilityStatus, SpellPoints, bSpendPointsEnabled, bEquipEnabled );
	
	FString Description = FString();
	FString NextLevelDescription = FString();
	GetAuraAbilitySystemComponent()->GetDescriptionsByAbilityTag( AbilityTag, Description, NextLevelDescription );
				
	SpellGlobeSelectedDelegate.Broadcast( bSpendPointsEnabled, bEquipEnabled, Description, NextLevelDescription);
}

void USpellMenuWidgetController::SpendPointButtonPressed()
{
	GetAuraAbilitySystemComponent()->ServerSpendSpellPoint( SelectedAbility.AbilityTag );
}

void USpellMenuWidgetController::ShouldEnableButtons( const FGameplayTag& StatusTag, const int32 SpellPoints, bool& bOutSpendPointsEnabled, bool& bOutEquipEnabled ) const
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	// Check if player has any available spell points
	const bool bSpellPointsAvailable = SpellPoints > 0;

	// Get Ability Status Tag

	// Player can spend points only if both are true:
	// 1) Has >0 spell points available
	// 2) Ability status is not Locked
	bOutSpendPointsEnabled = bSpellPointsAvailable && !StatusTag.MatchesTagExact( GameplayTags.Abilities_Status_Locked );

	// Player can equip ability if Ability status is Unlocked or Equipped
	bOutEquipEnabled = StatusTag.MatchesTagExact( GameplayTags.Abilities_Status_Unlocked ) ||
		StatusTag.MatchesTagExact( GameplayTags.Abilities_Status_Equipped );
}
