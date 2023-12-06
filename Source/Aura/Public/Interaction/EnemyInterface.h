// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyInterface.generated.h"

UINTERFACE(MinimalAPI)
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Enemies interaction interface
 */
class AURA_API IEnemyInterface
{
	GENERATED_BODY()

public:
	/** Highlights the Actor */
	virtual void HighlightActor() = 0;
	
	/** Unhighlights the Actor */
	virtual void UnhighlightActor() = 0;
};
