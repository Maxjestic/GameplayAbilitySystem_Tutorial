// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/AuraDamageGameplayAbility.h"
#include "AuraBeamSpell.generated.h"

/**
 * A beam of lightning spell
 */
UCLASS()
class AURA_API UAuraBeamSpell : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

public:
	/** Caching values important for this form TargetDataUnderMouse */
	UFUNCTION( BlueprintCallable )
	void StoreMouseDataInfo( const FHitResult& HitResult );

	/** Stores player controller of the owner of this */
	UFUNCTION( BlueprintCallable )
	void StoreOwnerVariables();

	/**
	 * Traces for the fires target between caster and BeamTargetLocation
	 * if there is nothing between BeamTargetLocation is consider a first target
	 */
	UFUNCTION( BlueprintCallable )
	void TraceFirstTarget( const FVector& BeamTargetLocation );

	/** Stores additional targets in specified radius, number od additional targets is based on ability level */
	UFUNCTION( BlueprintCallable )
	void StoreAdditionalTargets( TArray<AActor*>& OutAdditionalTargets );

	/** Callback to when primary target dies during cast */
	UFUNCTION( BlueprintImplementableEvent )
	void PrimaryTargetDied( AActor* DeadActor );

	/** Callback to when additional target dies during cast */
	UFUNCTION( BlueprintImplementableEvent )
	void AdditionalTargetDied( AActor* DeadActor );

protected:
	/** Mouse location deduced in TargetDataUnderMouse */
	UPROPERTY( BlueprintReadWrite, Category="Beam" )
	FVector MouseHitLocation;

	/** Actor deduced in TargetDataUnderMouse */
	UPROPERTY( BlueprintReadWrite, Category="Beam" )
	TObjectPtr<AActor> MouseHitActor;

	/** Player controller of the owner of this */
	UPROPERTY( BlueprintReadWrite, Category="Beam" )
	TObjectPtr<APlayerController> OwnerPlayerController;

	/** The owner character of this */
	UPROPERTY( BlueprintReadWrite, Category="Beam" )
	TObjectPtr<ACharacter> OwnerCharacter;

	/** Maximum number of targets for this, based on ability level */
	UPROPERTY( EditDefaultsOnly, Category="Beam" )
	int32 MaxNumShockTargets = 5;
};
