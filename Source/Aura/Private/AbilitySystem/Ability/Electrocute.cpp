// Copyright Maxjestic


#include "AbilitySystem/Ability/Electrocute.h"

FString UElectrocute::GetDescription( const int32 Level, const bool bForNextLevel )
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
		Title = FString::Printf( TEXT( "Electrocute" ) );
	}
	const float ManaCost = FMath::Abs( GetManaCost( DescriptionLevel ) );
	const float Cooldown = GetCooldown( DescriptionLevel );
	const FString Propagating = DescriptionLevel == 1
									? FString::Printf( TEXT( "" ) )
									: FString::Printf(
										TEXT( " and propagating to %d additional targets nearby" ),
										FMath::Min( DescriptionLevel, MaxNumShockTargets - 1 ) );
	const float ScaledDamage = Damage.GetValueAtLevel( DescriptionLevel );

	return FString::Printf( TEXT(
		// Title
		"<Title>%s</>\n\n"

		// Details
		"<Small>Level: </><Level>%d</>\n"
		"<Small>Cost: </><ManaCost>%.1f</><Small> mana</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</><Small>s</>\n\n"

		// Description
		"<Default>Emits a beam of lightning, connectiong with the target%s, repeatedly causing</>"
		"<Damage> %.2f </><Default>lightning damage with a chance to stun.</>" ),
							*Title,
							DescriptionLevel,
							ManaCost,
							Cooldown,
							*Propagating,
							ScaledDamage );
}
