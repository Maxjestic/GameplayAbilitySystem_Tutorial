// Copyright Maxjestic


#include "AbilitySystem/Ability/Electrocute.h"

FString UElectrocute::GetDescription( const int32 Level )
{
	const float ManaCost = FMath::Abs( GetManaCost( Level ) );
	const float Cooldown = GetCooldown( Level );
	const FString Propagating = Level == 1
		                            ? FString::Printf( TEXT( "" ) )
		                            : FString::Printf(
			                            TEXT( " and propagating to %d additional targets nearby" ),
			                            FMath::Min( Level, MaxNumShockTargets - 1 ) );
	const float ScaledDamage = Damage.GetValueAtLevel( Level );

	return FString::Printf( TEXT(
		// Title
		"<Title>Electrocute</>\n\n"

		// Details
		"<Small>Level: </><Level>%d</>\n"
		"<Small>Cost: </><ManaCost>%.1f</><Small> mana</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</><Small>s</>\n\n"

		// Description
		"<Default>Emits a beam of lightning, connectiong with the target%s, repeatedly causing</>"
		"<Damage> %.2f </><Default>lightning damage with a chance to stun.</>" ),
	                        Level,
	                        ManaCost,
	                        Cooldown,
	                        *Propagating,
	                        ScaledDamage );
}

FString UElectrocute::GetNextLevelDescription( const int32 Level )
{
	const int32 NextLevel = Level + 1;
	const float ManaCost = FMath::Abs( GetManaCost( NextLevel ) );
	const float Cooldown = GetCooldown( NextLevel );
	const float ScaledDamage = Damage.GetValueAtLevel( NextLevel );

	return FString::Printf( TEXT(
		// Title
		"<Title>Next Level:</>\n\n"

		// Details
		"<Small>Level: </><Level>%d</>\n"
		"<Small>Cost: </><ManaCost>%.1f</><Small> mana</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</><Small>s</>\n\n"

		// Description
		"<Default>Emits a beam of lightning, connectiong with the target and propagating to %d additional targets nearby, repeatedly causing</>"
		"<Damage> %.2f </><Default>lightning damage with a chance to stun.</>" ),
	                        NextLevel,
	                        ManaCost,
	                        Cooldown,
	                        FMath::Min( NextLevel, MaxNumShockTargets - 1 ),
	                        ScaledDamage );
}
