// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "OverlayWidgetController.generated.h"

class UAbilityInfo;
class UAuraUserWidget;

/** Row structure containing pop-up information data displayed to user */
USTRUCT( BlueprintType )
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	/** Gameplay tag used to distinguish type of displayed message */
	UPROPERTY( EditAnywhere, BlueprintReadOnly )
	FGameplayTag MessageTag = FGameplayTag();

	/** Message displayed to user */
	UPROPERTY( EditAnywhere, BlueprintReadOnly )
	FText Message = FText();

	/** Class of displayed widget */
	UPROPERTY( EditAnywhere, BlueprintReadOnly )
	TSubclassOf<UAuraUserWidget> MessageWidget;

	/** Image displayed to user */
	UPROPERTY( EditAnywhere, BlueprintReadOnly )
	UTexture2D* Image = nullptr;
};

/** Declaration of delegate broadcasting attribute changes */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnAttributeChangedSignature, float, NewValue );

/** Declaration of delegate broadcasting UIWidgetRow to show message for user in viewport */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FMessageWidgetRowSignature, FUIWidgetRow, Row );

/**
 * Controller for Overlay Widget
 * Delegates: OnAttributeChanged, MessageWidgetRow
 * Contains DataTable with data used in message
 */
UCLASS( BlueprintType, Blueprintable )
class AURA_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	//~ Begin UAuraWidgetController Interface
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
	//~ End UAuraWidgetController Interface

	/**
	 * OnAttributeChanged Delegates
	 */
	UPROPERTY( BlueprintAssignable, Category = "GAS|Attributes" )
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY( BlueprintAssignable, Category = "GAS|Attributes" )
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY( BlueprintAssignable, Category = "GAS|Attributes" )
	FOnAttributeChangedSignature OnManaChanged;

	UPROPERTY( BlueprintAssignable, Category = "GAS|Attributes" )
	FOnAttributeChangedSignature OnMaxManaChanged;

	/** Delegate broadcasting data of type FUIWidgetRow to display in player's viewport */
	UPROPERTY( BlueprintAssignable, Category = "GAS|Messages" )
	FMessageWidgetRowSignature MessageWidgetRow;

protected:
	/** Finds data table row by corresponding gameplay tag in given data table */
	template <typename T>
	T* GetDataTableRowByTag( UDataTable* DataTable, const FGameplayTag& Tag );

	/** Data Table used to display messages to the player screen */
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data" )
	TObjectPtr<UDataTable> MessageWidgetDataTable;

	/** Data asset used to assign visuals to spell globes in overlay */
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data" )
	TObjectPtr<UAbilityInfo> AbilityInfo;
};

template <typename T>
T* UOverlayWidgetController::GetDataTableRowByTag( UDataTable* DataTable, const FGameplayTag& Tag )
{
	return DataTable->FindRow<T>( Tag.GetTagName(), TEXT( "" ) );
}
