// Copyright Maxjestic


#include "Game/LoadScreenSaveGame.h"

bool ULoadScreenSaveGame::GetSavedMapWithMapName( const FString& InMapName, FSavedMap& OutSavedMap )
{
	for (const FSavedMap& Map : SavedMaps)
	{
		if(Map.MapAssetName == InMapName)
		{
			OutSavedMap = Map;
			return true;
		}
	}
	return false;
}
