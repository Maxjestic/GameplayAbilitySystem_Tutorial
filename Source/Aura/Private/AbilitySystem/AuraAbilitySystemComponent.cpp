// Copyright Maxjestic


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/Ability/AuraGameplayAbility.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Aura/AuraLogChannels.h"
#include "Interaction/PlayerInterface.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject( this, &UAuraAbilitySystemComponent::ClientEffectApplied );
}

void UAuraAbilitySystemComponent::AddCharacterAbilities( const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities )
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec( AbilityClass, 1 );
		if (const UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>( AbilitySpec.Ability ))
		{
			AbilitySpec.DynamicAbilityTags.AddTag( AuraAbility->StartupInputTag );
			AbilitySpec.DynamicAbilityTags.AddTag( FAuraGameplayTags::Get().Abilities_Status_Equipped );
			GiveAbility( AbilitySpec );
		}
	}
	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast();
}

void UAuraAbilitySystemComponent::AddCharacterPassiveAbilities( const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities )
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupPassiveAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec( AbilityClass, 1 );
		GiveAbilityAndActivateOnce( AbilitySpec );
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagPressed( const FGameplayTag& InputTag )
{
	if (!InputTag.IsValid())
	{
		return;
	}

	FScopedAbilityListLock ActiveScopeLoc( *this );
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact( InputTag ))
		{
			AbilitySpecInputPressed( AbilitySpec );
			if (AbilitySpec.IsActive())
			{
				InvokeReplicatedEvent( EAbilityGenericReplicatedEvent::InputPressed,
				                       AbilitySpec.Handle,
				                       AbilitySpec.ActivationInfo.GetActivationPredictionKey() );
			}
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased( const FGameplayTag& InputTag )
{
	if (!InputTag.IsValid())
	{
		return;
	}

	FScopedAbilityListLock ActiveScopeLoc( *this );
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact( InputTag ) && AbilitySpec.IsActive())
		{
			AbilitySpecInputReleased( AbilitySpec );
			InvokeReplicatedEvent( EAbilityGenericReplicatedEvent::InputReleased,
			                       AbilitySpec.Handle,
			                       AbilitySpec.ActivationInfo.GetActivationPredictionKey() );
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld( const FGameplayTag& InputTag )
{
	if (!InputTag.IsValid())
	{
		return;
	}

	FScopedAbilityListLock ActiveScopeLoc( *this );
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact( InputTag ))
		{
			AbilitySpecInputPressed( AbilitySpec );
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility( AbilitySpec.Handle );
			}
		}
	}
}

void UAuraAbilitySystemComponent::ForEachAbility( const FForEachAbility& Delegate )
{
	FScopedAbilityListLock ActiveScopeLock( *this );
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!Delegate.ExecuteIfBound( AbilitySpec ))
		{
			UE_LOG( LogAura, Error, TEXT("Failed to execute delegate in %hs"), __FUNCTION__ )
		}
	}
}

FGameplayTag UAuraAbilitySystemComponent::GetAbilityTagFromSpec( const FGameplayAbilitySpec& AbilitySpec )
{
	if (!AbilitySpec.Ability)
	{
		return FGameplayTag();
	}

	for (const FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
	{
		if (Tag.MatchesTag( FGameplayTag::RequestGameplayTag( FName( "Abilities" ) ) ))
		{
			return Tag;
		}
	}

	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetInputTagFromSpec( const FGameplayAbilitySpec& AbilitySpec )
{
	for (const FGameplayTag Tag : AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag( FGameplayTag::RequestGameplayTag( FName( "InputTag" ) ) ))
		{
			return Tag;
		}
	}

	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetStatusTagFromSpec( const FGameplayAbilitySpec& AbilitySpec )
{
	for (const FGameplayTag Tag : AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag( FGameplayTag::RequestGameplayTag( FName( "Abilities.Status" ) ) ))
		{
			return Tag;
		}
	}

	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetStatusTagFromAbilityTag( const FGameplayTag& AbilityTag )
{
	if (const FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag( AbilityTag ))
	{
		return GetStatusTagFromSpec( *AbilitySpec );
	}
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetSlotFromAbilityTag( const FGameplayTag& AbilityTag )
{
	if (const FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag( AbilityTag ))
	{
		return GetInputTagFromSpec( *AbilitySpec );
	}
	return FGameplayTag();
}

FGameplayAbilitySpec* UAuraAbilitySystemComponent::GetSpecFromAbilityTag( const FGameplayTag& AbilityTag )
{
	FScopedAbilityListLock ActiveScopeLock( *this );
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		for (const FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag( AbilityTag ))
			{
				return &AbilitySpec;
			}
		}
	}

	return nullptr;
}

bool UAuraAbilitySystemComponent::SlotIsEmpty( const FGameplayTag& Slot )
{
	FScopedAbilityListLock ActiveScopeLoc( *this );
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilityHasSlot( AbilitySpec, Slot ))
		{
			return false;
		}
	}
	return true;
}

bool UAuraAbilitySystemComponent::AbilityHasSlot( const FGameplayAbilitySpec& Spec, const FGameplayTag& Slot )
{
	return Spec.DynamicAbilityTags.HasTagExact( Slot );
}

bool UAuraAbilitySystemComponent::AbilityHasAnySlot( const FGameplayAbilitySpec& Spec )
{
	return Spec.DynamicAbilityTags.HasTag( FGameplayTag::RequestGameplayTag( FName( "InputTag" ) ) );
}

FGameplayAbilitySpec* UAuraAbilitySystemComponent::GetSpecWithSlot( const FGameplayTag& Slot )
{
	FScopedAbilityListLock ActiveScopeLoc( *this );
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilityHasSlot( AbilitySpec, Slot ))
		{
			return &AbilitySpec;
		}
	}
	return nullptr;
}

bool UAuraAbilitySystemComponent::IsPassiveAbility( const FGameplayAbilitySpec& Spec ) const
{
	const UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo( GetAvatarActor() );
	const FGameplayTag AbilityTag = GetAbilityTagFromSpec( Spec );
	const FAuraAbilityInfo& Info = AbilityInfo->FindAbilityInfoForTag( AbilityTag );
	const FGameplayTag AbilityType = Info.AbilityTypeTag;

	return AbilityType.MatchesTagExact( FAuraGameplayTags::Get().Abilities_Type_Passive );
}

void UAuraAbilitySystemComponent::AssignSlotToAbility( FGameplayAbilitySpec& Spec, const FGameplayTag& Slot )
{
	ClearSlot( &Spec );
	Spec.DynamicAbilityTags.AddTag( Slot );
}

void UAuraAbilitySystemComponent::MulticastActivatePassiveEffect_Implementation( const FGameplayTag& AbilityTag, const bool bActivate )
{
	ActivatePassiveEffect.Broadcast( AbilityTag, bActivate );
}

void UAuraAbilitySystemComponent::UpgradeAttribute( const FGameplayTag& AttributeTag )
{
	if (GetAvatarActor()->Implements<UPlayerInterface>() && IPlayerInterface::Execute_GetAttributePoints( GetAvatarActor() ) > 0)
	{
		ServerUpgradeAttribute( AttributeTag );
	}
}

void UAuraAbilitySystemComponent::UpdateAbilityStatuses( const int32 Level )
{
	UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo( GetAvatarActor() );

	for (const FAuraAbilityInfo& Info : AbilityInfo->AbilityInformation)
	{
		if (!Info.AbilityTag.IsValid() || Level < Info.LevelRequirement)
		{
			continue;
		}
		if (GetSpecFromAbilityTag( Info.AbilityTag ) == nullptr)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec( Info.AbilityClass, 1 );
			AbilitySpec.DynamicAbilityTags.AddTag( FAuraGameplayTags::Get().Abilities_Status_Eligible );
			GiveAbility( AbilitySpec );
			MarkAbilitySpecDirty( AbilitySpec );
			ClientUpdateAbilityStatus( Info.AbilityTag, FAuraGameplayTags::Get().Abilities_Status_Eligible, AbilitySpec.Level );
		}
	}
}

bool UAuraAbilitySystemComponent::GetDescriptionsByAbilityTag( const FGameplayTag& AbilityTag, FString& OutDescription,
                                                               FString& OutNextLevelDescription )
{
	if (const FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag( AbilityTag ))
	{
		if (UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>( AbilitySpec->Ability ))
		{
			OutDescription = AuraAbility->GetDescription( AbilitySpec->Level );
			OutNextLevelDescription = AuraAbility->GetDescription( AbilitySpec->Level, true );
			return true;
		}
	}
	const UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo( GetAvatarActor() );
	if (!AbilityTag.IsValid() || AbilityTag.MatchesTagExact( FAuraGameplayTags::Get().Abilities_None ))
	{
		OutDescription = FString();
	}
	else
	{
		OutDescription = UAuraGameplayAbility::GetLockedDescription( AbilityInfo->FindAbilityInfoForTag( AbilityTag ).LevelRequirement );
	}
	OutNextLevelDescription = FString();
	return false;
}

void UAuraAbilitySystemComponent::ClearSlot( FGameplayAbilitySpec* AbilitySpec )
{
	const FGameplayTag Slot = GetInputTagFromSpec( *AbilitySpec );
	AbilitySpec->DynamicAbilityTags.RemoveTag( Slot );
}

void UAuraAbilitySystemComponent::ClearAbilitiesOfSlot( const FGameplayTag& Slot )
{
	FScopedAbilityListLock ActiveScopeLock( *this );
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilityHasSlot( &AbilitySpec, Slot ))
		{
			ClearSlot( &AbilitySpec );
		}
	}
}

bool UAuraAbilitySystemComponent::AbilityHasSlot( const FGameplayAbilitySpec* AbilitySpec, const FGameplayTag& Slot )
{
	for (const FGameplayTag& Tag : AbilitySpec->DynamicAbilityTags)
	{
		if (Tag.MatchesTagExact( Slot ))
		{
			return true;
		}
	}
	return false;
}

void UAuraAbilitySystemComponent::ServerUpgradeAttribute_Implementation( const FGameplayTag& AttributeTag )
{
	FGameplayEventData Payload;
	Payload.EventTag = AttributeTag;
	Payload.EventMagnitude = 1.f;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor( GetAvatarActor(), AttributeTag, Payload );

	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_AddToAttributePoints( GetAvatarActor(), -1 );
	}
}

void UAuraAbilitySystemComponent::ServerSpendSpellPoint_Implementation( const FGameplayTag& AbilityTag )
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag( AbilityTag ))
	{
		if (GetAvatarActor()->Implements<UPlayerInterface>())
		{
			IPlayerInterface::Execute_AddToSpellPoints( GetAvatarActor(), -1 );
		}

		const FAuraGameplayTags& AuraGameplayTags = FAuraGameplayTags::Get();
		FGameplayTag StatusTag = GetStatusTagFromSpec( *AbilitySpec );
		if (StatusTag.MatchesTagExact( AuraGameplayTags.Abilities_Status_Eligible ))
		{
			// Remove Current (Eligible) status and set new (Unlocked) status
			AbilitySpec->DynamicAbilityTags.RemoveTag( StatusTag );
			AbilitySpec->DynamicAbilityTags.AddTag( AuraGameplayTags.Abilities_Status_Unlocked );
			StatusTag = GetStatusTagFromSpec( *AbilitySpec );
		}
		else if (!StatusTag.MatchesTagExact( AuraGameplayTags.Abilities_Status_Locked ))
		{
			// Increase ability level without canceling it
			AbilitySpec->Level++;
		}
		ClientUpdateAbilityStatus( AbilityTag, StatusTag, AbilitySpec->Level );
		MarkAbilitySpecDirty( *AbilitySpec );
	}
}

void UAuraAbilitySystemComponent::ServerEquipAbility_Implementation( const FGameplayTag& AbilityTag, const FGameplayTag& Slot )
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag( AbilityTag ))
	{
		const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();
		const FGameplayTag& PreviousSlot = GetInputTagFromSpec( *AbilitySpec );
		const FGameplayTag& Status = GetStatusTagFromSpec( *AbilitySpec );

		const bool bStatusValid = Status == Tags.Abilities_Status_Equipped || Status == Tags.Abilities_Status_Unlocked;
		if (bStatusValid)
		{
			// Handle Activation/Deactivation for passive abilities

			// Check if slot is empty, if it isn't deactivate and clear its slot
			if (!SlotIsEmpty( Slot ))
			{
				FGameplayAbilitySpec* SpecWithSlot = GetSpecWithSlot( Slot );

				if (SpecWithSlot)
				{
					// is current and previous ability the same? if so return
					if (AbilityTag.MatchesTagExact( GetAbilityTagFromSpec( *SpecWithSlot ) ))
					{
						ClientEquipAbility( AbilityTag, Tags.Abilities_Status_Equipped, Slot, PreviousSlot );
						return;
					}

					if (IsPassiveAbility( *SpecWithSlot ))
					{
						MulticastActivatePassiveEffect( GetAbilityTagFromSpec( *SpecWithSlot ), false );
						DeactivatePassiveAbility.Broadcast( GetAbilityTagFromSpec( *SpecWithSlot ) );
					}

					ClearSlot( SpecWithSlot );
				}
			}

			// Ability doesn't have a slot (not active)
			if (!AbilityHasAnySlot( *AbilitySpec ))
			{
				if (IsPassiveAbility( *AbilitySpec ))
				{
					TryActivateAbility( AbilitySpec->Handle );
					MulticastActivatePassiveEffect( AbilityTag, true );
				}
			}
			AssignSlotToAbility( *AbilitySpec, Slot );

			MarkAbilitySpecDirty( *AbilitySpec );
		}

		ClientEquipAbility( AbilityTag, Tags.Abilities_Status_Equipped, Slot, PreviousSlot );
	}
}

void UAuraAbilitySystemComponent::ClientEquipAbility_Implementation( const FGameplayTag& AbilityTag, const FGameplayTag& Status,
                                                                     const FGameplayTag& Slot, const FGameplayTag& PreviousSlot )
{
	AbilityEquipped.Broadcast( AbilityTag, Status, Slot, PreviousSlot );
}

void UAuraAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();
	if (!bStartupAbilitiesGiven)
	{
		bStartupAbilitiesGiven = true;
		AbilitiesGivenDelegate.Broadcast();
	}
}

void UAuraAbilitySystemComponent::ClientUpdateAbilityStatus_Implementation( const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag,
                                                                            const int32 AbilityLevel )
{
	AbilityStatusChanged.Broadcast( AbilityTag, StatusTag, AbilityLevel );
}

void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation( UAbilitySystemComponent* AbilitySystemComponent,
                                                                      const FGameplayEffectSpec& EffectSpec,
                                                                      FActiveGameplayEffectHandle ActiveEffectHandle )
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags( TagContainer );

	EffectAssetTagsDelegate.Broadcast( TagContainer );
}
