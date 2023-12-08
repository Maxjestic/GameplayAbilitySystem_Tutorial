// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "AuraCharacter.generated.h"

/**
 * Class for player-controlled character
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()

public:
	/**
	 *  Sets CharacterMovementComponent and Controller
	 */
	AAuraCharacter();

	//~ Begin APawn Interface
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	//~ End APawn Interface
	
	//~ Begin Combat Interface
	virtual int32 GetCharacterLevel() override;
	//~ End Combat Interface

private:
	//~ Begin AAuraCharacterBase Interface
	virtual void InitAbilityActorInfo() override;
	//~ End AAuraCharacterBase Interface
	
};
