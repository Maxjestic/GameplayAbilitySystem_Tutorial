// Copyright Maxjestic


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Actor/MagicCircle.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Character.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "UI/Widget/DamageTextComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
	Spline = CreateDefaultSubobject<USplineComponent>( "Spline" );
}

void AAuraPlayerController::PlayerTick( float DeltaTime )
{
	Super::PlayerTick( DeltaTime );

	CursorTrace();
	if (bAutoRunning)
	{
		AutoRun();
	}
	UpdateMagicCircleLocation();
}

void AAuraPlayerController::ShowMagicCircle()
{
	if (!IsValid( MagicCircle ))
	{
		MagicCircle = GetWorld()->SpawnActor<AMagicCircle>( MagicCircleClass );
	}
}

void AAuraPlayerController::HideMagicCircle() const
{
	if (IsValid( MagicCircle ))
	{
		MagicCircle->Destroy();
	}
}

void AAuraPlayerController::ShowDamageNumber_Implementation( const float Damage, ACharacter* TargetCharacter, const bool bBlockedHit,
                                                             const bool bCriticalHit )
{
	if (IsValid( TargetCharacter ) && DamageTextComponentClass && IsLocalController())
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>( TargetCharacter, DamageTextComponentClass );
		DamageText->RegisterComponent();
		DamageText->AttachToComponent( TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform );
		DamageText->DetachFromComponent( FDetachmentTransformRules::KeepWorldTransform );
		DamageText->SetDamageText( Damage, bBlockedHit, bCriticalHit );
	}
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check( AuraContext );

	// Default Input Component class set in Project Settings > Input > Default Classes
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer() ))
	{
		Subsystem->AddMappingContext( AuraContext, 0 );
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior( EMouseLockMode::DoNotLock );
	InputModeData.SetHideCursorDuringCapture( false );
	SetInputMode( InputModeData );
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>( InputComponent );

	AuraInputComponent->BindAction( MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move );
	AuraInputComponent->BindAction( ShiftAction, ETriggerEvent::Started, this, &AAuraPlayerController::ShiftPressed );
	AuraInputComponent->BindAction( ShiftAction, ETriggerEvent::Completed, this, &AAuraPlayerController::ShiftReleased );
	AuraInputComponent->BindAbilityActions( InputConfig,
	                                        this,
	                                        &ThisClass::AbilityInputTagPressed,
	                                        &ThisClass::AbilityInputTagReleased,
	                                        &ThisClass::AbilityInputTagHeld );
}

void AAuraPlayerController::Move( const FInputActionValue& InputActionValue )
{
	if (GetAbilitySystemComponent() &&
		GetAbilitySystemComponent()->HasMatchingGameplayTag( FAuraGameplayTags::Get().Player_Block_InputPressed ))
	{
		return;
	}

	bAutoRunning = false;

	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();

	// Define forward direction   
	const FRotator YawRotation( 0.f, GetControlRotation().Yaw, 0.f );

	const FVector ForwardDirection = FRotationMatrix( YawRotation ).GetUnitAxis( EAxis::X );
	const FVector RightDirection = FRotationMatrix( YawRotation ).GetUnitAxis( EAxis::Y );

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput( ForwardDirection, InputAxisVector.Y );
		ControlledPawn->AddMovementInput( RightDirection, InputAxisVector.X );
	}
}

void AAuraPlayerController::AutoRun()
{
	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(
			ControlledPawn->GetActorLocation(),
			ESplineCoordinateSpace::World );

		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(
			LocationOnSpline,
			ESplineCoordinateSpace::World );

		ControlledPawn->AddMovementInput( Direction );

		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

void AAuraPlayerController::CursorTrace()
{
	if (GetAbilitySystemComponent() &&
		GetAbilitySystemComponent()->HasMatchingGameplayTag( FAuraGameplayTags::Get().Player_Block_CursorTrace ))
	{
		if (PreviousActor)
		{
			PreviousActor->UnhighlightActor();
		}
		if (NewActor)
		{
			NewActor->UnhighlightActor();
		}
		PreviousActor = nullptr;
		NewActor = nullptr;
		return;
	}
	GetHitResultUnderCursor( ECC_Visibility, false, CursorHit );
	if (!CursorHit.bBlockingHit) return;

	PreviousActor = NewActor;
	NewActor = Cast<IEnemyInterface>( CursorHit.GetActor() );

	if (PreviousActor != NewActor)
	{
		if (PreviousActor)
		{
			PreviousActor->UnhighlightActor();
		}
		if (NewActor)
		{
			NewActor->HighlightActor();
		}
	}
}

void AAuraPlayerController::AbilityInputTagPressed( FGameplayTag InputTag )
{
	if (GetAbilitySystemComponent() &&
		GetAbilitySystemComponent()->HasMatchingGameplayTag( FAuraGameplayTags::Get().Player_Block_InputPressed ))
	{
		return;
	}
	if (InputTag.MatchesTagExact( FAuraGameplayTags::Get().InputTag_RMB ))
	{
		bIsTargeting = NewActor ? true : false;
		FollowTime = 0.f;
		bAutoRunning = false;
	}
	if (GetAbilitySystemComponent())
	{
		GetAbilitySystemComponent()->AbilityInputTagPressed( InputTag );
	}
}

void AAuraPlayerController::AbilityInputTagReleased( FGameplayTag InputTag )
{
	if (GetAbilitySystemComponent() &&
		GetAbilitySystemComponent()->HasMatchingGameplayTag( FAuraGameplayTags::Get().Player_Block_InputReleased ))
	{
		return;
	}
	// if it's not RMB we just try to activate ability
	if (!InputTag.MatchesTagExact( FAuraGameplayTags::Get().InputTag_RMB ))
	{
		if (GetAbilitySystemComponent())
		{
			GetAbilitySystemComponent()->AbilityInputTagReleased( InputTag );
		}
		return;
	}

	// We are informing ASC that we released the button
	if (GetAbilitySystemComponent())
	{
		GetAbilitySystemComponent()->AbilityInputTagReleased( InputTag );
	}

	// if it's RMB we check - is this not targeting and shift key is not pressed? true: start auto running to clicked destination
	if (!bIsTargeting && !bShiftKeyDown)
	{
		const APawn* ControllerPawn = GetPawn();
		if (FollowTime <= ShortPressThreshold && ControllerPawn)
		{
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(
				this,
				ControllerPawn->GetActorLocation(),
				CachedDestination ))
			{
				Spline->ClearSplinePoints();

				for (const FVector& Point : NavPath->PathPoints)
				{
					Spline->AddSplinePoint( Point, ESplineCoordinateSpace::World );
				}
				if (NavPath->PathPoints.Num() > 0)
				{
					CachedDestination = NavPath->PathPoints.Last();
					bAutoRunning = true;
				}
			}
			if (GetAbilitySystemComponent() &&
				!GetAbilitySystemComponent()->HasMatchingGameplayTag( FAuraGameplayTags::Get().Player_Block_InputPressed ))
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation( this, ClickNiagaraSystem, CachedDestination );
			}
		}
	}
}

void AAuraPlayerController::AbilityInputTagHeld( FGameplayTag InputTag )
{
	if (GetAbilitySystemComponent() &&
		GetAbilitySystemComponent()->HasMatchingGameplayTag( FAuraGameplayTags::Get().Player_Block_InputHeld ))
	{
		return;
	}
	// if it's not RMB we just try to activate ability
	if (!InputTag.MatchesTagExact( FAuraGameplayTags::Get().InputTag_RMB ))
	{
		if (GetAbilitySystemComponent())
		{
			GetAbilitySystemComponent()->AbilityInputTagHeld( InputTag );
		}
		return;
	}

	// it it's RMB we check - is this targeting or is shift key pressed? true: try to activate ability; false: run to cursor
	if (bIsTargeting || bShiftKeyDown)
	{
		if (GetAbilitySystemComponent())
		{
			GetAbilitySystemComponent()->AbilityInputTagHeld( InputTag );
		}
	}
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();

		if (CursorHit.bBlockingHit)
		{
			CachedDestination = CursorHit.ImpactPoint;
		}

		if (APawn* ControlledPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput( WorldDirection );
		}
	}
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetAbilitySystemComponent()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent( GetPawn<APawn>() ) );
	}
	return AuraAbilitySystemComponent;
}

void AAuraPlayerController::UpdateMagicCircleLocation()
{
	if (IsValid( MagicCircle ))
	{
		MagicCircle->SetActorLocation( CursorHit.ImpactPoint );
	}
}
