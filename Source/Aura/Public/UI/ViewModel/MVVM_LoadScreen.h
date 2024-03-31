// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadScreen.generated.h"

class UMVVM_LoadSlot;
/**
 * MVVM view model class for load screen
 */
UCLASS()
class AURA_API UMVVM_LoadScreen : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	/** Initializes load slots */
	void InitializeLoadSlots();

	UFUNCTION( BlueprintPure )
	UMVVM_LoadSlot* GetLoadSlotViewModelByIndex( const int32 Index ) const;

	/** Test accessors */
	void SetNumLoadSlots( const int32 InNumLoadSlots );
	int32 GetNumLoadSlots() const { return NumLoadSlots; }

	/** View model class for slots */
	UPROPERTY( EditDefaultsOnly )
	TSubclassOf<UMVVM_LoadSlot> LoadSlotViewModelClass;

	/**
	 * Called when new slot button is pressed
	 * @param Slot index of caller slot
	 * @param EnteredName name provided by player
	 */
	UFUNCTION( BlueprintCallable )
	void NewSlotButtonPressed( const int32 Slot, const FString& EnteredName );

	/**
	 * Called when new game button is pressed
	 * Switches to widget where player can set name
	 * @param Slot index of caller slot
	 */
	UFUNCTION( BlueprintCallable )
	void NewGameButtonPressed( int32 Slot );

	/**
	 * Called when select slot button is pressed
	 * @param Slot index of caller slot
	 */
	UFUNCTION( BlueprintCallable )
	void SelectSlotButtonPressed( int32 Slot );

	/** Loads data from saved files to load slots */
	void LoadData();

private:
	/** Container for all load slot ViewModels for given widget switcher */
	UPROPERTY()
	TMap<int32, UMVVM_LoadSlot*> LoadSlots;

	/** Pointer to every load slot */
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_0;

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_1;

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_2;

	/** Test variable to force mvvm initialization */
	UPROPERTY( EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess="true") )
	int32 NumLoadSlots;
};
