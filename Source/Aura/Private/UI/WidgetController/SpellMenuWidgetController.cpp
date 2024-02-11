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

void USpellMenuWidgetController::SpellGlobeSelected( const FGameplayTag& AbilityTag )
{
	const bool bAbilityTagValid = AbilityTag.IsValid();
	const bool bNoneTag = AbilityTag.MatchesTag( FAuraGameplayTags::Get().Abilities_None );
	const FGameplayAbilitySpec* AbilitySpec = GetAuraAbilitySystemComponent()->GetSpecFromAbilityTag( AbilityTag );
	const bool bAbilitySpecValid = AbilitySpec != nullptr;

	// Check for inappropriate data
	if (!bAbilityTagValid || bNoneTag || !bAbilitySpecValid)
	{
		EnableButtons.Broadcast( false, false );
		return;
	}

	const FGameplayTag AbilityStatus = GetAuraAbilitySystemComponent()->GetStatusTagFromSpec( *AbilitySpec );
	const int32 SpellPoints = GetAuraPlayerState()->GetSpellPoints();

	ShouldEnableButtons( AbilityStatus, SpellPoints );
}

void USpellMenuWidgetController::ShouldEnableButtons( const FGameplayTag& StatusTag, const int32 SpellPoints ) const 
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	// Check if player has any available spell points
	const bool bSpellPointsAvailable = SpellPoints > 0;

	// Get Ability Status Tag

	// Player can spend points only if both are true:
	// 1) Has >0 spell points available
	// 2) Ability status is not Locked
	const bool bSpendPointsEnabled = bSpellPointsAvailable && !StatusTag.MatchesTagExact( GameplayTags.Abilities_Status_Locked );

	// Player can equip ability if Ability status is Unlocked or Equipped
	const bool bEquipEnabled = StatusTag.MatchesTagExact( GameplayTags.Abilities_Status_Unlocked ) ||
		StatusTag.MatchesTagExact( GameplayTags.Abilities_Status_Equipped );

	EnableButtons.Broadcast( bSpendPointsEnabled, bEquipEnabled );
}
