// Copyright Maxjestic


#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadSlot::InitializeSlot()
{
	const int32 WidgetSwitcherIndex = SlotStatus.GetValue();
	SetWidgetSwitcherIndex.Broadcast( WidgetSwitcherIndex );
}

void UMVVM_LoadSlot::SetLoadSlotName( const FString& InLoadSlotName )
{
	UE_MVVM_SET_PROPERTY_VALUE( LoadSlotName, InLoadSlotName );
}

void UMVVM_LoadSlot::SetPlayerName( const FString& InPlayerName )
{
	UE_MVVM_SET_PROPERTY_VALUE( PlayerName, InPlayerName );
}