// Copyright Maxjestic


#include "UI/ViewModel/MVVM_LoadScreen.h"

void UMVVM_LoadScreen::InitializeLoadSlots()
{
	SetNumLoadSlots( 3 );
}

void UMVVM_LoadScreen::SetNumLoadSlots( int32 InNumLoadSlots )
{
	UE_MVVM_SET_PROPERTY_VALUE( NumLoadSlots, InNumLoadSlots );
}
