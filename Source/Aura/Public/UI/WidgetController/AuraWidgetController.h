// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraWidgetController.generated.h"

class UAbilityInfo;
class UAuraAttributeSet;
class UAuraAbilitySystemComponent;
class AAuraPlayerState;
class AAuraPlayerController;
class UAttributeSet;
class UAbilitySystemComponent;

/**
 * A Struct used to initialize Widget Controller with Models
 * Models: PlayerController, PlayerState, AbilitySystemComponent, AttributeSet
 */
USTRUCT( BlueprintType )
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams()
	{
	}

	FWidgetControllerParams( APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS )
		: PlayerController( PC ), PlayerState( PS ), AbilitySystemComponent( ASC ), AttributeSet( AS )
	{
	}

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	TObjectPtr<APlayerState> PlayerState = nullptr;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};



/** Broadcasts player stat changes */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnPlayerStatChangedSignature, int32, NewValue );

/** Broadcasts AbilityInfo */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam ( FAbilityInfoSignature, const FAuraAbilityInfo&, Info );

/**
 * Base Widget Controller class containing each possible model
 * One way dependency on various models
 */
UCLASS()
class AURA_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()

public:
	/** Function setting all models for Widget Controller */
	UFUNCTION( BlueprintCallable )
	void SetWidgetControllerParams( const FWidgetControllerParams& WidgetControllerParams );

	/** Function broadcasting initial values, empty definition - no need to call Super */
	UFUNCTION( BlueprintCallable )
	virtual void BroadcastInitialValues();

	/** Function used to bind callbacks to dependencies, empty definition - no need to call Super */
	virtual void BindCallbacksToDependencies();

	/** Forces all abilities to broadcast their ability info */
	void BroadcastAbilityInfo();

	/** Broadcasts AbilityInfo */
	UPROPERTY( BlueprintAssignable, Category = "GAS|Messages" )
	FAbilityInfoSignature AbilityInfoDelegate;

protected:
	/** Model - Player Controller */
	UPROPERTY( BlueprintReadOnly, Category = "WidgetController" )
	TObjectPtr<APlayerController> PlayerController;

	/** Model - Player State */
	UPROPERTY( BlueprintReadOnly, Category = "WidgetController" )
	TObjectPtr<APlayerState> PlayerState;

	/** Model - Ability System Component */
	UPROPERTY( BlueprintReadOnly, Category = "WidgetController" )
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	/** Model - Attribute Set */
	UPROPERTY( BlueprintReadOnly, Category = "WidgetController" )
	TObjectPtr<UAttributeSet> AttributeSet;

	/** Data asset used to assign visuals to spell globes in overlay */
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data" )
	TObjectPtr<UAbilityInfo> AbilityInfo;

	/** Returns Aura Player Controller, lazy casts */
	AAuraPlayerController* GetAuraPlayerController();

	/** Returns Aura Player State, lazy casts */
	AAuraPlayerState* GetAuraPlayerState();

	/** Returns Aura Ability System Component, lazy casts */
	UAuraAbilitySystemComponent* GetAuraAbilitySystemComponent();

	/** Returns Aura Attribute Set, lazy casts */
	UAuraAttributeSet* GetAuraAttributeSet();

private:	
	/** Aura Player Controller stored after lazy cast */
	UPROPERTY( /*BlueprintReadOnly, Category = "WidgetController"*/ )
	TObjectPtr<AAuraPlayerController> AuraPlayerController;

	/** Aura Player State stored after lazy cast */
	UPROPERTY( /*BlueprintReadOnly, Category = "WidgetController"*/ )
	TObjectPtr<AAuraPlayerState> AuraPlayerState;

	/** Aura Ability System Component stored after lazy cast */
	UPROPERTY( /*BlueprintReadOnly, Category = "WidgetController"*/ )
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	/** Aura Attribute Set stored after lazy cast */
	UPROPERTY( /*BlueprintReadOnly, Category = "WidgetController"*/ )
	TObjectPtr<UAuraAttributeSet> AuraAttributeSet;
	
};
