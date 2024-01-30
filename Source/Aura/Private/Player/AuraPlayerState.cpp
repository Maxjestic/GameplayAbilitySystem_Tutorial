// Copyright Maxjestic


#include "Player/AuraPlayerState.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Net/UnrealNetwork.h"

AAuraPlayerState::AAuraPlayerState()
{
	NetUpdateFrequency = 100.f;

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>( "AbilitySystemComponent" );
	AbilitySystemComponent->SetIsReplicated( true );
	AbilitySystemComponent->SetReplicationMode( EGameplayEffectReplicationMode::Mixed );

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>( "AttributeSet" );
}

void AAuraPlayerState::GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );

	DOREPLIFETIME( AAuraPlayerState, Level )
	DOREPLIFETIME( AAuraPlayerState, Experience )
}

void AAuraPlayerState::SetLevel( const int32 InLevel )
{
	Level = InLevel;
	Level = FMath::Clamp( Level, 0, Level );
	OnLevelChangedDelegate.Broadcast( Level );
}

void AAuraPlayerState::AddToLevel( int32 InLevel )
{
	InLevel = FMath::Clamp( InLevel, 0, InLevel );
	Level += InLevel;
	OnLevelChangedDelegate.Broadcast( Level );
}

void AAuraPlayerState::SetExperience( const int32 InExperience )
{
	Experience = InExperience;
	Experience = FMath::Clamp( Experience, 0, Experience );
	OnExperienceChangedDelegate.Broadcast( Experience );
}

void AAuraPlayerState::AddToExperience( int32 InExperience )
{
	InExperience = FMath::Clamp( InExperience, 0, InExperience );
	Experience += InExperience;
	OnExperienceChangedDelegate.Broadcast( Experience );
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraPlayerState::OnRep_Level( int32 OldLevel )
{
}

void AAuraPlayerState::OnRep_Experience( int32 OldExperience )
{
	OnExperienceChangedDelegate.Broadcast( Experience );
}
