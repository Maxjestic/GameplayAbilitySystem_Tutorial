// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE( MinimalAPI )
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IPlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/** Returns owners experience points*/
	UFUNCTION( BlueprintNativeEvent )
	int32 GetExperience() const;

	/** Adds experience to the owner */
	UFUNCTION( BlueprintNativeEvent )
	void AddExperience( const int32 InExperience );


	/** Returns level corresponding to given experience amount */
	UFUNCTION( BlueprintNativeEvent )
	int32 FindLevelForExperience( const int32 InExperience ) const;

	/** Performs level up on the owner */
	UFUNCTION( BlueprintNativeEvent )
	void LevelUp();

	/** Adds given amount to level of the owner */
	UFUNCTION( BlueprintNativeEvent )
	void AddToPlayerLevel( const int32 InPlayerLevel );

	
	/** Returns amount of attribute points that owner has */
	UFUNCTION( BlueprintNativeEvent )
	int32 GetAttributePoints() const;

	/** Returns amount of attribute points that should be rewarded to the owner */
	UFUNCTION( BlueprintNativeEvent )
	int32 GetAttributePointsReward( const int32 PlayerLevel ) const;

	/** Adds given amount of attribute points */
	UFUNCTION( BlueprintNativeEvent )
	void AddToAttributePoints( const int32 InAttributePoints );


	/** Returns amount of spell points that owner has */
	UFUNCTION( BlueprintNativeEvent )
	int32 GetSpellPoints() const;
	
	/** Returns amount of spell points that should be rewarded to the owner */
	UFUNCTION( BlueprintNativeEvent )
	int32 GetSpellPointsReward( const int32 PlayerLevel ) const;

	/** Adds given amount to spell points */
	UFUNCTION( BlueprintNativeEvent )
	void AddToSpellPoints( const int32 InSpellPoints );

	/** Shows magic circle, material can be changed by passing new one */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ShowMagicCircle(UMaterialInterface* DecalMaterial = nullptr);

	/** Hides magic circle */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void HideMagicCircle();
};
