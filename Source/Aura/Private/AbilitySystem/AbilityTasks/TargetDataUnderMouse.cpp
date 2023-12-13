// Copyright Maxjestic


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse( UGameplayAbility* OwningAbility )
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	
	if(const APlayerController* PlayerController = Ability->GetCurrentActorInfo()->PlayerController.Get())
	{
		FHitResult CursorHit;
		PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
		//if (!CursorHit.bBlockingHit) return;
		ValidData.Broadcast(CursorHit.ImpactPoint);
	}
}
