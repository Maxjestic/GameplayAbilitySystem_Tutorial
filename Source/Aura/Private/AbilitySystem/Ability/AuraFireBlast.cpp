// Copyright Maxjestic


#include "AbilitySystem/Ability/AuraFireBlast.h"

FString UAuraFireBlast::GetDescription( const int32 Level, const bool bForNextLevel )
{
	int32 DescriptionLevel;
	FString Title;
	if (bForNextLevel)
	{
		DescriptionLevel = Level + 1;
		Title = FString::Printf( TEXT( "Next Level" ) );
	}
	else
	{
		DescriptionLevel = Level;
		Title = FString::Printf( TEXT( "Fire Blast" ) );
	}
	const float ManaCost = FMath::Abs( GetManaCost( DescriptionLevel ) );
	const float Cooldown = GetCooldown( DescriptionLevel );
	const float ScaledDamage = Damage.GetValueAtLevel( DescriptionLevel );

	return FString::Printf( TEXT(
		// Title
		"<Title>%s</>\n\n"

		// Details
		"<Small>Level: </><Level>%d</>\n"
		"<Small>Cost: </><ManaCost>%.1f</><Small> mana</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</><Small>s</>\n\n"

		// Description
		"<Default>Launches %d fire balls in all directions, each coming back and </>"
		"<Default>exploding upon return causing</><Damage> %.2f </>"
		"<Default>radial fire damage with a chance to burn.</>" ),
	                        *Title,
	                        DescriptionLevel,
	                        ManaCost,
	                        Cooldown,
	                        NumFireBalls,
	                        ScaledDamage );
}

TArray<AAuraFireBall*> UAuraFireBlast::SpawnFireBalls()
{
	return TArray<AAuraFireBall*>();
}
