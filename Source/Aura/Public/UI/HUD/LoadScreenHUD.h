// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LoadScreenHUD.generated.h"

class UMVVM_LoadScreen;
class ULoadScreenWidget;

/**
 * HUD class for load screen
 */
UCLASS()
class AURA_API ALoadScreenHUD : public AHUD
{
	GENERATED_BODY()

public:
	/** Load screen ViewModel class */
	UPROPERTY( EditDefaultsOnly )
	TSubclassOf<UMVVM_LoadScreen> LoadScreenViewModelClass;

	/** Actual ViewModel */
	UPROPERTY( BlueprintReadOnly )
	TObjectPtr<UMVVM_LoadScreen> LoadScreenViewModel;
	
	/** Load screen main widget class that should be added to viewport */
	UPROPERTY( EditDefaultsOnly )
	TSubclassOf<UUserWidget> LoadScreenWidgetClass;

	/** Actual load screen main widget */
	UPROPERTY( BlueprintReadOnly )
	TObjectPtr<ULoadScreenWidget> LoadScreenWidget;

protected:
	//~ Begin AActor Interface
	virtual void BeginPlay() override;
	//~ End AActor Interface
};
