// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Data/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilitySystemLibrary.generated.h"

struct FDamageEffectParams;
class UAbilityInfo;
class AAuraHUD;
struct FWidgetControllerParams;
class USpellMenuWidgetController;
struct FGameplayEffectContextHandle;
class UAbilitySystemComponent;
class UAttributeMenuWidgetController;
class UOverlayWidgetController;

/**
 * Set of blueprint functions related to GAS used in Aura
 */
UCLASS()
class AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Creates Widget Controller Params, returns false if failed */
	UFUNCTION( BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject") )
	static bool MakeWidgetControllerParams( const UObject* WorldContextObject, FWidgetControllerParams& OutWidgetControllerParams,
	                                        AAuraHUD*& OutAuraHUD );

	/** Get overlay widget controller getter, may return nullptr */
	UFUNCTION( BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject") )
	static UOverlayWidgetController* GetOverlayWidgetController( const UObject* WorldContextObject );

	/** Get attribute menu widget controller, may return nullptr */
	UFUNCTION( BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject") )
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController( const UObject* WorldContextObject );

	/** Get spell menu widget controller, may return nullptr */
	UFUNCTION( BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject") )
	static USpellMenuWidgetController* GetSpellMenuWidgetController( const UObject* WorldContextObject );

	/** Initializes default attribute values for enemies */
	UFUNCTION( BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefaults" )
	static void InitializeDefaultAttributes( const UObject* WorldContextObject, ECharacterClass CharacterClass,
	                                         float Level, UAbilitySystemComponent* AbilitySystemComponent );

	/** Initializes startup abilities for enemy */
	UFUNCTION( BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefaults" )
	static void GiveStartupAbilities( const UObject* WorldContextObject,
	                                  UAbilitySystemComponent* AbilitySystemComponent, ECharacterClass CharacterClass );

	/** Character Class Info data asset getter */
	UFUNCTION( BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefaults" )
	static UCharacterClassInfo* GetCharacterClassInfo( const UObject* WorldContextObject );

	/** Character Class Info data asset getter */
	UFUNCTION( BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefaults" )
	static UAbilityInfo* GetAbilityInfo( const UObject* WorldContextObject );

	/** Returns true if hit was blocked from FAuraGameplayEffectContext */
	UFUNCTION( BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects" )
	static bool IsBlockedHit( const FGameplayEffectContextHandle& EffectContextHandle );

	/** Returns true if hit was critical from FAuraGameplayEffectContext */
	UFUNCTION( BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects" )
	static bool IsCriticalHit( const FGameplayEffectContextHandle& EffectContextHandle );

	/** Returns true if debuff was successful from FAuraGameplayEffectContext */
	UFUNCTION( BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects" )
	static bool IsSuccessfulDebuff( const FGameplayEffectContextHandle& EffectContextHandle );

	/** Returns debuff damage from FAuraGameplayEffectContext */
	UFUNCTION( BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects" )
	static float GetDebuffDamage( const FGameplayEffectContextHandle& EffectContextHandle );

	/** Returns debuff duration from FAuraGameplayEffectContext */
	UFUNCTION( BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects" )
	static float GetDebuffDuration( const FGameplayEffectContextHandle& EffectContextHandle );

	/** Returns debuff frequency from FAuraGameplayEffectContext */
	UFUNCTION( BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects" )
	static float GetDebuffFrequency( const FGameplayEffectContextHandle& EffectContextHandle );

	/** Returns damage effect's damage type from FAuraGameplayEffectContext */
	UFUNCTION( BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects" )
	static FGameplayTag GetDamageType( const FGameplayEffectContextHandle& EffectContextHandle );

	/** Returns damage effect's death impulse from FAuraGameplayEffectContext */
	UFUNCTION( BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects" )
	static FVector GetDeathImpulse( const FGameplayEffectContextHandle& EffectContextHandle );

	/** Returns damage effect's knockback impulse from FAuraGameplayEffectContext */
	UFUNCTION( BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects" )
	static FVector GetKnockbackForce( const FGameplayEffectContextHandle& EffectContextHandle );

	/** Sets if hit was blocked in FAuraGameplayEffectContext */
	UFUNCTION( BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects" )
	static void SetIsBlockedHit( UPARAM( ref )FGameplayEffectContextHandle& EffectContextHandle, const bool bInIsBlockedHit );

	/** Sets if hit was critical in FAuraGameplayEffectContext */
	UFUNCTION( BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects" )
	static void SetIsCriticalHit( UPARAM( ref )FGameplayEffectContextHandle& EffectContextHandle, const bool bInIsCriticalHit );

	/** Sets if debuff was successful in FAuraGameplayEffectContext */
	UFUNCTION( BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects" )
	static void SetIsSuccessfulDebuff( UPARAM( ref )FGameplayEffectContextHandle& EffectContextHandle, const bool bInIsSuccessfulDebuff );

	/** Sets debuff damage in FAuraGameplayEffectContext */
	UFUNCTION( BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects" )
	static void SetDebuffDamage( UPARAM( ref )FGameplayEffectContextHandle& EffectContextHandle, const float InDebuffDamage );

	/** Sets debuff duration in FAuraGameplayEffectContext */
	UFUNCTION( BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects" )
	static void SetDebuffDuration( UPARAM( ref )FGameplayEffectContextHandle& EffectContextHandle, const float InDebuffDuration );

	/** Sets debuff frequency in FAuraGameplayEffectContext */
	UFUNCTION( BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects" )
	static void SetDebuffFrequency( UPARAM( ref )FGameplayEffectContextHandle& EffectContextHandle, const float InDebuffFrequency );

	/** Sets damage effect damage type in FAuraGameplayEffectContext */
	UFUNCTION( BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects" )
	static void SetDamageType( UPARAM( ref )FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType );

	/** Sets damage effect death impulse in FAuraGameplayEffectContext */
	UFUNCTION( BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects" )
	static void SetDeathImpulse( UPARAM( ref )FGameplayEffectContextHandle& EffectContextHandle, const FVector& InDeathImpulse );

	/** Sets damage effect knockback impulse in FAuraGameplayEffectContext */
	UFUNCTION( BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects" )
	static void SetKnockbackForce( UPARAM( ref )FGameplayEffectContextHandle& EffectContextHandle, const FVector& InKnockbackForce );

	/** Gets all alive players in given sphere */
	UFUNCTION( BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayMechanics" )
	static void GetLivePlayersWithinRadius( const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors,
	                                        const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin );

	/** Check if two actors are "friends" */
	UFUNCTION( BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayMechanics" )
	static bool IsNotFriend( const AActor* FirstActor, const AActor* SecondActor );

	/** Applies damage effect carried over in FDamageEffectParams */
	UFUNCTION( BlueprintCallable, Category = "AuraAbilitySystemLibrary|DamageEffects" )
	static FGameplayEffectContextHandle ApplyDamageEffect( const FDamageEffectParams& DamageEffectParams );

	/** Returns amount of experience reward based on level and class of the character */
	static int32 GetExperienceRewardForClassAndLevel( const UObject* WorldContextObject, const ECharacterClass CharacterClass,
	                                                  const int32 CharacterLevel );
};
