// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class UDamageTextComponent;
class USplineComponent;
class UAuraAbilitySystemComponent;
class UAuraInputConfig;
class UInputMappingContext;
class UInputAction;
class IEnemyInterface;

struct FInputActionValue;

/**
 * Controller class used as default controller class in game
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	/**
	 * Sets replication and initializes spline component
	 */
	AAuraPlayerController();

	//~ Begin APlayerController Interface
	virtual void PlayerTick( float DeltaTime ) override;
	//~ End APlayerController Interface

	/** Called when character takes damage to show the amount with widget */
	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(const float Damage, ACharacter* TargetCharacter);
	
protected:
	//~ Begin AActor Interface
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	//~ End AActor Interface

private:
	/** Callback function implementing movement */
	void Move( const FInputActionValue& InputActionValue );

	/** Callback function implementing movement */
	void ShiftPressed() { bShiftKeyDown = true; }

	/** Callback function implementing movement */
	void ShiftReleased() { bShiftKeyDown = false; }

	/** Implementation of movement using points on spline generated in response to given Input Action */
	void AutoRun();

	/** Tracing under cursor to target actors */
	void CursorTrace();

	/**
	 * Input callback functions for each supported trigger event
	 */
	void AbilityInputTagPressed( FGameplayTag InputTag );
	void AbilityInputTagReleased( FGameplayTag InputTag );
	void AbilityInputTagHeld( FGameplayTag InputTag );

	/** A getter for AbilitySystemComponent, if ASC property is nullptr function tries to get it but may fail and return nullptr */
	UAuraAbilitySystemComponent* GetAbilitySystemComponent();

	/** Input Mapping Context used to map input */
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	/** Input Action variable used to move character */
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	/** Input Action variable for shift key */
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ShiftAction;

	/** Input config data asset, contains associated input actions with gameplay tags */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

	/** Ability system controller associated with pawn controlled by this */
	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	/**
	 * Interactable actors under cursor
	 */
	IEnemyInterface* PreviousActor;
	IEnemyInterface* NewActor;

	/** Hit result from under cursor trace */
	FHitResult CursorHit;

	/** Running action destination */
	FVector CachedDestination = FVector::ZeroVector;

	/** How long this is following a cursor */
	float FollowTime = 0.f;

	/** Threshold for short button press */
	float ShortPressThreshold = 0.5f;

	/** Is this in auto running state? */
	bool bAutoRunning = false;

	/** Is mouse hovering above target? */
	bool bIsTargeting = false;

	/** Is shift pressed */
	bool bShiftKeyDown = false;

	/** How close this can get to the destination */
	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;

	/** Curve on which this is auto running */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	/** Widget spawned to show dealt damage */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;
};
