// Copyright Maxjestic


#include "Character/AuraEnemy.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AI/AuraAIController.h"
#include "Aura/Aura.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/Widget/AuraUserWidget.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel( ECC_Visibility, ECR_Block );

	GetMesh()->SetCustomDepthStencilValue( CUSTOM_DEPTH_RED );
	Weapon->SetCustomDepthStencilValue( CUSTOM_DEPTH_RED );

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>( "AbilitySystemComponent" );
	AbilitySystemComponent->SetIsReplicated( true );
	AbilitySystemComponent->SetReplicationMode( EGameplayEffectReplicationMode::Minimal );

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>( "AttributeSet" );

	HealthBar = CreateDefaultSubobject<UWidgetComponent>( "HealthBar" );
	HealthBar->SetupAttachment( GetRootComponent() );

	BaseWalkSpeed = 250.f;
}

void AAuraEnemy::HighlightActor_Implementation()
{
	GetMesh()->SetRenderCustomDepth( true );
	Weapon->SetRenderCustomDepth( true );
}

void AAuraEnemy::UnHighlightActor_Implementation()
{
	GetMesh()->SetRenderCustomDepth( false );
	Weapon->SetRenderCustomDepth( false );
}

void AAuraEnemy::SetCombatTarget_Implementation( AActor* InTargetActor )
{
	CombatTarget = InTargetActor;
}

AActor* AAuraEnemy::GetCombatTarget_Implementation()
{
	return CombatTarget;
}

void AAuraEnemy::PossessedBy( AController* NewController )
{
	Super::PossessedBy( NewController );

	if (!HasAuthority())
	{
		return;
	}

	AuraAIController = Cast<AAuraAIController>( NewController );
	AuraAIController->GetBlackboardComponent()->InitializeBlackboard( *BehaviorTree->BlackboardAsset );
	AuraAIController->RunBehaviorTree( BehaviorTree );
	AuraAIController->GetBlackboardComponent()->SetValueAsBool( FName( "HitReacting" ), false );
	AuraAIController->GetBlackboardComponent()->SetValueAsBool( FName( "RangedAttacker" ), CharacterClass != ECharacterClass::Warrior );
}

int32 AAuraEnemy::GetCharacterLevel_Implementation() const
{
	return Level;
}

void AAuraEnemy::Die( const FVector& DeathImpulse )
{
	SetLifeSpan( LifeSpan );
	if (AuraAIController)
	{
		AuraAIController->GetBlackboardComponent()->SetValueAsBool( FName( "Dead" ), true );
	}
	Super::Die( DeathImpulse );
}

void AAuraEnemy::HitReactTagChanged( const FGameplayTag CallbackTag, int32 NewCount )
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	if (AuraAIController && AuraAIController->GetBlackboardComponent())
	{
		AuraAIController->GetBlackboardComponent()->SetValueAsBool( FName( "HitReacting" ), bHitReacting );
	}
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	InitAbilityActorInfo();
	if (HasAuthority())
	{
		UAuraAbilitySystemLibrary::GiveStartupAbilities( this, AbilitySystemComponent, CharacterClass );
	}
	SetupHealthBarWidget();
}

void AAuraEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo( this, this );
	Cast<UAuraAbilitySystemComponent>( AbilitySystemComponent )->AbilityActorInfoSet();

	if (HasAuthority())
	{
		InitializeDefaultAttributes();
	}

	OnAbilitySystemComponentRegistered.Broadcast( AbilitySystemComponent );
	AbilitySystemComponent->RegisterGameplayTagEvent( FAuraGameplayTags::Get().Debuff_Stun, EGameplayTagEventType::NewOrRemoved ).AddUObject( this, &AAuraEnemy::StunTagChanged );
}

void AAuraEnemy::InitializeDefaultAttributes() const
{
	UAuraAbilitySystemLibrary::InitializeDefaultAttributes( this, CharacterClass, Level, AbilitySystemComponent );
}

void AAuraEnemy::StunTagChanged( const FGameplayTag CallbackTag, int32 NewCount )
{
	Super::StunTagChanged( CallbackTag, NewCount );
	if (AuraAIController && AuraAIController->GetBlackboardComponent())
	{
		AuraAIController->GetBlackboardComponent()->SetValueAsBool( FName( "Stunned" ), bIsStunned );
	}
}

void AAuraEnemy::SetupHealthBarWidget()
{
	// Set widget controller, enemy is the controller in this case
	if (UAuraUserWidget* AuraUserWidget = Cast<UAuraUserWidget>( HealthBar->GetUserWidgetObject() ))
	{
		AuraUserWidget->SetWidgetController( this );
	}

	// bind callbacks to delegates
	if (const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>( AttributeSet ))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate( AuraAttributeSet->GetHealthAttribute() ).
		                        AddLambda(
			                        [this]( const FOnAttributeChangeData& Data )
			                        {
				                        OnHealthChanged.Broadcast( Data.NewValue );
			                        }
		                        );
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate( AuraAttributeSet->GetMaxHealthAttribute() ).
		                        AddLambda(
			                        [this]( const FOnAttributeChangeData& Data )
			                        {
				                        OnMaxHealthChanged.Broadcast( Data.NewValue );
			                        }
		                        );

		AbilitySystemComponent->RegisterGameplayTagEvent( FAuraGameplayTags::Get().Effects_HitReact,
		                                                  EGameplayTagEventType::NewOrRemoved ).AddUObject(
			this,
			&AAuraEnemy::HitReactTagChanged
		);

		// Broadcast initial values
		OnHealthChanged.Broadcast( AuraAttributeSet->GetHealth() );
		OnMaxHealthChanged.Broadcast( AuraAttributeSet->GetMaxHealth() );
	}
}
