// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AuraAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
		GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/** Holds information about entities involved in given gameplay effect
 *  Data is filled in PostGameplayEffectExecute */
USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties(){}	

	/** Handle to executed gameplay effect context */
	FGameplayEffectContextHandle EffectContextHandle;

	/** Ability system component of the source of the executed gameplay effect */
	UPROPERTY()
	UAbilitySystemComponent* SourceAbilitySystemComponent = nullptr;

	/** Avatar actor of the source of the executed gameplay effect */
	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;

	/** Controller of the source of the executed gameplay effect, can be null */
	UPROPERTY()
	AController* SourceController = nullptr;

	/** Character of the source of the executed gameplay effect, can be null */
	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	/** Ability system component of the target of the executed gameplay effect */
	UPROPERTY()
	UAbilitySystemComponent* TargetAbilitySystemComponent = nullptr;
	
	/** Avatar actor of the target of the executed gameplay effect */
	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;

	/** Controller of the target of the executed gameplay effect, can be null */
	UPROPERTY()
	AController* TargetController = nullptr;

	/** Character of the target of the executed gameplay effect, can be null */
	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
};

/** An alias for Function Pointer to function with any signature - T(*)( ... ) */
template<class T>
using TStaticFuncPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;

/**
 * Attribute set class containing all attributes for both player- and AI-controlled characters.
 * There are types of Attributes: Primary, Secondary, Vital. All have their own Gameplay Tags.
 * All Attributes are replicated
 */
UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	/**
	 * Sets gameplay tags to attributes
	 */
	UAuraAttributeSet();

	//~ Begin UObject Interface
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override;
	//~ End UObject Interface

	//~ Begin UAttributeSet Interface
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	//~ End UAttributeSet Interface
	
	/** Map assigning gameplay tags to attributes  */
	TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> TagsToAttributes;
	
	/**
	 * Attribute properties, with Attribute Accessors
	 */

	/**
	 * Primary Attributes
	 */

	/** Increases Physical Damage */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "Primary Attributes")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Strength);

	/** Increases Magical Damage */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence, Category = "Primary Attributes")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Intelligence);

	/** Increases Armor and Armor Penetration */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Resilience, Category = "Primary Attributes")
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Resilience);

	/** Increases Health */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vigor, Category = "Primary Attributes")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Vigor);

	/**
	 * Secondary Attributes
	 */
	
	/** Reduces Damage taken, increases Block Chance */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category = "Secondary Attributes")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Armor);

	/** Ignores percentage of enemy Armor, increases Critical Hit Chance */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorPenetration, Category = "Secondary Attributes")
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ArmorPenetration);

	/** Chance to cut incoming Damage in half */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BlockChance, Category = "Secondary Attributes")
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, BlockChance);

	/** Chance to double Damage plus Critical Hit bonus */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitChance, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitChance);

	/** Bonus Damage added when Critical Hit is scored */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitDamage, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitDamage);

	/** Reduces Critical Hit Chance of attacking enemies */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitResistance, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitResistance);

	/** Amount of Health regenerated every second */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegeneration, Category = "Secondary Attributes")
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, HealthRegeneration);

	/** Amount of Mana regenerated every second */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegeneration, Category = "Secondary Attributes")
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ManaRegeneration);

	/** Maximum amount of obtainable Health */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Vital Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth);

	/** Maximum amount of obtainable Mana */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Vital Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana);

	/**
	 * Vital Attributes
	 */
	
	/** Maximum amount of Health obtainable */	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health);

	/** Maximum amount of Mana obtainable */	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana);

	/**
	 * Meta Attributes
	 */

	/** Product of calculation that will deal damage, set and calculated on the server */
	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, IncomingDamage);
	
	/*
	 * Replication Functions, called everytime when any attribute changes
	 */

	/**
	 * Primary Attributes
	 */
	UFUNCTION()
	void OnRep_Strength( const FGameplayAttributeData& OldStrength ) const;

	UFUNCTION()
	void OnRep_Intelligence( const FGameplayAttributeData& OldIntelligence ) const;

	UFUNCTION()
	void OnRep_Resilience( const FGameplayAttributeData& OldResilience ) const;

	UFUNCTION()
	void OnRep_Vigor( const FGameplayAttributeData& OldVigor ) const;
	
	/**
	 * Secondary Attributes
	 */
	UFUNCTION()
	void OnRep_Armor( const FGameplayAttributeData& OldArmor ) const;

	UFUNCTION()
	void OnRep_ArmorPenetration( const FGameplayAttributeData& OldArmorPenetration ) const;

	UFUNCTION()
	void OnRep_BlockChance( const FGameplayAttributeData& OldBlockChance ) const;

	UFUNCTION()
	void OnRep_CriticalHitChance( const FGameplayAttributeData& OldCriticalHitChance ) const;

	UFUNCTION()
	void OnRep_CriticalHitDamage( const FGameplayAttributeData& OldCriticalHitDamage ) const;

	UFUNCTION()
	void OnRep_CriticalHitResistance( const FGameplayAttributeData& OldCriticalHitResistance ) const;

	UFUNCTION()
	void OnRep_HealthRegeneration( const FGameplayAttributeData& OldHealthRegeneration ) const;

	UFUNCTION()
	void OnRep_ManaRegeneration( const FGameplayAttributeData& OldManaRegeneration ) const;

	UFUNCTION()
	void OnRep_MaxHealth( const FGameplayAttributeData& OldMaxHealth ) const;

	UFUNCTION()
	void OnRep_MaxMana( const FGameplayAttributeData& OldMaxMana ) const;

	/**
	 * Vital Attributes
	 */
	UFUNCTION()
	void OnRep_Health( const FGameplayAttributeData& OldHealth ) const;

	UFUNCTION()
	void OnRep_Mana( const FGameplayAttributeData& OldMana ) const;
	
private:
	/**
	 * Fills up EffectProperties during PostGameplayEffectExecute
	 * with data from source and target involved in given gameplay effect
	 */
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Properties) const;
};
