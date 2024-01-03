// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * Singleton struct containing native gameplay tags, used to centralize gameplay tags used in code
 */
struct FAuraGameplayTags
{
public:
	/** Singleton getter of the only instance of FAuraGameplayTags */
	static const FAuraGameplayTags& Get() { return GameplayTags; }

	/** Initializes native gameplay tags */
	static void InitializeNativeGameplayTags();


	/**
	 * Primary Attributes Tags variables
	 */
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Resilience;
	FGameplayTag Attributes_Primary_Vigor;

	/**
	 * Secondary Attributes Tags variables
	 */
	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_ArmorPenetration;
	FGameplayTag Attributes_Secondary_BlockChance;
	FGameplayTag Attributes_Secondary_CriticalHitChance;
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	FGameplayTag Attributes_Secondary_CriticalHitResistance;
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	FGameplayTag Attributes_Secondary_ManaRegeneration;
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;

	/**
	 * Input Tags variables
	 */
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;

	/**
	 * Gameplay related
	 */
	FGameplayTag Damage;
	FGameplayTag Effects_HitReact;
	
private:
	/** The only instance of GameplayTags */
	static FAuraGameplayTags GameplayTags;
};
