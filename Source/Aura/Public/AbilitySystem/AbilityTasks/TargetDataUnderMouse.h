// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"

/** Delegate sending out Position of target under cursor */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataSignature, const FVector&, Data);

/**
 * Gets target data related to target under mouse cursor
 */
UCLASS()
class AURA_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()

public:
	/** Delegate sending out Position of target under cursor */
	UPROPERTY(BlueprintAssignable)
	FMouseTargetDataSignature ValidData;
	
	/** Creates instance of this class and returns it */
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "TargetDataUnderMouse", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"))
	static UTargetDataUnderMouse* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility);

private:
	//~ Begin UGameplayTask Interface
	virtual void Activate() override;
	//~ end UGameplayTask Interface
};
