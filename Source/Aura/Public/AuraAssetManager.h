// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "AuraAssetManager.generated.h"

/**
 * Default asset manager class in Aura
 * Initializes Gameplay Tags from FAuraGameplayTags
 */
UCLASS()
class AURA_API UAuraAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	/** Singleton getter of the only instance of FAuraAssetManager */
	static UAuraAssetManager& Get();

protected:
	//~ Begin UAssetManager Interface
	virtual void StartInitialLoading() override;
	//~ End UAssetManager Interface
};
