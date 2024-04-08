// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "Interaction/HighlightInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AuraEnemy.generated.h"

class AAuraAIController;
class UBehaviorTree;
class UWidgetComponent;
/**
 * Class for AI-controlled characters
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface, public IHighlightInterface
{
	GENERATED_BODY()

public:
	/** 
	 * Sets mesh collision, custom depth stencil value, AbilitySystemComponent (+replication), AttributeSet, controller rotation
	 * Creates health bar widget
	 */
	AAuraEnemy();

	//~ Begin IEnemy Interface
	virtual void SetCombatTarget_Implementation( AActor* InTargetActor ) override;
	virtual AActor* GetCombatTarget_Implementation() override;
	//~ End IEnemy Interface

	//~ Begin IHighlight Interface	
	virtual void HighlightActor() override;
	virtual void UnhighlightActor() override;
	//~ End IHighlight Interface

	//~ Begin APawn Interface
	virtual void PossessedBy( AController* NewController ) override;
	//~ End APawn Interface

	//~ Begin ICombat Interface
	virtual int32 GetCharacterLevel_Implementation() const override;
	virtual void Die( const FVector& DeathImpulse ) override;
	//~ End ICombat Interface

	/** Callback responding to Effects.HitReact tag change */
	void HitReactTagChanged( const FGameplayTag CallbackTag, int32 NewCount );

	/** Delegate to broadcast when health changes */
	UPROPERTY( BlueprintAssignable )
	FOnAttributeChangedSignature OnHealthChanged;

	/** Delegate to broadcast when health changes */
	UPROPERTY( BlueprintAssignable )
	FOnAttributeChangedSignature OnMaxHealthChanged;

	/** Is this hit reacting */
	UPROPERTY( BlueprintReadOnly, Category = "Combat" )
	bool bHitReacting = false;

	/** Life span value */
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Combat" )
	float LifeSpan = 5.f;

	/** What this is currently targeting */
	UPROPERTY( BlueprintReadWrite, Category = "Combat" )
	TObjectPtr<AActor> CombatTarget;

protected:
	//~ Begin AActor Interface
	virtual void BeginPlay() override;
	//~ End AActor Interface

	//~ Begin AAuraCharacterBase Interface
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;
	virtual void StunTagChanged( const FGameplayTag CallbackTag, int32 NewCount ) override;
	//~ End AAuraCharacterBase Interface

	/** Enemy level */
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults" )
	int32 Level = 1;

	/** Health bar above enemy */
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly )
	TObjectPtr<UWidgetComponent> HealthBar;

	/** Behavior tree used by this */
	UPROPERTY( EditAnywhere, Category = "AI" )
	TObjectPtr<UBehaviorTree> BehaviorTree;

	/** AI controller of this */
	UPROPERTY()
	TObjectPtr<AAuraAIController> AuraAIController;

private:
	/** Sets controller, binds callbacks to delegates, broadcasts initial values */
	void SetupHealthBarWidget();
};
