// Copyright Maxjestic


#include "AbilitySystem/Ability/ArcaneShards.h"

FString UArcaneShards::GetDescription( const int32 Level, const bool bForNextLevel )
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
		Title = FString::Printf( TEXT( "Arcane Shards" ) );
	}
	const float ManaCost = FMath::Abs( GetManaCost( DescriptionLevel ) );
	const float Cooldown = GetCooldown( DescriptionLevel );
	const FString Shards = DescriptionLevel == 1
							   ? FString::Printf( TEXT( "a shard" ) )
							   : FString::Printf( TEXT( "%d shards" ), FMath::Min( DescriptionLevel, MaxNumShards ) );
	const float ScaledDamage = Damage.GetValueAtLevel( DescriptionLevel );

	return FString::Printf( TEXT(
		// Title
		"<Title>%s</>\n\n"

		// Details
		"<Small>Level: </><Level>%d</>\n"
		"<Small>Cost: </><ManaCost>%.1f</><Small> mana</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</><Small>s</>\n\n"

		// Description
		"<Default>Summon %s of arcane energy, causing</>"
		"<Damage> %.2f </><Default>radial arcane damage.</>" ),
							*Title,
							DescriptionLevel,
							ManaCost,
							Cooldown,
							*Shards,
							ScaledDamage );
}
