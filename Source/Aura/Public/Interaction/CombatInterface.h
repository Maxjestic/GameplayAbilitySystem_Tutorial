// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

class UAnimMontage;

// This class does not need to be modified.
UINTERFACE( MinimalAPI, BlueprintType )
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Combat interaction interface
 */
class AURA_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/** Returns character level */
	virtual int32 GetCharacterLevel() const;

	/** Returns a socket from weapon that may be used to for example spawn a projectile */
	virtual FVector GetCombatSocketLocation();

	/** Blueprint function to set warp target */
	UFUNCTION( BlueprintImplementableEvent, BlueprintCallable )
	void UpdateFacingTarget( const FVector& TargetLocation );

	/** Returns a animation montage that should be played when reacting to hit */
	UFUNCTION( BlueprintNativeEvent, BlueprintCallable )
	UAnimMontage* GetHitReactMontage();

	/** Function called when character's health reaches 0, called on the server */
	virtual void Die() = 0;
};
