// Copyright Maxjestic


#include "AuraGameplayTags.h"

#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::GameplayTags;

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	/**
	 * Primary Attribute Tags
	 */
	GameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "Attributes.Primary.Intelligence" ),
		FString( "Increases Magical Damage" ) );

	GameplayTags.Attributes_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "Attributes.Primary.Resilience" ),
		FString( "Increases Armor and Armor Penetration" ) );

	GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "Attributes.Primary.Strength" ),
		FString( "Increases Physical Damage" ) );

	GameplayTags.Attributes_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "Attributes.Primary.Vigor" ),
		FString( "Increases Health" ) );

	/**
	 * Secondary Attribute Tags
	 */
	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "Attributes.Secondary.Armor" ),
		FString( "Reduces Damage taken, increases Block Chance" ) );

	GameplayTags.Attributes_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "Attributes.Secondary.ArmorPenetration" ),
		FString( "Ignores percentage of enemy Armor, increases Critical Hit Chance" ) );

	GameplayTags.Attributes_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "Attributes.Secondary.BlockChance" ),
		FString( "Chance to cut incoming damage in half" ) );

	GameplayTags.Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "Attributes.Secondary.CriticalHitChance" ),
		FString( "Chance to double Damage plus Critical Hit bonus" ) );

	GameplayTags.Attributes_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "Attributes.Secondary.CriticalHitDamage" ),
		FString( "Bonus Damage added when Critical Hit is scored" ) );

	GameplayTags.Attributes_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "Attributes.Secondary.CriticalHitResistance" ),
		FString( "Reduces Critical Hit Chance of attacking enemies" ) );

	GameplayTags.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "Attributes.Secondary.HealthRegeneration" ),
		FString( "Amount of Health regenerated every second" ) );

	GameplayTags.Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "Attributes.Secondary.ManaRegeneration" ),
		FString( "Amount of Mana regenerated every second" ) );

	GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "Attributes.Secondary.MaxHealth" ),
		FString( "Maximum amount of Health obtainable" ) );

	GameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "Attributes.Secondary.MaxMana" ),
		FString( "Maximum amount of Mana obtainable" ) );

	/**
	 * Meta Attribute Tags
	 */
	GameplayTags.Attributes_Meta_IncomingExperience = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "Attributes.Meta.IncomingExperience" ),
		FString( "Incoming Experience Meta Attribute" ) );

	/**
	 * Input Tags
	 */
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "InputTag.LMB" ),
		FString( "Input Tag for Left Mouse Button" ) );

	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "InputTag.RMB" ),
		FString( "Input Tag for Right Mouse Button" ) );

	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "InputTag.1" ),
		FString( "Input Tag for 1 key" ) );

	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "InputTag.2" ),
		FString( "Input Tag for 2 key" ) );

	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "InputTag.3" ),
		FString( "Input Tag for 3 key" ) );

	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "InputTag.4" ),
		FString( "Input Tag for 4 key" ) );

	GameplayTags.InputTag_Passive_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "InputTag.Passive.1" ),
		FString( "Input Tag for Passive Ability 1" ) );

	GameplayTags.InputTag_Passive_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "InputTag.Passive.2" ),
		FString( "Input Tag for Passive Ability 2" ) );

	/**
	 * Damage Type Tags
	 */
	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "Damage" ),
		FString( "Damage" ) );

	GameplayTags.Damage_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "Damage.Arcane" ),
		FString( "Arcane Damage Type" ) );

	GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "Damage.Fire" ),
		FString( "Fire Damage Type" ) );

	GameplayTags.Damage_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "Damage.Lightning" ),
		FString( "Lightning Damage Type" ) );

	GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "Damage.Physical" ),
		FString( "Physical Damage Type" ) );

	/**
	 * Resistance Attributes Tags
	*/
	GameplayTags.Attributes_Resistance_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "Attributes.Resistance.Arcane" ),
		FString( "Resistance to Arcane Damage" ) );

	GameplayTags.Attributes_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "Attributes.Resistance.Fire" ),
		FString( "Resistance to Fire Damage" ) );

	GameplayTags.Attributes_Resistance_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "Attributes.Resistance.Lightning" ),
		FString( "Resistance to Lightning Damage" ) );

	GameplayTags.Attributes_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "Attributes.Resistance.Physical" ),
		FString( "Resistance to Physical Damage" ) );

	/**
	 * Map of Damage Type Tags to Resistance Tags 
	 */
	GameplayTags.DamageTypesToResistances.Add( GameplayTags.Damage_Arcane, GameplayTags.Attributes_Resistance_Arcane );
	GameplayTags.DamageTypesToResistances.Add( GameplayTags.Damage_Fire, GameplayTags.Attributes_Resistance_Fire );
	GameplayTags.DamageTypesToResistances.Add( GameplayTags.Damage_Lightning, GameplayTags.Attributes_Resistance_Lightning );
	GameplayTags.DamageTypesToResistances.Add( GameplayTags.Damage_Physical, GameplayTags.Attributes_Resistance_Physical );

	/**
	 * Debuff Type Tags
	 */
	GameplayTags.Debuff_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "Debuff.Arcane" ),
		FString( "Debuff for Arcane Damage" ) );
	
	GameplayTags.Debuff_Burn = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "Debuff.Burn" ),
		FString( "Debuff for Fire Damage" ) );
	
	GameplayTags.Debuff_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "Debuff.Physical" ),
		FString( "Debuff for Physical Damage" ) );
	
	GameplayTags.Debuff_Stun = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "Debuff.Stun" ),
		FString( "Debuff for Lightning Damage" ) );

	/**
	 * Map of Damage Type Tags to Debuff Type Tags
	 */
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Arcane, GameplayTags.Debuff_Arcane);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Fire, GameplayTags.Debuff_Burn);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Physical, GameplayTags.Debuff_Physical);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Lightning, GameplayTags.Debuff_Stun);

	/**
	 * Debuff Type Tags
	 */
	GameplayTags.Debuff_Properties_Chance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "Debuff.Properties.Chance" ),
		FString( "Chance to Apply the Debuff" ) );
	
	GameplayTags.Debuff_Properties_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "Debuff.Properties.Damage" ),
		FString( "Damage of the Debuff" ) );
	
	GameplayTags.Debuff_Properties_Duration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "Debuff.Properties.Duration" ),
		FString( "Duration of the Debuff" ) );
	
	GameplayTags.Debuff_Properties_Frequency = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "Debuff.Properties.Frequency" ),
		FString( "Frequency of the Application of the Debuff" ) );	

	/**
	 * Ability Tags
	 */
	GameplayTags.Abilities_None = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "Abilities.None" ),
		FString( "NoAbility - like the nullptr for Ability Tags" ) );
	
	GameplayTags.Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "Abilities.Attack" ),
		FString( "Attack Ability Tag" ) );

	GameplayTags.Abilities_Summon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "Abilities.Summon" ),
		FString( "Summon Ability Tag" ) );

	GameplayTags.Abilities_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "Abilities.Fire.FireBolt" ),
		FString( "Fire Bolt Ability Tag" ) );

	GameplayTags.Abilities_Lightning_Electrocute = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "Abilities.Lightning.Electrocute" ),
		FString( "Electrocute Ability Tag" ) );

	GameplayTags.Abilities_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "Abilities.HitReact" ),
		FString( "Hit React Ability Tag" ) );

	/**
	 * Ability Status Tags
	 */
	GameplayTags.Abilities_Status_Eligible = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "Abilities.Status.Eligible" ),
		FString( "Eligible Ability Status Tag" ) );
	
	GameplayTags.Abilities_Status_Equipped = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "Abilities.Status.Equipped" ),
		FString( "Equipped Ability Status Tag" ) );
	
	GameplayTags.Abilities_Status_Locked = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "Abilities.Status.Locked" ),
		FString( "Locked Ability Status Tag" ) );

	GameplayTags.Abilities_Status_Unlocked = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "Abilities.Status.Unlocked" ),
		FString( "Unlocked Ability Status Tag" ) );	

	/**
	 * Ability Type Tags
	 */
	GameplayTags.Abilities_Type_None = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName( "Abilities.Type.None" ),
	FString( "None Ability Type Tag" ) );
	
	GameplayTags.Abilities_Type_Offensive = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName( "Abilities.Type.Offensive" ),
	FString( "Offensive Ability Type Tag" ) );
	
	GameplayTags.Abilities_Type_Passive = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName( "Abilities.Type.Passive" ),
	FString( "Passive Ability Type Tag" ) );

	/**
	 * Cooldown Tags
	 */
	GameplayTags.Cooldown_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "Cooldown.Fire.FireBolt" ),
		FString( "Fire Bolt Cooldown Tag" ) );

	/**
	 * Effect Tags
	 */
	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "Effects.HitReact" ),
		FString( "Tag granted when Hit reacting" ) );

	/**
	 * Combat Socket Tags
	 */
	GameplayTags.CombatSocket_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "CombatSocket.Weapon" ),
		FString( "Weapon" ) );

	GameplayTags.CombatSocket_RightHand = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "CombatSocket.RightHand" ),
		FString( "Right Hand" ) );

	GameplayTags.CombatSocket_LeftHand = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "CombatSocket.LeftHand" ),
		FString( "Left Hand" ) );

	GameplayTags.CombatSocket_Tail = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "CombatSocket.Tail" ),
		FString( "Tail" ) );

	/**
	 * Montage Tags
	*/
	GameplayTags.Montage_Attack_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "Montage.Attack.1" ),
		FString( "Attack 1" ) );

	GameplayTags.Montage_Attack_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "Montage.Attack.2" ),
		FString( "Attack 2" ) );

	GameplayTags.Montage_Attack_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "Montage.Attack.3" ),
		FString( "Attack 3" ) );

	GameplayTags.Montage_Attack_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName( "Montage.Attack.4" ),
		FString( "Attack 4" ) );
}
