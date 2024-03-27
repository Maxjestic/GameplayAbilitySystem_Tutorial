// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadScreenWidget.generated.h"

/**
 * Base class for widgets in load screen
 */
UCLASS()
class AURA_API ULoadScreenWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/** Blueprint implemented and called in C++ to initialize the widget with all of its important stuff */
	UFUNCTION( BlueprintImplementableEvent, BlueprintCallable )
	void BlueprintInitializeWidget();
};
