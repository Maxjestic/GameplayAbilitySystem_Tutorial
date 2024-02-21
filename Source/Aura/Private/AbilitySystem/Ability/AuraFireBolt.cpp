// Copyright Maxjestic


#include "AbilitySystem/Ability/AuraFireBolt.h"

FString UAuraFireBolt::GetDescription( const int32 Level )
{
	const float ManaCost = FMath::Abs( GetManaCost( Level ) );
	const float Cooldown = GetCooldown( Level );
	const FString Bolts = Level == 1
		                      ? FString::Printf( TEXT( "a bolt" ) )
		                      : FString::Printf( TEXT( "%d bolts" ), FMath::Min( Level, NumProjectiles ) );
	const float ScaledDamage = Damage.GetValueAtLevel( Level );

	return FString::Printf( TEXT(
		// Title
		"<Title>Fire Bolt</>\n\n"

		// Details
		"<Small>Level: </><Level>%d</>\n"
		"<Small>Cost: </><ManaCost>%.1f</><Small> mana</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</><Small>s</>\n\n"

		// Description
		"<Default>Launches %s of fire, exploding on impact and dealing: </><Damage>%.2f</>"
		"<Default> fire damage with a chance to burn.</>" ),
	                        Level,
	                        ManaCost,
	                        Cooldown,
	                        *Bolts,
	                        ScaledDamage );
}

FString UAuraFireBolt::GetNextLevelDescription( const int32 Level )
{
	const int32 NextLevel = Level + 1;
	const float ManaCost = FMath::Abs( GetManaCost( NextLevel ) );
	const float Cooldown = GetCooldown( NextLevel );
	const float ScaledDamage = Damage.GetValueAtLevel( Level );

	return FString::Printf( TEXT(
		// Title
		"<Title>Next Level:</>\n\n"

		// Details
		"<Small>Level: </><Level>%d</>\n"
		"<Small>Cost: </><ManaCost>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</><Small>s</>\n\n"

		// Description
		"<Default>Launches %d bolts of fire, exploding on impact and dealing: </><Damage>%.2f</>"
		"<Default> fire damage with a chance to burn.</>" ),
	                        NextLevel,
	                        ManaCost,
	                        Cooldown,
	                        FMath::Min( NextLevel, NumProjectiles ),
	                        ScaledDamage );
}
