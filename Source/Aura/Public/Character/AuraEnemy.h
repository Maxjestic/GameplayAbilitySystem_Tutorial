// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AuraEnemy.generated.h"

class UWidgetComponent;
/**
 * Class for AI-controlled characters
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	/** 
	 * Sets mesh collision, custom depth stencil value, AbilitySystemComponent (+replication), AttributeSet
	 * Creates health bar widget
	 */
	AAuraEnemy();

	//~ Begin IEnemy Interface
	virtual void HighlightActor() override;
	virtual void UnhighlightActor() override;
	//~ End IEnemy Interface

	//~ Begin ICombat Interface
	virtual int32 GetCharacterLevel() const override;
	virtual void Die() override;
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
	UPROPERTY( BlueprintReadOnly, Category="Combat" )
	bool bHitReacting = false;

	/** Base walk speed value */
	UPROPERTY( BlueprintReadOnly, Category="Combat" )
	float BaseWalkSpeed = 250.f;

	/** Life span value */
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category="Combat" )
	float LifeSpan = 5.f;

protected:
	//~ Begin AActor Interface
	virtual void BeginPlay() override;
	//~ End AActor Interface

	//~ Begin AAuraCharacterBase Interface
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;
	//~ End AAuraCharacterBase Interface

	/** Enemy level */
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults" )
	int32 Level = 1;

	/** Enemy Character class */
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults" )
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	/** Health bar above enemy */
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly )
	TObjectPtr<UWidgetComponent> HealthBar;

private:
	/** Sets controller, binds callbacks to delegates, broadcasts initial values */
	void SetupHealthBarWidget();
};
