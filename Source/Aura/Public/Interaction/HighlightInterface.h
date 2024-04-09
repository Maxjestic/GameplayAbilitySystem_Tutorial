// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HighlightInterface.generated.h"

// This class does not need to be modified.
UINTERFACE( MinimalAPI, BlueprintType )
class UHighlightInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * interface for highlighting actors
 */
class AURA_API IHighlightInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/** Highlights the Actor */
	UFUNCTION( BlueprintNativeEvent )
	void HighlightActor();

	/** Unhighlights the Actor */
	UFUNCTION( BlueprintNativeEvent )
	void UnHighlightActor();

	/** Returns move to location (if there is one) when clicked on owner */
	UFUNCTION( BlueprintNativeEvent )
	void SetMoveToLocation( FVector& OutDestination );
};
