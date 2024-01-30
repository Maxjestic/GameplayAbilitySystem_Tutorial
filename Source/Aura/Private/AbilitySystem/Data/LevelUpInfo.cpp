// Copyright Maxjestic


#include "AbilitySystem/Data/LevelUpInfo.h"

int32 ULevelUpInfo::FindLevelForXP( const int32 XP ) const
{
	
	bool bSearching = true;
	int32 Level = 1;
	while (bSearching)
	{
		// LevelUpInformation[1] = Level 1 Information - LevelUpInformation[0] is place holder
		if( LevelUpInformation.Num() - 1 <= Level)
		{
			return Level;
		}

		if(XP >= LevelUpInformation[Level].LevelUpRequirement)
		{
			Level++;
		}
		else
		{
			bSearching = false;
		}
	}
	return Level;
}
