// Copyright Maxjestic


#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
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

	if (Properties.TargetCharacter->Implements<UCombatInterface>() && ICombatInterface::Execute_IsDead( Properties.TargetCharacter ))
	{
		return;
	}

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
		HandleIncomingDamage( Properties );
	}
	if (Data.EvaluatedData.Attribute == GetIncomingExperienceAttribute())
	{
		HandleIncomingExperience( Properties );
	}
}

void UAuraAttributeSet::PostAttributeChange( const FGameplayAttribute& Attribute, const float OldValue, const float NewValue )
{
	Super::PostAttributeChange( Attribute, OldValue, NewValue );

	if (Attribute == GetMaxHealthAttribute() && bTopOffHealth)
	{
		SetHealth( GetMaxHealth() );
		bTopOffHealth = false;
	}

	if (Attribute == GetMaxManaAttribute() && bTopOffMana)
	{
		SetMana( GetMaxMana() );
		bTopOffMana = false;
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
	if (Properties.TargetCharacter->Implements<UCombatInterface>())
	{
		const ECharacterClass CharacterClass = ICombatInterface::Execute_GetCharacterClass( Properties.TargetCharacter );
		const int32 Level = ICombatInterface::Execute_GetCharacterLevel( Properties.TargetCharacter );

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

void UAuraAttributeSet::HandleIncomingDamage( const FEffectProperties& Properties )
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
				CombatInterface->Die( UAuraAbilitySystemLibrary::GetDeathImpulse( Properties.EffectContextHandle ) );
			}
			SendExperienceEvent( Properties );
		}
		else
		{
			if (Properties.TargetCharacter->Implements<UCombatInterface>()
				&& !ICombatInterface::Execute_IsBeingShocked( Properties.TargetCharacter ))
			{
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag( FAuraGameplayTags::Get().Effects_HitReact );
				Properties.TargetAbilitySystemComponent->TryActivateAbilitiesByTag( TagContainer );
			}			

			const FVector& KnockbackForce = UAuraAbilitySystemLibrary::GetKnockbackForce( Properties.EffectContextHandle );
			if (!KnockbackForce.IsNearlyZero( 1.f ))
			{
				Properties.TargetCharacter->LaunchCharacter( KnockbackForce, true, true );
			}
		}
		const bool bBlockedHit = UAuraAbilitySystemLibrary::IsBlockedHit( Properties.EffectContextHandle );
		const bool bCriticalHit = UAuraAbilitySystemLibrary::IsCriticalHit( Properties.EffectContextHandle );
		ShowFloatingText( Properties, LocalIncomingDamage, bBlockedHit, bCriticalHit );
		if (UAuraAbilitySystemLibrary::IsSuccessfulDebuff( Properties.EffectContextHandle ))
		{
			HandleDebuff( Properties );
		}
	}
}

void UAuraAttributeSet::HandleDebuff( const FEffectProperties& Properties )
{
	const FAuraGameplayTags Tags = FAuraGameplayTags::Get();
	FGameplayEffectContextHandle EffectContextHandle = Properties.SourceAbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject( Properties.SourceAvatarActor );

	const FGameplayTag DamageType = UAuraAbilitySystemLibrary::GetDamageType( Properties.EffectContextHandle );
	const float DebuffDamage = UAuraAbilitySystemLibrary::GetDebuffDamage( Properties.EffectContextHandle );
	const float DebuffDuration = UAuraAbilitySystemLibrary::GetDebuffDuration( Properties.EffectContextHandle );
	const float DebuffFrequency = UAuraAbilitySystemLibrary::GetDebuffFrequency( Properties.EffectContextHandle );

	const FString DebuffName = FString::Printf( TEXT( "DynamicDebuff_%s" ), *DamageType.ToString() );

	UGameplayEffect* Effect = NewObject<UGameplayEffect>( GetTransientPackage(), FName( DebuffName ) );
	Effect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
	Effect->Period = DebuffFrequency;
	Effect->DurationMagnitude = FScalableFloat( DebuffDuration );

	FInheritedTagContainer InheritedTagContainer = FInheritedTagContainer();
	UTargetTagsGameplayEffectComponent& TargetTagsGameplayEffectComponent = Effect->
		FindOrAddComponent<UTargetTagsGameplayEffectComponent>();

	const FGameplayTag DebuffTag = Tags.DamageTypesToDebuffs[DamageType];
	InheritedTagContainer.Added.AddTag( DebuffTag );

	if (DebuffTag.MatchesTagExact( Tags.Debuff_Stun ))
	{
		InheritedTagContainer.Added.AddTag( Tags.Player_Block_InputPressed );
		InheritedTagContainer.Added.AddTag( Tags.Player_Block_InputHeld );
		InheritedTagContainer.Added.AddTag( Tags.Player_Block_InputReleased );
		InheritedTagContainer.Added.AddTag( Tags.Player_Block_CursorTrace );
	}

	TargetTagsGameplayEffectComponent.SetAndApplyTargetTagChanges( InheritedTagContainer );

	Effect->StackingType = EGameplayEffectStackingType::AggregateBySource;
	Effect->StackLimitCount = 1;

	const int32 Index = Effect->Modifiers.Num();
	Effect->Modifiers.Add( FGameplayModifierInfo() );
	FGameplayModifierInfo& ModifierInfo = Effect->Modifiers[Index];

	ModifierInfo.ModifierMagnitude = FScalableFloat( DebuffDamage );
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	ModifierInfo.Attribute = GetIncomingDamageAttribute();

	if (const FGameplayEffectSpec* MutableSpec = new FGameplayEffectSpec( Effect, EffectContextHandle, 1.f ))
	{
		FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<FAuraGameplayEffectContext*>(MutableSpec->GetContext().Get());
		const TSharedPtr<FGameplayTag> DebuffDamageType = MakeShareable( new FGameplayTag( DamageType ) );
		AuraGameplayEffectContext->SetDamageType( DebuffDamageType );
		Properties.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf( *MutableSpec );
	}
}

void UAuraAttributeSet::HandleIncomingExperience( const FEffectProperties& Properties )
{
	const float LocalIncomingExperience = GetIncomingExperience();
	SetIncomingExperience( 0.f );
	if (LocalIncomingExperience > 0)
	{
		// Source Character is the owner, since GA_ListenForEvent applies GE_EventBasedEffect, adding to IncomingExperience
		if (Properties.SourceCharacter->Implements<UPlayerInterface>() && Properties.SourceCharacter->Implements<UCombatInterface>())
		{
			const int32 CurrentLevel = ICombatInterface::Execute_GetCharacterLevel( Properties.SourceCharacter );
			const int32 CurrentExperience = IPlayerInterface::Execute_GetExperience( Properties.SourceCharacter );

			const int32 NewLevel = IPlayerInterface::Execute_FindLevelForExperience(
				Properties.SourceCharacter,
				CurrentExperience + LocalIncomingExperience );

			const int32 LevelUpsNum = NewLevel - CurrentLevel;

			if (LevelUpsNum > 0)
			{
				IPlayerInterface::Execute_AddToPlayerLevel( Properties.SourceCharacter, LevelUpsNum );

				
				int32 AttributePointsReward = 0;

				int32 SpellPointsReward = 0;

				for (int32 i = 0; i< LevelUpsNum; i++)
				{
					AttributePointsReward += IPlayerInterface::Execute_GetAttributePointsReward( Properties.SourceCharacter, CurrentLevel + i );
					SpellPointsReward += IPlayerInterface::Execute_GetSpellPointsReward( Properties.SourceCharacter, CurrentLevel + i );
				}
				
				IPlayerInterface::Execute_AddToAttributePoints( Properties.SourceCharacter, AttributePointsReward );
				IPlayerInterface::Execute_AddToSpellPoints( Properties.SourceCharacter, SpellPointsReward );

				bTopOffHealth = true;
				bTopOffMana = true;

				IPlayerInterface::Execute_LevelUp( Properties.SourceCharacter );
			}

			IPlayerInterface::Execute_AddExperience( Properties.SourceCharacter, LocalIncomingExperience );
		}
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
