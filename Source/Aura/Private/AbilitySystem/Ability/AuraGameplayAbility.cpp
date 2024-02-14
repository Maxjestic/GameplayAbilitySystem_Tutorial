// Copyright Maxjestic


#include "AbilitySystem/Ability/AuraGameplayAbility.h"

FString UAuraGameplayAbility::GetDescription( const int32 Level )
{
	return FString::Printf( TEXT( "<Default>%s, </><Level>%d</>" ),
	                        L"Default Ability Name - Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nulla lacus orci, tempus in molestie id, luctus in nibh. Curabitur leo.",
	                        Level );
}

FString UAuraGameplayAbility::GetNextLevelDescription( const int32 Level )
{
	return FString::Printf( TEXT( "<Default>Next Level: </><Level>%d</>\n<Default>Causes much more damage.</>" ), Level + 1 );
}

FString UAuraGameplayAbility::GetLockedDescription( const int32 Level )
{
	return FString::Printf( TEXT( "<Default>Spell Locked Until Level: %d</>" ), Level );
}
