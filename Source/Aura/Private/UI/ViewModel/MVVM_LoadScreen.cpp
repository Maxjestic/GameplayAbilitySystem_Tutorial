// Copyright Maxjestic


#include "UI/ViewModel/MVVM_LoadScreen.h"

#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadScreen::InitializeLoadSlots()
{
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>( this, LoadSlotViewModelClass );
	LoadSlot_0->SetLoadSlotName( FString( "LoadSlot_0" ) );
	LoadSlot_0->SlotIndex = 0;
	LoadSlots.Add( 0, LoadSlot_0 );

	LoadSlot_1 = NewObject<UMVVM_LoadSlot>( this, LoadSlotViewModelClass );
	LoadSlot_1->SetLoadSlotName( FString( "LoadSlot_1" ) );
	LoadSlot_1->SlotIndex = 1;
	LoadSlots.Add( 1, LoadSlot_1 );

	LoadSlot_2 = NewObject<UMVVM_LoadSlot>( this, LoadSlotViewModelClass );
	LoadSlot_2->SetLoadSlotName( FString( "LoadSlot_2" ) );
	LoadSlot_2->SlotIndex = 2;
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

void UMVVM_LoadScreen::NewSlotButtonPressed( const int32 Slot, const FString& EnteredName )
{
	if (const AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>( UGameplayStatics::GetGameMode( this ) ))
	{
		LoadSlots[Slot]->SetPlayerName( EnteredName );

		if (AuraGameMode->SaveSlotData( LoadSlots[Slot], Slot ))
		{
			LoadSlots[Slot]->SlotStatus = Taken;
			LoadSlots[Slot]->SetLevelName( AuraGameMode->DefaultLevelName );
		}

		LoadSlots[Slot]->InitializeSlot();
	}
}

void UMVVM_LoadScreen::NewGameButtonPressed( const int32 Slot )
{
	LoadSlots[Slot]->SetWidgetSwitcherIndex.Broadcast( 1 );
}

void UMVVM_LoadScreen::SelectSlotButtonPressed( const int32 Slot )
{
	SlotSelected.Broadcast();
	SelectedSlot = LoadSlots[Slot];
	for (const TTuple<int, UMVVM_LoadSlot*> LoadSlot : LoadSlots)
	{
		LoadSlot.Value->EnableSelectSlotButton.Broadcast( Slot != LoadSlot.Key );
	}
}

void UMVVM_LoadScreen::DeleteButtonPressed()
{
	if (IsValid( SelectedSlot ))
	{
		AAuraGameModeBase::DeleteSlotIfExists( SelectedSlot->GetLoadSlotName(), SelectedSlot->SlotIndex );
		SelectedSlot->SlotStatus = Vacant;
		SelectedSlot->InitializeSlot();
		SelectedSlot->EnableSelectSlotButton.Broadcast( true );
	}
}

void UMVVM_LoadScreen::LoadData()
{
	const AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>( UGameplayStatics::GetGameMode( this ) );
	for (const TTuple<int, UMVVM_LoadSlot*> LoadSlot : LoadSlots)
	{
		const ULoadScreenSaveGame* SaveObject = AuraGameMode->GetSaveSlotData( LoadSlot.Value->GetLoadSlotName(), LoadSlot.Key );
		const FString PlayerName = SaveObject->PlayerName;
		const FString LevelName = SaveObject->LevelName;
		const TEnumAsByte<ESaveSlotStatus> SlotStatus = SaveObject->SaveSlotStatus;

		LoadSlot.Value->SlotStatus = SlotStatus;
		LoadSlot.Value->SetPlayerName( PlayerName );
		LoadSlot.Value->SetLevelName( LevelName );
		LoadSlot.Value->InitializeSlot();
	}
}
