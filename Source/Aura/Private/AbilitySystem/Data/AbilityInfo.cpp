// Copyright Maxjestic


#include "AbilitySystem/Data/AbilityInfo.h"

#include "Aura/AuraLogChannels.h"

FAuraAbilityInfo UAbilityInfo::FindAbilityForTag( const FGameplayTag& AbilityTag, bool bLogNotFound ) const
{
	for (const FAuraAbilityInfo& Info : AbilityInformation)
	{
		if (Info.AbilityTag.MatchesTagExact( AbilityTag ))
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG( LogAura,
		        Error,
		        TEXT("Can't find Info for AabilityeTag [%s] on AbilityInfo [%s]."),
		        *AbilityTag.ToString(),
		        *GetNameSafe(this) );
	}

	return FAuraAbilityInfo();
}
