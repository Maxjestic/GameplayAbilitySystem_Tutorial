// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

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
	AAuraPlayerController();

	//~ Begin APlayerController Interface
	virtual void PlayerTick(float DeltaTime) override;
	//~ End APlayerController Interface
	
protected:
	//~ Begin AActor Interface
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	//~ End AActor Interface
	
private:
	/** Callback function implementing movement */
	void Move(const FInputActionValue& InputActionValue);

	/** Implementation of movement using points on spline generated in response to given Input Action */
	void AutoRun();

	/** Tracing under cursor to target actors */
	void CursorTrace();
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	/** A getter for AbilitySystemComponent */
	UAuraAbilitySystemComponent* GetASC();

	/** Input Mapping Context used to map input */
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	/** Input Action property used to move character */
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;
	
	IEnemyInterface* LastActor;
	IEnemyInterface* CurrentActor;

	FHitResult CursorHit;
	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false;
	bool bIsTargeting = false;

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;
};
