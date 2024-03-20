// Copyright Maxjestic


#include "AbilitySystem/Ability/ArcaneShards.h"

FString UArcaneShards::GetDescription( const int32 Level )
{
	const float ManaCost = FMath::Abs( GetManaCost( Level ) );
	const float Cooldown = GetCooldown( Level );
	const FString Shards = Level == 1
		                       ? FString::Printf( TEXT( "a shard" ) )
		                       : FString::Printf( TEXT( "%d shards" ), FMath::Min( Level, MaxNumShards ) );
	const float ScaledDamage = Damage.GetValueAtLevel( Level );

	return FString::Printf( TEXT(
		// Title
		"<Title>Arcane Shards</>\n\n"

		// Details
		"<Small>Level: </><Level>%d</>\n"
		"<Small>Cost: </><ManaCost>%.1f</><Small> mana</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</><Small>s</>\n\n"

		// Description
		"<Default>Summon %s of arcane energy, causing</>"
		"<Damage> %.2f </><Default>radial arcane damage.</>" ),
	                        Level,
	                        ManaCost,
	                        Cooldown,
	                        *Shards,
	                        ScaledDamage );
}

FString UArcaneShards::GetNextLevelDescription( const int32 Level )
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
		"<Default>Summon %d shards of arcane energy, causing</>"
		"<Damage> %.2f </><Default>radial arcane damage at each shard origin.</>" ),
	                        NextLevel,
	                        ManaCost,
	                        Cooldown,
	                        FMath::Min( NextLevel, MaxNumShards ),
	                        ScaledDamage );
}
