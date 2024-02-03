// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

class UAuraAttributeSet;
class UAttributeInfo;

/** Declaration of delegate sending AuraAttributeInfo */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FAttributeInfoSignature, const FAuraAttributeInfo&, Info );

/**
 * Widget controller for attribute menu
 */
UCLASS( BlueprintType, Blueprintable )
class AURA_API UAttributeMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	//~ Begin UAuraWidgetController Interface
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
	//~ End UAuraWidgetController Interface

	/** Delegate to broadcast AuraAttributeInfo */
	UPROPERTY( BlueprintAssignable, Category="GAS|Attributes" )
	FAttributeInfoSignature AttributeInfoDelegate;

	/** Broadcasts attribute points on change */
	UPROPERTY( BlueprintAssignable, Category = "GAS|Attributes" )
	FOnPlayerStatChangedSignature AttributePointsChangedDelegate;

	/** Broadcasts spell points on change */
	UPROPERTY( BlueprintAssignable, Category = "GAS|Spells" )
	FOnPlayerStatChangedSignature SpellPointsChangedDelegate;

	/** Activated from blueprint to increase attribute associated with given tag */
	UFUNCTION(BlueprintCallable)
	void UpgradeAttribute( const FGameplayTag& AttributeTag );

protected:
	/** Data asset containing AuraAttributeInfo with relevant attributes */
	UPROPERTY( EditDefaultsOnly )
	TObjectPtr<UAttributeInfo> AttributeInfo;

private:
	/** Searches for attribute and assigns attribute value to AuraAttributeInfo struct */
	void BroadcastAttributeInfo( const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute ) const;
};
