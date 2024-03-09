// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "AuraCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UNiagaraComponent;
/**
 * Class for player-controlled character
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()

public:
	/**
	 *  Sets CameraComponent, SpringArmComponent, (LevelUp)NiagaraComponent, CharacterMovementComponent and Controller
	 */
	AAuraCharacter();

	//~ Begin APawn Interface
	virtual void PossessedBy( AController* NewController ) override;
	virtual void OnRep_PlayerState() override;
	//~ End APawn Interface

	//~ Begin Combat Interface
	virtual int32 GetCharacterLevel_Implementation() const override;
	//~ End Combat Interface

	//~ Begin Player Interface
	virtual int32 GetExperience_Implementation() const override;
	virtual void AddExperience_Implementation( const int32 InExperience ) override;
	virtual int32 FindLevelForExperience_Implementation( const int32 InExperience ) const override;
	virtual void LevelUp_Implementation() override;
	virtual void AddToPlayerLevel_Implementation( const int32 InPlayerLevel ) override;
	virtual int32 GetAttributePoints_Implementation() const override;
	virtual int32 GetAttributePointsReward_Implementation( const int32 PlayerLevel ) const override;
	virtual void AddToAttributePoints_Implementation( const int32 InAttributePoints ) override;
	virtual int32 GetSpellPoints_Implementation() const override;
	virtual int32 GetSpellPointsReward_Implementation( const int32 PlayerLevel ) const override;
	virtual void AddToSpellPoints_Implementation( const int32 InSpellPoints ) override;
	//~ End Player Interface
	
	//~ Begin AAuraCharacterBase Interface
	virtual void OnRep_Burned() override;
	virtual void OnRep_Stunned() override;
	//~ End AAuraCharacterBase Interface

protected:
	/** Gameplay effect initializing primary attributes for the character */
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Attributes" )
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	/** Gameplay effect initializing secondary attributes for the character */
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Attributes" )
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	/** Gameplay effect initializing vital attributes for the character */
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Attributes" )
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	/** Niagara component used on level up */
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly )
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;

private:
	//~ Begin AAuraCharacterBase Interface
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;
	//~ End AAuraCharacterBase Interface

	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUpParticles() const;

	/** Player character camera component */
	UPROPERTY( VisibleAnywhere )
	TObjectPtr<UCameraComponent> TopDownCameraComponent;
	
	/** Spring arm component for camera */
	UPROPERTY( VisibleAnywhere )
	TObjectPtr<USpringArmComponent> CameraBoom;
};
