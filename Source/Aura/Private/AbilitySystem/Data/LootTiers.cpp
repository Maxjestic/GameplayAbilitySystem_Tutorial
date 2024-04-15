// Copyright Maxjestic


#include "AbilitySystem/Data/LootTiers.h"

TArray<FLootItem> ULootTiers::GetLootItems()
{
	TArray<FLootItem> ReturnItems;

	for (FLootItem& LootItem : LootItems)
	{
		for (int32 i = 0; i < LootItem.MaxNumberToSpawn; i++)
		{
			if (FMath::RandRange( 1.f, 100.f ) < LootItem.ChanceToSpawn)
			{
				FLootItem NewItem;
				NewItem.LootClass = LootItem.LootClass;
				NewItem.bLootLevelOverride = LootItem.bLootLevelOverride;
				ReturnItems.Add( NewItem );
			}
		}
	}

	return ReturnItems;
}
