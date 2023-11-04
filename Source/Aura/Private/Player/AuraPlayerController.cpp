// Copyright Maxjestic


#include "Player/AuraPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick( float DeltaTime )
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(AuraContext, 0);

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

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move (const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if(APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if(!CursorHit.bBlockingHit) return;

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
	
	if(LastActor == nullptr)
	{
		if(CurrentActor == nullptr)
		{
			// Case 1)			
		}
		else
		{
			// Case 2)
			CurrentActor->HighlightActor();
		}
	}
	else
	{
		if(CurrentActor == nullptr)
		{
			// Case 3)
			LastActor->UnhighlightActor();
		}
		else
		{
			if(LastActor != CurrentActor)
			{
				// Case 4
				LastActor->UnhighlightActor();
				CurrentActor->HighlightActor();
			}
			else
			{
				// Case 5
			}
		}
	}
}