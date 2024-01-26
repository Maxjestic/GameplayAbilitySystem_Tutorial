// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AttributeInfo.generated.h"

/**
 * Stores information associated with given attribute
 * Everything but value can be set in blueprint
 * value is set based on actual value of the attribute of given character if needed (e.g. Attribute menu)
 */
USTRUCT( BlueprintType )
struct FAuraAttributeInfo
{
	GENERATED_BODY()

	/** Gameplay tag associated with the attribute */
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	FGameplayTag AttributeTag = FGameplayTag();

	/** Name of the attribute */
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	FText AttributeName = FText();

	/** Description of the attribute */
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	FText AttributeDescription = FText();

	/** Value of the attribute */
	UPROPERTY( BlueprintReadOnly )
	float AttributeValue = 0.f;
};

/**
 * Data asset storing FAuraAttributeInfo
 */
UCLASS()
class AURA_API UAttributeInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	/** Finds AttributeInfo associated with given gameplay tag, returns empty if not found, can be logged */
	FAuraAttributeInfo FindAttributeInfoForTag( const FGameplayTag& AttributeTag, const bool bLogNotFound = false ) const;

	/** Stores attribute infos structs */
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	TArray<FAuraAttributeInfo> AttributeInformation;
};
