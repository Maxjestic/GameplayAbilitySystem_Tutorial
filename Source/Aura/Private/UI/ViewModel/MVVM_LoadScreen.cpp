// Copyright Maxjestic


#include "UI/ViewModel/MVVM_LoadScreen.h"

#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
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

void UMVVM_LoadScreen::NewSlotButtonPressed( int32 Slot, const FString& EnteredName )
{
	if (AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>( UGameplayStatics::GetGameMode( this ) ))
	{
		LoadSlots[Slot]->SetPlayerName( EnteredName );
		
		AuraGameMode->SaveSlotData( LoadSlots[Slot], Slot );
		
		LoadSlots[Slot]->InitializeSlot();
	}
}

void UMVVM_LoadScreen::NewGameButtonPressed( int32 Slot )
{
	LoadSlots[Slot]->SetWidgetSwitcherIndex.Broadcast( 1 );
}

void UMVVM_LoadScreen::SelectSlotButtonPressed( int32 Slot )
{
}
