// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "Data/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilitySystemLibrary.generated.h"

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
	static bool MakeWidgetControllerParams( const UObject* WorldContextObject, FWidgetControllerParams& OutWidgetControllerParams, AAuraHUD*& OutAuraHUD );
	
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

	/** Returns amount of experience reward based on level and class of the character */
	static int32 GetExperienceRewardForClassAndLevel (const UObject* WorldContextObject, const ECharacterClass CharacterClass, const int32 CharacterLevel);
	
	/** Character Class Info data asset getter */
	UFUNCTION( BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefaults" )
	static UCharacterClassInfo* GetCharacterClassInfo( const UObject* WorldContextObject );

	/** Returns true if hit was blocked from FAuraGameplayEffectContext */
	UFUNCTION( BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects" )
	static bool IsBlockedHit( const FGameplayEffectContextHandle& EffectContextHandle );

	/** Returns true if hit was critical from FAuraGameplayEffectContext */
	UFUNCTION( BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects" )
	static bool IsCriticalHit( const FGameplayEffectContextHandle& EffectContextHandle );

	/** Sets if hit was blocked information in FAuraGameplayEffectContext */
	UFUNCTION( BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects" )
	static void SetIsBlockedHit( UPARAM( ref )FGameplayEffectContextHandle& EffectContextHandle, const bool bInIsBlockedHit );

	/** Sets if hit was critical information in FAuraGameplayEffectContext */
	UFUNCTION( BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects" )
	static void SetIsCriticalHit( UPARAM( ref )FGameplayEffectContextHandle& EffectContextHandle, const bool bInIsCriticalHit );

	/** Gets all alive players in given sphere */
	UFUNCTION( BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayMechanics" )
	static void GetLivePlayersWithinRadius( const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors,
	                                        const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin );

	/** Check if two actors are "friends" */
	UFUNCTION( BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayMechanics" )
	static bool IsNotFriend( const AActor* FirstActor, const AActor* SecondActor );

};
