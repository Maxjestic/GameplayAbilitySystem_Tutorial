// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "DebuffNiagaraComponent.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UDebuffNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()

public:
	/**
	 * Default constructor
	 * disables auto activate
	 */
	UDebuffNiagaraComponent();

	/** Debuff tag associated with this */
	UPROPERTY( VisibleAnywhere )
	FGameplayTag DebuffTag;

protected:
	//~ Begin UActorComponent Interface
	virtual void BeginPlay() override;
	//~ End UActorComponent Interface

	/** Callback to when tags changes on the owner */
	void DebuffTagChanged( const FGameplayTag CallbackTag, int32 NewCount );
};
