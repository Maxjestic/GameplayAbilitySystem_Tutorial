// Copyright Maxjestic


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AAuraPlayerController::PlayerTick( float DeltaTime )
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();

	if(bAutoRunning)
	{		
		AutoRun();
	}
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);

	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed,
	                                       &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AAuraPlayerController::Move( const FInputActionValue& InputActionValue )
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAuraPlayerController::AutoRun()
{
	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(
			ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(
			LocationOnSpline, ESplineCoordinateSpace::World);
		
		ControlledPawn->AddMovementInput(Direction);

		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if(DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = CurrentActor;
	CurrentActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	/*
	 * Line trace from cursor.
	 * Possible scenarios:
	 *		1) Current and Last actors are null
	 *			- Do nothing
	 *		2) Last actor is null and current actor is valid
	 *			- highlight current actor
	 *		3) Last actor is valid and current actor is null
	 *			- Unhighlight last actor
	 *		4) Both actors are valid, but they are different
	 *			- Unhighlight last actor and highlight current actor
	 *		5) Both actors are valid, they are the same actor
	 *			- Do nothing
	 */

	if (LastActor == nullptr)
	{
		if (CurrentActor == nullptr)
		{
			// Case 1)
			bTargeting = false;
		}
		else
		{
			// Case 2)
			CurrentActor->HighlightActor();
			bTargeting = true;
		}
	}
	else
	{
		if (CurrentActor == nullptr)
		{
			// Case 3)
			LastActor->UnhighlightActor();
			bTargeting = false;
		}
		else
		{
			if (LastActor != CurrentActor)
			{
				// Case 4
				LastActor->UnhighlightActor();
				CurrentActor->HighlightActor();
				bTargeting = true;
			}
			else
			{
				// Case 5
			}
		}
	}
}

void AAuraPlayerController::AbilityInputTagPressed( FGameplayTag InputTag )
{
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_RMB))
	{
		FollowTime = 0.f;
		bAutoRunning = false;
	}
}

void AAuraPlayerController::AbilityInputTagReleased( FGameplayTag InputTag )
{
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_RMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagReleased(InputTag);
		}
		return;
	}

	if (bTargeting)
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagReleased(InputTag);
		}
	}
	else
	{
		const APawn* ControllerPawn = GetPawn();
		if (FollowTime <= ShortPressThreshold && ControllerPawn)
		{
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(
				this, ControllerPawn->GetActorLocation(), CachedDestination))
			{
				Spline->ClearSplinePoints();
				
				for (const FVector& Point : NavPath->PathPoints)
				{
					Spline->AddSplinePoint(Point, ESplineCoordinateSpace::World);
					DrawDebugSphere(GetWorld(), Point, 10.f, 10.f, FColor::Red, false, 10.f);
				}
				if (NavPath->PathPoints.Num() >0 )
				{
					CachedDestination = NavPath->PathPoints.Last();
					bAutoRunning = true;
				}
			}
		}
	}
}

void AAuraPlayerController::AbilityInputTagHeld( FGameplayTag InputTag )
{
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_RMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
		return;
	}

	if (bTargeting)
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
	}
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();

		FHitResult Hit;
		if (GetHitResultUnderCursor(ECC_Visibility, false, Hit))
		{
			CachedDestination = Hit.ImpactPoint;
		}

		if (APawn* ControlledPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AuraAbilitySystemComponent;
}