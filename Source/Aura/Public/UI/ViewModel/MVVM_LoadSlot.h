// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadSlot.generated.h"

/** Broadcast widget switcher index */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FSetWidgetSwitcherIndex, int32, WidgetSwitcherIndex );

/**
 * ViewModel for load slots
 */
UCLASS()
class AURA_API UMVVM_LoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	/** Broadcasts index to witch widget switcher should switch */
	UPROPERTY( BlueprintAssignable )
	FSetWidgetSwitcherIndex SetWidgetSwitcherIndex;

	/** Initializes slot */
	void InitializeSlot();

	/** Simple Accessors */
	void SetLoadSlotName( const FString& InLoadSlotName );
	FString GetLoadSlotName() const { return LoadSlotName; }

	void SetPlayerName( const FString& InPlayerName );
	FString GetPlayerName() const { return PlayerName; }

	/** Index associated with save slot */
	UPROPERTY()
	FString SlotIndex;

private:
	/** Name associated with save slot */
	UPROPERTY( EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess="true") )
	FString LoadSlotName;

	/** Name provided by player */
	UPROPERTY( EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess="true") )
	FString PlayerName;
};
