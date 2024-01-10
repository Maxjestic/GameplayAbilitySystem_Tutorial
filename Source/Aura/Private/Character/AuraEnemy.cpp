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

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>( "AttributeSet" );

	HealthBar = CreateDefaultSubobject<UWidgetComponent>( "HealthBar" );
	HealthBar->SetupAttachment( GetRootComponent() );
}

void AAuraEnemy::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth( true );
	Weapon->SetRenderCustomDepth( true );
}

void AAuraEnemy::UnhighlightActor()
{
	GetMesh()->SetRenderCustomDepth( false );
	Weapon->SetRenderCustomDepth( false );
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
}

int32 AAuraEnemy::GetCharacterLevel() const
{
	return Level;
}

void AAuraEnemy::Die()
{
	SetLifeSpan( LifeSpan );
	Super::Die();
}

void AAuraEnemy::HitReactTagChanged( const FGameplayTag CallbackTag, int32 NewCount )
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	InitAbilityActorInfo();
	if (HasAuthority())
	{
		UAuraAbilitySystemLibrary::GiveStartupAbilities( this, AbilitySystemComponent );
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
}

void AAuraEnemy::InitializeDefaultAttributes() const
{
	UAuraAbilitySystemLibrary::InitializeDefaultAttributes( this, CharacterClass, Level, AbilitySystemComponent );
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
