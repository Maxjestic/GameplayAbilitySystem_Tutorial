// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"

class UGameplayEffect;
class UGameplayAbility;

/**
 * All character classes in Aura
 */
UENUM( BlueprintType )
enum class ECharacterClass : uint8
{
	Elementalist,
	Warrior,
	Ranger
};

/**
 * Stores information for each class
 */
USTRUCT( BlueprintType )
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()

	/** Gameplay effect setting primary attributes */
	UPROPERTY( EditDefaultsOnly, Category = "Class Defaults" )
	TSubclassOf<UGameplayEffect> PrimaryAttributes;
};

/**
 * Data asset for each character class and the values for its default attributes based on its level
 */
UCLASS()
class AURA_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	/** Class default info getter */
	FCharacterClassDefaultInfo GetClassDefaultInfo( const ECharacterClass CharacterClass );

	/** Default class info for each character class */
	UPROPERTY( EditDefaultsOnly, Category = "Character Class Defaults" )
	TMap<ECharacterClass, FCharacterClassDefaultInfo> CharacterClassInformation;

	/** Gameplay effect setting secondary attributes, common to all characters */
	UPROPERTY( EditDefaultsOnly, Category = "Common Class Defaults" )
	TSubclassOf<UGameplayEffect> SecondaryAttributes;

	/** Gameplay effect setting vital attributes, common to all characters */
	UPROPERTY( EditDefaultsOnly, Category = "Common Class Defaults" )
	TSubclassOf<UGameplayEffect> VitalAttributes;

	/** Applicable gameplay abilities */
	UPROPERTY( EditDefaultsOnly, Category = "Common Class Defaults" )
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;

	/** Coefficients used when calculating damage */
	UPROPERTY( EditDefaultsOnly, Category = "Common Class Defaults|Damage" )
	TObjectPtr<UCurveTable> DamageCalculationCoefficients;
};
