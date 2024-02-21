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
	 * Primary Attribute Tags
	 */
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Resilience;
	FGameplayTag Attributes_Primary_Vigor;

	/**
	 * Secondary Attribute Tags
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
	 * Meta Attribute Tags
	 */	
	FGameplayTag Attributes_Meta_IncomingExperience;

	/**
	 * Input Tags
	 */
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;
	FGameplayTag InputTag_Passive_1;
	FGameplayTag InputTag_Passive_2;

	/**
	 * Damage Type Tags
	 */
	FGameplayTag Damage;
	FGameplayTag Damage_Arcane;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Lightning;
	FGameplayTag Damage_Physical;

	/**
	 * Resistance Attribute Tags
	 */
	FGameplayTag Attributes_Resistance_Arcane;
	FGameplayTag Attributes_Resistance_Fire;
	FGameplayTag Attributes_Resistance_Lightning;
	FGameplayTag Attributes_Resistance_Physical;

	/**
	 * Debuff Type Tags
	 */
	FGameplayTag Debuff_Arcane;
	FGameplayTag Debuff_Burn;
	FGameplayTag Debuff_Physical;
	FGameplayTag Debuff_Stun;

	/**
	 * Debuff Properties Tags
	 */
	FGameplayTag Debuff_Properties_Chance;
	FGameplayTag Debuff_Properties_Damage;
	FGameplayTag Debuff_Properties_Duration;
	FGameplayTag Debuff_Properties_Frequency;

	/**
	 * Ability Tags
	 */
	FGameplayTag Abilities_None;
	
	FGameplayTag Abilities_Attack;
	FGameplayTag Abilities_Summon;

	FGameplayTag Abilities_HitReact;
	
	FGameplayTag Abilities_Fire_FireBolt;
	
	FGameplayTag Abilities_Lightning_Electrocute;

	/**
	 * Ability Status Tags
	 */
	FGameplayTag Abilities_Status_Locked;
	FGameplayTag Abilities_Status_Eligible;
	FGameplayTag Abilities_Status_Unlocked;
	FGameplayTag Abilities_Status_Equipped;

	/**
	 * Ability Type Tags
	 */
	FGameplayTag Abilities_Type_Offensive;
	FGameplayTag Abilities_Type_Passive;
	FGameplayTag Abilities_Type_None;

	/**
	 * Cooldown Tags
	 */
	FGameplayTag Cooldown_Fire_FireBolt;

	/**
	 * Effects
	 */
	FGameplayTag Effects_HitReact;

	/**
	 * Combat Socket Tags
	 */
	FGameplayTag CombatSocket_Weapon;
	FGameplayTag CombatSocket_RightHand;
	FGameplayTag CombatSocket_LeftHand;
	FGameplayTag CombatSocket_Tail;

	/**
	 * Montage Tags
	 */
	FGameplayTag Montage_Attack_1;
	FGameplayTag Montage_Attack_2;
	FGameplayTag Montage_Attack_3;
	FGameplayTag Montage_Attack_4;

	/** Map assigning Damage Type Tags to Resistance Tags */
	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;

	/** Map assigning Damage Type Tags to Debuff Type Tags */
	TMap<FGameplayTag, FGameplayTag> DamageTypesToDebuffs;

private:
	/** The only instance of GameplayTags */
	static FAuraGameplayTags GameplayTags;
};
