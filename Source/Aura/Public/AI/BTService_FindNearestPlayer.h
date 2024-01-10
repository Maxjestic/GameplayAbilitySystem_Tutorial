// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlueprintBase.h"
#include "BTService_FindNearestPlayer.generated.h"

/**
 * Gets all players and looks for the closest one
 */
UCLASS()
class AURA_API UBTService_FindNearestPlayer : public UBTService_BlueprintBase
{
	GENERATED_BODY()

protected:
	//~Begin UBTAuxiliaryNode Interface
	virtual void TickNode( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds ) override;
	//~End UBTAuxiliaryNode Interface

	/** Blackboard key selector for target that this follow */
	UPROPERTY( BlueprintReadOnly, EditAnywhere )
	FBlackboardKeySelector TargetToFollowSelector;

	/** Blackboard key selector for distance to target that this follow */
	UPROPERTY( BlueprintReadOnly, EditAnywhere )
	FBlackboardKeySelector DistanceToTargetSelector;
};
