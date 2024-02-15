// Copyright Maxjestic


#include "AbilitySystem/Ability/AuraGameplayAbility.h"

#include "AbilitySystem/AuraAttributeSet.h"

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

float UAuraGameplayAbility::GetManaCost( const float InLevel ) const 
{
	if (const UGameplayEffect* CostEffect = GetCostGameplayEffect())
	{
		for (const FGameplayModifierInfo& Modifier : CostEffect->Modifiers)
		{
			if (Modifier.Attribute == UAuraAttributeSet::GetManaAttribute())
			{
				float ManaCost = 0.f;
				Modifier.ModifierMagnitude.GetStaticMagnitudeIfPossible( InLevel, ManaCost );
				return ManaCost;
			}
		}
	}
	return 0.f;
}

float UAuraGameplayAbility::GetCooldown( const float InLevel ) const
{
	if (const UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect())
	{
		float Cooldown = 0.f;
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible( InLevel, Cooldown );
		return Cooldown;
	}
	
	return 0.f;
}
