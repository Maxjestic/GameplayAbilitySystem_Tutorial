// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/AuraDamageGameplayAbility.h"
#include "AuraFireBlast.generated.h"

class AAuraFireBall;
/**
 * 
 */
UCLASS()
class AURA_API UAuraFireBlast : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

public:
	//~ Begin UAuraGameplayAbility Interface
	virtual FString GetDescription(const int32 Level, const bool bForNextLevel) override;
	//~ End UAuraGameplayAbility Interface

	/** Spawns projectiles */
	UFUNCTION(BlueprintCallable)
	TArray<AAuraFireBall*> SpawnFireBalls();
	
protected:
	/** Number of fireballs to spawn */
	UPROPERTY( EditDefaultsOnly, Category="FireBlast" )
	int32 NumFireBalls = 12;

private:
	/** Projectile actor class to spawn */
	UPROPERTY( EditDefaultsOnly )
	TSubclassOf<AAuraFireBall> FireBallClass;
};
