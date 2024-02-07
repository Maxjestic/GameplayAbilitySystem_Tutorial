// Copyright Maxjestic


#include "UI/HUD/AuraHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/WidgetController/SpellMenuWidgetController.h"

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController( const FWidgetControllerParams& WidgetControllerParams )
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>( this, OverlayWidgetControllerClass );
		OverlayWidgetController->SetWidgetControllerParams( WidgetControllerParams );
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}

UAttributeMenuWidgetController* AAuraHUD::GetAttributeMenuWidgetController(
	const FWidgetControllerParams& WidgetControllerParams )
{
	if (AttributeMenuWidgetController == nullptr)
	{
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(
			this,
			AttributeMenuWidgetControllerClass );
		
		AttributeMenuWidgetController->SetWidgetControllerParams( WidgetControllerParams );
		AttributeMenuWidgetController->BindCallbacksToDependencies();
	}
	return AttributeMenuWidgetController;
}

USpellMenuWidgetController* AAuraHUD::GetSpellMenuWidgetController( const FWidgetControllerParams& WidgetControllerParams )
{
	if (SpellMenuWidgetController == nullptr)
	{
		SpellMenuWidgetController = NewObject<USpellMenuWidgetController>(
			this,
			SpellMenuWidgetControllerClass );
		
		SpellMenuWidgetController->SetWidgetControllerParams( WidgetControllerParams );
		SpellMenuWidgetController->BindCallbacksToDependencies();
	}
	return SpellMenuWidgetController;
}

void AAuraHUD::InitOverlay( APlayerController* PlayerController, APlayerState* PlayerState,
                            UAbilitySystemComponent* AbilitySystemComponent, UAttributeSet* AttributeSet )
{
	checkf( OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, please fill out BP_AuraHUD") )
	checkf( OverlayWidgetControllerClass,
	        TEXT("Overlay Widget Controller Class uninitialized, please fill out BP_AuraHUD") )

	const FWidgetControllerParams WidgetControllerParams( PlayerController,
	                                                      PlayerState,
	                                                      AbilitySystemComponent,
	                                                      AttributeSet );
	
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController( WidgetControllerParams );

	UUserWidget* Widget = CreateWidget<UUserWidget>( GetWorld(), OverlayWidgetClass );
	OverlayWidget = Cast<UAuraUserWidget>( Widget );
	OverlayWidget->SetWidgetController( WidgetController );
	WidgetController->BroadcastInitialValues();
	Widget->AddToViewport();
}
