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

/** Broadcasting if Sp */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams( FEnableButtonsSignature, bool, bSpendPointsEnabled, bool, bEquipEnabled );

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

	/** Called when player selects spell from spell trees in spell menu */
	UFUNCTION( BlueprintCallable )
	void SpellGlobeSelected( const FGameplayTag& AbilityTag );

	/** Broadcasted in SpellGlobeSelected to see if buttons should be enabled */
	UPROPERTY( BlueprintAssignable, Category = "UI" )
	FEnableButtonsSignature EnableButtons;

private:
	/** Sets which buttons should be enabled based on StatusTag and SpellPoints */
	void ShouldEnableButtons( const FGameplayTag& StatusTag, const int32 SpellPoints ) const;

	/** Currently selected ability */
	FSelectedAbility SelectedAbility = { FAuraGameplayTags::Get().Abilities_None, FAuraGameplayTags::Get().Abilities_Status_Locked };

	/** Currently available spell points */
	int32 CurrentSpellPoints = 0;
};
