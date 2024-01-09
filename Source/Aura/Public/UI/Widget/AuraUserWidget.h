// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraUserWidget.generated.h"

/**
 * Generic class used as a base for every User Widget Blueprint
 * One way dependency on Widget Controller
 */
UCLASS()
class AURA_API UAuraUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Widget Controller setter, also calling WidgetControllerSet() */
	UFUNCTION( BlueprintCallable )
	void SetWidgetController( UObject* InWidgetController );

	/** Controller from which data is retrieved */
	UPROPERTY( BlueprintReadOnly )
	TObjectPtr<UObject> WidgetController;

protected:
	/** BeginPlay-like event implemented in blueprint*/
	UFUNCTION( BlueprintImplementableEvent )
	void WidgetControllerSet();
};
