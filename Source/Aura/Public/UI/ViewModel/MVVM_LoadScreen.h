// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadScreen.generated.h"

/**
 * MVVM view model class for load screen
 */
UCLASS()
class AURA_API UMVVM_LoadScreen : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	void InitializeLoadSlots();

	/** Test accessors */
	void SetNumLoadSlots( int32 InNumLoadSlots );
	int32 GetNumLoadSlots() const { return NumLoadSlots; };

private:
	/** Test variable to force mvvm initialization */
	UPROPERTY( EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess="true") )
	int32 NumLoadSlots;
};
