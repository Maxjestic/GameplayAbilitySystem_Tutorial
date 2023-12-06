// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"

/**
 * Class for AI-controlled characters
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	/** No parameter constructor
	 *  Sets: mesh collision, custom depth stencil value,
	 *  AbilitySystemComponent (+replication), AttributeSet */
	AAuraEnemy();

	//~ Begin IEnemy Interface
	virtual void HighlightActor() override;
	virtual void UnhighlightActor() override;
	//~ End IEnemy Interface

	//~ Begin ICombat Interface
	virtual int32 GetCharacterLevel() override;
	//~ End ICombat Interface
	
protected:
	//~ Begin AActor Interface
	virtual void BeginPlay() override;
	//~ End AActor Interface

	//~ Begin AAuraCharacterBase Interface
	virtual void InitAbilityActorInfo() override;
	//~ End AAuraCharacterBase Interface

	/** Enemy level */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;
};
