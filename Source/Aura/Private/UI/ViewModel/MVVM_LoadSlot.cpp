// Copyright Maxjestic


#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadSlot::SetLoadSlotName( const FString& InLoadSlotName )
{
	UE_MVVM_SET_PROPERTY_VALUE( LoadSlotName, InLoadSlotName );
}
