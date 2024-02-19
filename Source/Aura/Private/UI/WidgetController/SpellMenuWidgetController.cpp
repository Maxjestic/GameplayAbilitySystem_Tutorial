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

			SpellGlobeSelectedDelegate.Broadcast( bSpendPointsEnabled, bEquipEnabled, Description, NextLevelDescription );
		} );

	GetAuraAbilitySystemComponent()->AbilityEquipped.AddUObject( this, &USpellMenuWidgetController::OnAbilityEquipped );
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

				SpellGlobeSelectedDelegate.Broadcast( bSpendPointsEnabled, bEquipEnabled, Description, NextLevelDescription );
			}

			if (AbilityInfo)
			{
				FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag( AbilityTag );
				Info.StatusTag = StatusTag;
				AbilityInfoDelegate.Broadcast( Info );
			}
		} );
}

void USpellMenuWidgetController::SpellGlobeSelected( const FGameplayTag& AbilityTag )
{
	if (bWaitingForEquipSelection)
	{
		const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag( AbilityTag ).AbilityTypeTag;
		StopWaitingForEquipDelegate.Broadcast( SelectedAbilityType );
		bWaitingForEquipSelection = false;
	}

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

	SpellGlobeSelectedDelegate.Broadcast( bSpendPointsEnabled, bEquipEnabled, Description, NextLevelDescription );
}

void USpellMenuWidgetController::SpendPointButtonPressed()
{
	GetAuraAbilitySystemComponent()->ServerSpendSpellPoint( SelectedAbility.AbilityTag );
}

void USpellMenuWidgetController::GlobeDeselect()
{
	if (bWaitingForEquipSelection)
	{
		const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag( SelectedAbility.AbilityTag ).AbilityTypeTag;
		StopWaitingForEquipDelegate.Broadcast( SelectedAbilityType );
		bWaitingForEquipSelection = false;
	}


	SelectedAbility.AbilityTag = FAuraGameplayTags::Get().Abilities_None;
	SelectedAbility.StatusTag = FAuraGameplayTags::Get().Abilities_Status_Locked;

	SpellGlobeSelectedDelegate.Broadcast( false, false, FString(), FString() );
}

void USpellMenuWidgetController::EquipButtonPressed()
{
	const FGameplayTag AbilityTypeTag = AbilityInfo->FindAbilityInfoForTag( SelectedAbility.AbilityTag ).AbilityTypeTag;

	WaitForEquipDelegate.Broadcast( AbilityTypeTag );
	bWaitingForEquipSelection = true;

	const FGameplayTag SelectedStatus = GetAuraAbilitySystemComponent()->GetStatusTagFromAbilityTag( SelectedAbility.AbilityTag );
	if (SelectedStatus.MatchesTagExact( FAuraGameplayTags::Get().Abilities_Status_Equipped ))
	{
		SelectedSlot = GetAuraAbilitySystemComponent()->GetInputTagFromAbilityTag( SelectedAbility.AbilityTag );
	}
}

void USpellMenuWidgetController::SpellRowGlobePressed( const FGameplayTag& SlotTag, const FGameplayTag& AbilityType )
{
	if (!bWaitingForEquipSelection)
	{
		return;
	}
	
	// Check selected ability against the slot's type. If they differ return - can't assign offensive spell to passive and vice versa
	const FGameplayTag& SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag( SelectedAbility.AbilityTag ).AbilityTypeTag;
	if (!SelectedAbilityType.MatchesTagExact( AbilityType ))
	{
		return;
	}

	GetAuraAbilitySystemComponent()->ServerEquipAbility( SelectedAbility.AbilityTag, SlotTag );
}

void USpellMenuWidgetController::OnAbilityEquipped( const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const FGameplayTag& Slot,
                                                    const FGameplayTag& PreviousSlot )
{
	bWaitingForEquipSelection = false;

	const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();

	FAuraAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag =Tags.Abilities_Status_Unlocked;
	LastSlotInfo.InputTag = PreviousSlot;
	LastSlotInfo.AbilityTag = Tags.Abilities_None;
	// Broadcast empty info if previous slot is a valid slot. Only if equipping an already equipped spell.
	AbilityInfoDelegate.Broadcast( LastSlotInfo );

	FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag( AbilityTag );
	Info.StatusTag = StatusTag;
	Info.InputTag = Slot;
	AbilityInfoDelegate.Broadcast( Info );

	StopWaitingForEquipDelegate.Broadcast( Info.AbilityTypeTag );
	SpellGlobeReassignedDelegate.Broadcast( AbilityTag );
	GlobeDeselect();
}

void USpellMenuWidgetController::ShouldEnableButtons( const FGameplayTag& StatusTag, const int32 SpellPoints, bool& bOutSpendPointsEnabled,
                                                      bool& bOutEquipEnabled ) const
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
