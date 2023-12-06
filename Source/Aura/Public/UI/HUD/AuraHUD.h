// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"

class UAuraWidgetController;
class UAttributeMenuWidgetController;
class UAttributeSet;
class UAbilitySystemComponent;
struct FWidgetControllerParams;
class UOverlayWidgetController;
class UAuraUserWidget;

/**
 * HUD class used to create default HUD blueprint
 * Responsible of main overlay and controllers creation
 * 
	 *  TODO #1: Change variable names to more descriptive
	 *  TODO #2: template getters for overlays?
 */
UCLASS()
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()

public:
	/** Overlay Controller getter, it creates one if it does not exist  */
	UOverlayWidgetController* GetOverlayWidgetController( const FWidgetControllerParams& WidgetControllerParams );

	/** Attribute Menu Controller getter, it creates one if it does not exist */
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(
		const FWidgetControllerParams& WidgetControllerParams );

	/** Initialization of Overlay, creates new Overlay UserWidget, sets WidgetController parameters
	 *  sets WidgetController, broadcasts initial values and adds Overlay to Viewport */
	void InitOverlay( APlayerController* PlayerController, APlayerState* PlayerState,
	                  UAbilitySystemComponent* AbilitySystemComponent, UAttributeSet* AttributeSet );

private:
	/** View - Overlay Widget, used store spawned overlay */
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> OverlayWidget;

	/** View - Overlay Widget class, used to spawn overlay */
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass;

	/** Controller - Overlay Widget Controller */
	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	/** Controller - Overlay Widget Controller class */
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	/** Controller - Attribute Menu Controller */
	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;

	/** Controller - Attribute Menu Controller class */
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;
};
