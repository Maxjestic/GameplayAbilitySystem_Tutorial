// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "AuraInputConfig.h"
#include "EnhancedInputComponent.h"
#include "AuraInputComponent.generated.h"

/**
 * Default input component used in Aura
 */
UCLASS()
class AURA_API UAuraInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	/**
	 * Template function receiving functions or function pointers, binds input to callbacks
	 * UserClass - user object
	 * Pressed/Released/HeldFuncType - functions that will be bind to corresponding trigger events
	 */
	template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions( const UAuraInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc,
	                         ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc );
};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UAuraInputComponent::BindAbilityActions( const UAuraInputConfig* InputConfig, UserClass* Object,
                                              PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc,
                                              HeldFuncType HeldFunc )
{
	check(InputConfig)

	for (const auto [InputAction, InputTag] : InputConfig->AbilityInputActions)
	{
		if(!InputAction || !InputTag.IsValid())
		{
			continue;
		}
			
		if(PressedFunc)
		{
			BindAction(InputAction, ETriggerEvent::Started, Object, PressedFunc, InputTag);
		}
			
		if(ReleasedFunc)
		{
			BindAction(InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, InputTag);
		}
			
		if(HeldFunc)
		{
			BindAction(InputAction, ETriggerEvent::Triggered, Object, HeldFunc, InputTag);
		}
	}
}
