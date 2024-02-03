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
	DOREPLIFETIME( AAuraPlayerState, AttributePoints )
	DOREPLIFETIME( AAuraPlayerState, SpellPoints )
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

void AAuraPlayerState::SetAttributePoints( const int32 InAttributePoints )
{
	AttributePoints = InAttributePoints;
	AttributePoints = FMath::Clamp( AttributePoints, 0, AttributePoints );
	OnAttributePointsChangedDelegate.Broadcast( AttributePoints );
}

void AAuraPlayerState::AddToAttributePoints( int32 InAttributePoints )
{
	InAttributePoints = FMath::Clamp( InAttributePoints, 0, InAttributePoints );
	AttributePoints += InAttributePoints;
	OnAttributePointsChangedDelegate.Broadcast( AttributePoints );
}

void AAuraPlayerState::SetSpellPoints( const int32 InSpellPoints )
{
	SpellPoints = InSpellPoints;
	SpellPoints = FMath::Clamp( SpellPoints, 0, SpellPoints );
	OnSpellPointsChangedDelegate.Broadcast( SpellPoints );
}

void AAuraPlayerState::AddToSpellPoints( int32 InSpellPoints )
{
	InSpellPoints = FMath::Clamp( InSpellPoints, 0, InSpellPoints );
	SpellPoints += InSpellPoints;
	OnSpellPointsChangedDelegate.Broadcast( SpellPoints );
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraPlayerState::OnRep_Level( int32 OldLevel )
{
	OnLevelChangedDelegate.Broadcast( Level );
}

void AAuraPlayerState::OnRep_Experience( int32 OldExperience )
{
	OnExperienceChangedDelegate.Broadcast( Experience );
}

void AAuraPlayerState::OnRep_AttributePoints( int32 OldAttributePoints )
{
	OnAttributePointsChangedDelegate.Broadcast( AttributePoints );
}

void AAuraPlayerState::OnRep_SpellPoints( int32 OldSpellPoints )
{
	OnSpellPointsChangedDelegate.Broadcast( SpellPoints );
}
