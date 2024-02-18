// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayTags.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

/**
 * Internal struct associated with currently selected ability
 */
struct FSelectedAbility
{
	FGameplayTag AbilityTag = FGameplayTag();
	FGameplayTag StatusTag = FGameplayTag();
};

/** Broadcasting info about selected spell in menu */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams( FSpellGlobeSelectedSignature,
                                               bool,
                                               bSpendPointsEnabled,
                                               bool,
                                               bEquipEnabled,
                                               FString,
                                               Description,
                                               FString,
                                               NextLevelDescription );

/** Broadcast ability type tag */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FWaitForEquipSelectionSignature, const FGameplayTag&, AbilityType);

/**
 * Widget controller for spell menu
 */
UCLASS( BlueprintType, Blueprintable )
class AURA_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	//~ Begin UAuraWidgetController Interface
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
	//~ End UAuraWidgetController Interface

	/** Broadcasts spell points on change */
	UPROPERTY( BlueprintAssignable, Category = "GAS|Spells" )
	FOnPlayerStatChangedSignature SpellPointsChangedDelegate;

	/** Broadcasted in SpellGlobeSelected to see if buttons should be enabled */
	UPROPERTY( BlueprintAssignable, Category = "UI" )
	FSpellGlobeSelectedSignature SpellGlobeSelectedDelegate;

	/** Broadcasts ability type tag when equip button is pressed */
	UPROPERTY( BlueprintAssignable, Category = "UI" )
	FWaitForEquipSelectionSignature WaitForEquipDelegate;

	/** Broadcasts ability type tag when player cancels equipping ability */
	UPROPERTY( BlueprintAssignable, Category = "UI" )
	FWaitForEquipSelectionSignature StopWaitingForEquipDelegate;

	/** Called when player selects spell from spell trees in spell menu */
	UFUNCTION( BlueprintCallable )
	void SpellGlobeSelected( const FGameplayTag& AbilityTag );

	/** Called when Spend Points button is clicked - unlocks/upgrades selected ability */
	UFUNCTION( BlueprintCallable )
	void SpendPointButtonPressed();

	/** Deselects spell globe button associated with given ability tag */
	UFUNCTION( BlueprintCallable )
	void GlobeDeselect();

	/** Called when "Equip" button is pressed */
	UFUNCTION( BlueprintCallable )
	void EquipButtonPressed();

private:
	/** Sets which buttons should be enabled based on StatusTag and SpellPoints */
	void ShouldEnableButtons( const FGameplayTag& StatusTag, const int32 SpellPoints, bool& bOutSpendPointsEnabled,
	                          bool& bOutEquipEnabled ) const;

	/** Currently selected ability */
	FSelectedAbility SelectedAbility = { FAuraGameplayTags::Get().Abilities_None, FAuraGameplayTags::Get().Abilities_Status_Locked };

	/** Currently available spell points */
	int32 CurrentSpellPoints = 0;

	/** Current state of equip action - true if we selected ability, clicked equip button and didn't assign ability to input yet  */
	bool bWaitingForEquipSelection = false;
	
};
