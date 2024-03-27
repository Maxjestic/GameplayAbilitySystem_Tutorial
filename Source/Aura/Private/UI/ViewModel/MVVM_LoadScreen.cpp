// Copyright Maxjestic


#include "UI/ViewModel/MVVM_LoadScreen.h"

#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadScreen::InitializeLoadSlots()
{
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>( this, LoadSlotViewModelClass );
	LoadSlot_0->SetLoadSlotName( FString( "LoadSlot_0" ) );
	LoadSlots.Add( 0, LoadSlot_0 );

	LoadSlot_1 = NewObject<UMVVM_LoadSlot>( this, LoadSlotViewModelClass );
	LoadSlot_1->SetLoadSlotName( FString( "LoadSlot_1" ) );
	LoadSlots.Add( 1, LoadSlot_1 );

	LoadSlot_2 = NewObject<UMVVM_LoadSlot>( this, LoadSlotViewModelClass );
	LoadSlot_2->SetLoadSlotName( FString( "LoadSlot_2" ) );
	LoadSlots.Add( 2, LoadSlot_2 );

	SetNumLoadSlots( LoadSlots.Num() );
}

UMVVM_LoadSlot* UMVVM_LoadScreen::GetLoadSlotViewModelByIndex( const int32 Index ) const
{
	return LoadSlots.FindChecked( Index );
}

void UMVVM_LoadScreen::SetNumLoadSlots( const int32 InNumLoadSlots )
{
	UE_MVVM_SET_PROPERTY_VALUE( NumLoadSlots, InNumLoadSlots );
}
