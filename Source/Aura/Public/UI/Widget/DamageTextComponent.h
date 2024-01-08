// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "DamageTextComponent.generated.h"

/**
 * Widget used to display damage dealt to character
 */
UCLASS()
class AURA_API UDamageTextComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	/** Sets damage text displayed by this */
	UFUNCTION( BlueprintImplementableEvent, BlueprintCallable )
	void SetDamageText( float Damage, const bool bBlockedHit, const bool bCriticalHit );
};
