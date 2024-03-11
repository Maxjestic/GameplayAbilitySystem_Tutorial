// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "PassiveNiagaraComponent.generated.h"

/**
 * Niagara component for passive abilities
 */
UCLASS()
class AURA_API UPassiveNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()

public:
	/**
	 * Default constructor
	 * sets bAutoActivate
	 */
	UPassiveNiagaraComponent();

	/** Gameplay tag related my ability */
	UPROPERTY( EditDefaultsOnly )
	FGameplayTag PassiveSpellTag;

protected:
	//~ Begin UActorComponent Interface
	virtual void BeginPlay() override;
	//~ End UActorComponent Interface

	/** Callback to on when passive ability is activated/deactivated */
	void OnPassiveActivate(const FGameplayTag& AbilityTag, const bool bActivate);
};
