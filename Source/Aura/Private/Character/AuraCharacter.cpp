// Copyright Maxjestic


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator( 0.f, 400.f, 0.f );
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	CharacterClass = ECharacterClass::Elementalist;
}

void AAuraCharacter::PossessedBy( AController* NewController )
{
	Super::PossessedBy( NewController );

	// Init ability actor info for the server
	InitAbilityActorInfo();
	AddCharacterAbilities();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the client
	InitAbilityActorInfo();
}

int32 AAuraCharacter::GetCharacterLevel_Implementation() const
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check( AuraPlayerState );
	return AuraPlayerState->GetPlayerLevel();
}

int32 AAuraCharacter::GetExperience_Implementation() const
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check( AuraPlayerState );

	return AuraPlayerState->GetPlayerExperience();
}

void AAuraCharacter::AddExperience_Implementation( const int32 InExperience )
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check( AuraPlayerState );

	AuraPlayerState->AddToExperience( InExperience );
}

int32 AAuraCharacter::FindLevelForExperience_Implementation( const int32 InExperience ) const
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check( AuraPlayerState );

	return AuraPlayerState->LevelUpInfo->FindLevelForExperience( InExperience );
}

void AAuraCharacter::LevelUp_Implementation()
{
}

void AAuraCharacter::AddToPlayerLevel_Implementation( const int32 InPlayerLevel )
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check( AuraPlayerState );

	AuraPlayerState->AddToLevel( InPlayerLevel );
}

int32 AAuraCharacter::GetAttributePointsReward_Implementation( const int32 PlayerLevel ) const
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check( AuraPlayerState );

	return AuraPlayerState->LevelUpInfo->LevelUpInformation[PlayerLevel].AttributePointReward;
}

void AAuraCharacter::AddToAttributePoints_Implementation( int32 InAttributePoints )
{
	// TODO: Add attribute points to player state
}

int32 AAuraCharacter::GetSpellPointsReward_Implementation( const int32 PlayerLevel ) const
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check( AuraPlayerState );

	return AuraPlayerState->LevelUpInfo->LevelUpInformation[PlayerLevel].SpellPointReward;
}

void AAuraCharacter::AddToSpellPoints_Implementation( int32 InSpellPoints )
{
	// TODO: Add spell points to player state
}

void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check( AuraPlayerState );
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo( AuraPlayerState, this );

	Cast<UAuraAbilitySystemComponent>( AuraPlayerState->GetAbilitySystemComponent() )->AbilityActorInfoSet();

	// Set AbilitySystemComponent and AttributeSet
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();

	if (AAuraPlayerController* AuraPlayerController = GetController<AAuraPlayerController>())
	//Cast<AAuraPlayerController>(GetController()))
	{
		if (AAuraHUD* AuraHUD = AuraPlayerController->GetHUD<AAuraHUD>())
		//Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			AuraHUD->InitOverlay( AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet );
		}
	}

	InitializeDefaultAttributes();
}

void AAuraCharacter::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf( DefaultPrimaryAttributes );
	ApplyEffectToSelf( DefaultSecondaryAttributes );
	ApplyEffectToSelf( DefaultVitalAttributes );
}
