// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AuraAIController.generated.h"

class UBlackboardComponent;
class UBehaviorTreeComponent;

/**
 * Base AIController class in Aura
 */
UCLASS()
class AURA_API AAuraAIController : public AAIController
{
	GENERATED_BODY()

public:
	/**
	 * Creates Blackboard and Behavior Tree Components
	 */
	AAuraAIController();

protected:	
	/** Behavior tree component owned by this */
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
};
