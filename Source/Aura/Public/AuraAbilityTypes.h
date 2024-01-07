#pragma once

#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

/**
 * GameplayEffectContext with additional Aura related data
 */
USTRUCT( BlueprintType )
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	/** Simple accessors to Aura related data */
	bool IsBlockedHit() const { return bIsBlockedHit; }
	bool IsCriticalHit() const { return bIsCriticalHit; }
	
	void SetIsBlockedHit( bool const bInIsBlockedHit ) { bIsBlockedHit = bInIsBlockedHit; }
	void SetIsCriticalHit( bool const bInIsCriticalHit ) { bIsCriticalHit = bInIsCriticalHit; }

	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FGameplayEffectContext::StaticStruct();
	}

	/** Custom serialization, subclasses must override this */
	virtual bool NetSerialize( FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess ) override;

protected:
	/** True if Hit was Blocked */
	UPROPERTY()
	bool bIsBlockedHit = false;

	/** true if Hit was Critical */
	UPROPERTY()
	bool bIsCriticalHit = false;
};
