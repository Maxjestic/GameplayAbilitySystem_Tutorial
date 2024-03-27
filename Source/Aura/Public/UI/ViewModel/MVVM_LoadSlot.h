// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadSlot.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UMVVM_LoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	/** Simple Accessors */
	void SetLoadSlotName( const FString& InLoadSlotName );
	FString GetLoadSlotName() const { return LoadSlotName; }

private:
	/** Load slot name */
	UPROPERTY( EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess="true") )
	FString LoadSlotName;
};
