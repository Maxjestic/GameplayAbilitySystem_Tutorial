// Copyright Maxjestic


#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Player/AuraPlayerController.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	// Primary
	TagsToAttributes.Add( GameplayTags.Attributes_Primary_Strength, GetStrengthAttribute );
	TagsToAttributes.Add( GameplayTags.Attributes_Primary_Intelligence, GetIntelligenceAttribute );
	TagsToAttributes.Add( GameplayTags.Attributes_Primary_Resilience, GetResilienceAttribute );
	TagsToAttributes.Add( GameplayTags.Attributes_Primary_Vigor, GetVigorAttribute );

	// Secondary
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
}

void UAuraAttributeSet::GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );

	// Primary
	DOREPLIFETIME_CONDITION_NOTIFY( UAuraAttributeSet, Strength, COND_None, REPNOTIFY_Always );
	DOREPLIFETIME_CONDITION_NOTIFY( UAuraAttributeSet, Intelligence, COND_None, REPNOTIFY_Always );
	DOREPLIFETIME_CONDITION_NOTIFY( UAuraAttributeSet, Resilience, COND_None, REPNOTIFY_Always );
	DOREPLIFETIME_CONDITION_NOTIFY( UAuraAttributeSet, Vigor, COND_None, REPNOTIFY_Always );

	// Secondary
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

	// Vital
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
		UE_LOG( LogTemp,
		        Warning,
		        TEXT("Changed Health on %s, Health: %f"),
		        *Properties.TargetAvatarActor->GetName(),
		        GetHealth() )
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
			}
			else
			{
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag( FAuraGameplayTags::Get().Effects_HitReact );
				Properties.TargetAbilitySystemComponent->TryActivateAbilitiesByTag( TagContainer );
			}
			ShowFloatingText( Properties, LocalIncomingDamage );
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

void UAuraAttributeSet::ShowFloatingText( const FEffectProperties& Properties, const float Damage ) const
{
	if (Properties.SourceCharacter != Properties.TargetCharacter)
	{
		if (AAuraPlayerController* PlayerController = Cast<AAuraPlayerController>(
			UGameplayStatics::GetPlayerController( Properties.SourceCharacter, 0 ) ))
		{
			PlayerController->ShowDamageNumber( Damage, Properties.TargetCharacter );
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
