// Copyright Maxjestic


#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/PlayerInterface.h"
#include "Net/UnrealNetwork.h"
#include "Player/AuraPlayerController.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	/** Primary */
	TagsToAttributes.Add( GameplayTags.Attributes_Primary_Strength, GetStrengthAttribute );
	TagsToAttributes.Add( GameplayTags.Attributes_Primary_Intelligence, GetIntelligenceAttribute );
	TagsToAttributes.Add( GameplayTags.Attributes_Primary_Resilience, GetResilienceAttribute );
	TagsToAttributes.Add( GameplayTags.Attributes_Primary_Vigor, GetVigorAttribute );

	/** Secondary */
	TagsToAttributes.Add( GameplayTags.Attributes_Secondary_Armor, GetArmorAttribute );
	TagsToAttributes.Add( GameplayTags.Attributes_Secondary_ArmorPenetration, GetArmorPenetrationAttribute );
	TagsToAttributes.Add( GameplayTags.Attributes_Secondary_BlockChance, GetBlockChanceAttribute );
	TagsToAttributes.Add( GameplayTags.Attributes_Secondary_CriticalHitChance, GetCriticalHitChanceAttribute );
	TagsToAttributes.Add( GameplayTags.Attributes_Secondary_CriticalHitDamage, GetCriticalHitDamageAttribute );
	TagsToAttributes.Add( GameplayTags.Attributes_Secondary_CriticalHitResistance, GetCriticalHitResistanceAttribute );
	TagsToAttributes.Add( GameplayTags.Attributes_Secondary_HealthRegeneration, GetHealthRegenerationAttribute );
	TagsToAttributes.Add( GameplayTags.Attributes_Secondary_ManaRegeneration, GetManaRegenerationAttribute );
	TagsToAttributes.Add( GameplayTags.Attributes_Secondary_MaxHealth, GetMaxHealthAttribute );
	TagsToAttributes.Add( GameplayTags.Attributes_Secondary_MaxMana, GetMaxManaAttribute );

	/** Resistance */
	TagsToAttributes.Add( GameplayTags.Attributes_Resistance_Arcane, GetArcaneResistanceAttribute );
	TagsToAttributes.Add( GameplayTags.Attributes_Resistance_Fire, GetFireResistanceAttribute );
	TagsToAttributes.Add( GameplayTags.Attributes_Resistance_Lightning, GetLightningResistanceAttribute );
	TagsToAttributes.Add( GameplayTags.Attributes_Resistance_Physical, GetPhysicalResistanceAttribute );
}

void UAuraAttributeSet::GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );

	/** Primary */
	DOREPLIFETIME_CONDITION_NOTIFY( UAuraAttributeSet, Strength, COND_None, REPNOTIFY_Always );
	DOREPLIFETIME_CONDITION_NOTIFY( UAuraAttributeSet, Intelligence, COND_None, REPNOTIFY_Always );
	DOREPLIFETIME_CONDITION_NOTIFY( UAuraAttributeSet, Resilience, COND_None, REPNOTIFY_Always );
	DOREPLIFETIME_CONDITION_NOTIFY( UAuraAttributeSet, Vigor, COND_None, REPNOTIFY_Always );

	/** Secondary */
	DOREPLIFETIME_CONDITION_NOTIFY( UAuraAttributeSet, Armor, COND_None, REPNOTIFY_Always );
	DOREPLIFETIME_CONDITION_NOTIFY( UAuraAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always );
	DOREPLIFETIME_CONDITION_NOTIFY( UAuraAttributeSet, BlockChance, COND_None, REPNOTIFY_Always );
	DOREPLIFETIME_CONDITION_NOTIFY( UAuraAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always );
	DOREPLIFETIME_CONDITION_NOTIFY( UAuraAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always );
	DOREPLIFETIME_CONDITION_NOTIFY( UAuraAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always );
	DOREPLIFETIME_CONDITION_NOTIFY( UAuraAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always );
	DOREPLIFETIME_CONDITION_NOTIFY( UAuraAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always );
	DOREPLIFETIME_CONDITION_NOTIFY( UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always );
	DOREPLIFETIME_CONDITION_NOTIFY( UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always );

	/** Resistance */
	DOREPLIFETIME_CONDITION_NOTIFY( UAuraAttributeSet, ArcaneResistance, COND_None, REPNOTIFY_Always );
	DOREPLIFETIME_CONDITION_NOTIFY( UAuraAttributeSet, FireResistance, COND_None, REPNOTIFY_Always );
	DOREPLIFETIME_CONDITION_NOTIFY( UAuraAttributeSet, LightningResistance, COND_None, REPNOTIFY_Always );
	DOREPLIFETIME_CONDITION_NOTIFY( UAuraAttributeSet, PhysicalResistance, COND_None, REPNOTIFY_Always );

	/** Vital */
	DOREPLIFETIME_CONDITION_NOTIFY( UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always );
	DOREPLIFETIME_CONDITION_NOTIFY( UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always );
}

void UAuraAttributeSet::PreAttributeChange( const FGameplayAttribute& Attribute, float& NewValue )
{
	Super::PreAttributeChange( Attribute, NewValue );

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp( NewValue, 0.f, GetMaxHealth() );
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp( NewValue, 0.f, GetMaxMana() );
	}
}

void UAuraAttributeSet::PostGameplayEffectExecute( const FGameplayEffectModCallbackData& Data )
{
	Super::PostGameplayEffectExecute( Data );

	FEffectProperties Properties;

	SetEffectProperties( Data, Properties );

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth( FMath::Clamp( GetHealth(), 0.f, GetMaxHealth() ) );
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana( FMath::Clamp( GetMana(), 0.f, GetMaxMana() ) );
	}
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float LocalIncomingDamage = GetIncomingDamage();
		SetIncomingDamage( 0.f );
		if (LocalIncomingDamage > 0.f)
		{
			const float NewHealth = GetHealth() - LocalIncomingDamage;
			SetHealth( FMath::Clamp( NewHealth, 0, GetMaxHealth() ) );

			const bool bFatal = NewHealth <= 0.f;
			if (bFatal)
			{
				if (ICombatInterface* CombatInterface = Cast<ICombatInterface>( Properties.TargetAvatarActor ))
				{
					CombatInterface->Die();
				}
				SendExperienceEvent( Properties );
			}
			else
			{
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag( FAuraGameplayTags::Get().Effects_HitReact );
				Properties.TargetAbilitySystemComponent->TryActivateAbilitiesByTag( TagContainer );
			}
			const bool bBlockedHit = UAuraAbilitySystemLibrary::IsBlockedHit( Properties.EffectContextHandle );
			const bool bCriticalHit = UAuraAbilitySystemLibrary::IsCriticalHit( Properties.EffectContextHandle );
			ShowFloatingText( Properties, LocalIncomingDamage, bBlockedHit, bCriticalHit );
		}
	}
	if (Data.EvaluatedData.Attribute == GetIncomingExperienceAttribute())
	{
		const float LocalIncomingExperience = GetIncomingExperience();
		SetIncomingExperience( 0.f );
		if (LocalIncomingExperience > 0)
		{
			//TODO: Check if character should level up
			if (Properties.SourceCharacter->Implements<UPlayerInterface>())
			{
				IPlayerInterface::Execute_AddExperience( Properties.SourceCharacter, LocalIncomingExperience );
			}
		}
	}
}

void UAuraAttributeSet::SetEffectProperties( const FGameplayEffectModCallbackData& Data,
                                             FEffectProperties& Properties ) const
{
	// Source = causer of the effect, Target = target of the effect (owner of this AS)

	Properties.EffectContextHandle = Data.EffectSpec.GetContext();
	Properties.SourceAbilitySystemComponent = Properties.EffectContextHandle.
	                                                     GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid( Properties.SourceAbilitySystemComponent ) &&
		Properties.SourceAbilitySystemComponent->AbilityActorInfo.IsValid() &&
		Properties.SourceAbilitySystemComponent->AbilityActorInfo->AvatarActor.IsValid())
	{
		Properties.SourceAvatarActor = Properties.SourceAbilitySystemComponent->AbilityActorInfo->AvatarActor.Get();
		Properties.SourceController = Properties.SourceAbilitySystemComponent->AbilityActorInfo->PlayerController.Get();

		// If AbilityActorInfo has a nullptr for the player controller, we try to get a controller from Pawn
		if (Properties.SourceController == nullptr && Properties.SourceAvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>( Properties.SourceAvatarActor ))
			{
				Properties.SourceController = Pawn->GetController();
			}
		}
		if (Properties.SourceController != nullptr)
		{
			Properties.SourceCharacter = Properties.SourceController->GetPawn<ACharacter>();
			//Cast<ACharacter>(Properties.SourceController->GetPawn());
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Properties.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Properties.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Properties.TargetCharacter = Cast<ACharacter>( Properties.TargetAvatarActor );
		Properties.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(
			Properties.TargetAvatarActor );
	}
}

void UAuraAttributeSet::ShowFloatingText( const FEffectProperties& Properties, const float Damage, const bool bBlockedHit,
                                          const bool bCriticalHit ) const
{
	if (Properties.SourceCharacter != Properties.TargetCharacter)
	{
		if (AAuraPlayerController* PlayerController = Cast<AAuraPlayerController>( Properties.SourceCharacter->Controller ))
		{
			PlayerController->ShowDamageNumber( Damage, Properties.TargetCharacter, bBlockedHit, bCriticalHit );
			return;
		}
		if (AAuraPlayerController* PlayerController = Cast<AAuraPlayerController>( Properties.TargetCharacter->Controller ))
		{
			PlayerController->ShowDamageNumber( Damage, Properties.TargetCharacter, bBlockedHit, bCriticalHit );
		}
	}
}

void UAuraAttributeSet::SendExperienceEvent( const FEffectProperties& Properties )
{
	if (const ICombatInterface* CombatInterface = Cast<ICombatInterface>( Properties.TargetCharacter ))
	{
		const ECharacterClass CharacterClass = ICombatInterface::Execute_GetCharacterClass( Properties.TargetCharacter );
		const int32 Level = CombatInterface->GetCharacterLevel();

		const int32 ExperienceReward = UAuraAbilitySystemLibrary::GetExperienceRewardForClassAndLevel(
			Properties.TargetCharacter,
			CharacterClass,
			Level );

		const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
		FGameplayEventData Payload;
		Payload.EventTag = GameplayTags.Attributes_Meta_IncomingExperience;
		Payload.EventMagnitude = ExperienceReward;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor( Properties.SourceCharacter,
		                                                          GameplayTags.Attributes_Meta_IncomingExperience,
		                                                          Payload );
	}
}

void UAuraAttributeSet::OnRep_Strength( const FGameplayAttributeData& OldStrength ) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY( UAuraAttributeSet, Strength, OldStrength );
}

void UAuraAttributeSet::OnRep_Intelligence( const FGameplayAttributeData& OldIntelligence ) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY( UAuraAttributeSet, Intelligence, OldIntelligence );
}

void UAuraAttributeSet::OnRep_Resilience( const FGameplayAttributeData& OldResilience ) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY( UAuraAttributeSet, Resilience, OldResilience );
}

void UAuraAttributeSet::OnRep_Vigor( const FGameplayAttributeData& OldVigor ) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY( UAuraAttributeSet, Vigor, OldVigor );
}

void UAuraAttributeSet::OnRep_Armor( const FGameplayAttributeData& OldArmor ) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY( UAuraAttributeSet, Armor, OldArmor );
}

void UAuraAttributeSet::OnRep_ArmorPenetration( const FGameplayAttributeData& OldArmorPenetration ) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY( UAuraAttributeSet, ArmorPenetration, OldArmorPenetration );
}

void UAuraAttributeSet::OnRep_BlockChance( const FGameplayAttributeData& OldBlockChance ) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY( UAuraAttributeSet, BlockChance, OldBlockChance );
}

void UAuraAttributeSet::OnRep_CriticalHitChance( const FGameplayAttributeData& OldCriticalHitChance ) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY( UAuraAttributeSet, CriticalHitChance, OldCriticalHitChance );
}

void UAuraAttributeSet::OnRep_CriticalHitDamage( const FGameplayAttributeData& OldCriticalHitDamage ) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY( UAuraAttributeSet, CriticalHitDamage, OldCriticalHitDamage );
}

void UAuraAttributeSet::OnRep_CriticalHitResistance( const FGameplayAttributeData& OldCriticalHitResistance ) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY( UAuraAttributeSet, CriticalHitResistance, OldCriticalHitResistance );
}

void UAuraAttributeSet::OnRep_HealthRegeneration( const FGameplayAttributeData& OldHealthRegeneration ) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY( UAuraAttributeSet, HealthRegeneration, OldHealthRegeneration );
}

void UAuraAttributeSet::OnRep_ManaRegeneration( const FGameplayAttributeData& OldManaRegeneration ) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY( UAuraAttributeSet, ManaRegeneration, OldManaRegeneration );
}

void UAuraAttributeSet::OnRep_MaxHealth( const FGameplayAttributeData& OldMaxHealth ) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY( UAuraAttributeSet, MaxHealth, OldMaxHealth );
}

void UAuraAttributeSet::OnRep_MaxMana( const FGameplayAttributeData& OldMaxMana ) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY( UAuraAttributeSet, MaxMana, OldMaxMana );
}

void UAuraAttributeSet::OnRep_Health( const FGameplayAttributeData& OldHealth ) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY( UAuraAttributeSet, Health, OldHealth );
}

void UAuraAttributeSet::OnRep_Mana( const FGameplayAttributeData& OldMana ) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY( UAuraAttributeSet, Mana, OldMana );
}

void UAuraAttributeSet::OnRep_ArcaneResistance( const FGameplayAttributeData& OldArcaneResistance ) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY( UAuraAttributeSet, ArcaneResistance, OldArcaneResistance );
}

void UAuraAttributeSet::OnRep_FireResistance( const FGameplayAttributeData& OldFireResistance ) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY( UAuraAttributeSet, FireResistance, OldFireResistance );
}

void UAuraAttributeSet::OnRep_LightningResistance( const FGameplayAttributeData& OldLightningResistance ) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY( UAuraAttributeSet, LightningResistance, OldLightningResistance );
}

void UAuraAttributeSet::OnRep_PhysicalResistance( const FGameplayAttributeData& OldPhysicalResistance ) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY( UAuraAttributeSet, PhysicalResistance, OldPhysicalResistance );
}
