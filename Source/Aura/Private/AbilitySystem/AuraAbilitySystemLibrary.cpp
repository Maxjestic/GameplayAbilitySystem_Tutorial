// Copyright Maxjestic


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AuraAbilityTypes.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"

bool UAuraAbilitySystemLibrary::MakeWidgetControllerParams( const UObject* WorldContextObject,
                                                            FWidgetControllerParams& OutWidgetControllerParams, AAuraHUD*& OutAuraHUD )
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController( WorldContextObject, 0 ))
	{
		OutAuraHUD = PlayerController->GetHUD<AAuraHUD>();
		if (OutAuraHUD)
		{
			AAuraPlayerState* PlayerState = PlayerController->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
			UAttributeSet* AttributeSet = PlayerState->GetAttributeSet();

			OutWidgetControllerParams.PlayerController = PlayerController;
			OutWidgetControllerParams.PlayerState = PlayerState;
			OutWidgetControllerParams.AbilitySystemComponent = AbilitySystemComponent;
			OutWidgetControllerParams.AttributeSet = AttributeSet;

			return true;
		}
	}
	return false;
}

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController( const UObject* WorldContextObject )
{
	FWidgetControllerParams WidgetControllerParams;
	AAuraHUD* AuraHUD = nullptr;

	if (MakeWidgetControllerParams( WorldContextObject, WidgetControllerParams, AuraHUD ))
	{
		return AuraHUD->GetOverlayWidgetController( WidgetControllerParams );
	}

	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject )
{
	FWidgetControllerParams WidgetControllerParams;
	AAuraHUD* AuraHUD = nullptr;

	if (MakeWidgetControllerParams( WorldContextObject, WidgetControllerParams, AuraHUD ))
	{
		return AuraHUD->GetAttributeMenuWidgetController( WidgetControllerParams );
	}

	return nullptr;
}

USpellMenuWidgetController* UAuraAbilitySystemLibrary::GetSpellMenuWidgetController( const UObject* WorldContextObject )
{
	FWidgetControllerParams WidgetControllerParams;
	AAuraHUD* AuraHUD = nullptr;

	if (MakeWidgetControllerParams( WorldContextObject, WidgetControllerParams, AuraHUD ))
	{
		return AuraHUD->GetSpellMenuWidgetController( WidgetControllerParams );
	}

	return nullptr;
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes( const UObject* WorldContextObject,
                                                             ECharacterClass CharacterClass, float Level,
                                                             UAbilitySystemComponent* AbilitySystemComponent )
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo( WorldContextObject );
	const FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo( CharacterClass );

	const AActor* AvatarActor = AbilitySystemComponent->GetAvatarActor();

	FGameplayEffectContextHandle AttributesContextHandle = AbilitySystemComponent->MakeEffectContext();
	AttributesContextHandle.AddSourceObject( AvatarActor );

	const FGameplayEffectSpecHandle PrimaryAttributeSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
		ClassDefaultInfo.PrimaryAttributes,
		Level,
		AttributesContextHandle );
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf( *PrimaryAttributeSpecHandle.Data.Get() );

	const FGameplayEffectSpecHandle SecondaryAttributeSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
		CharacterClassInfo->SecondaryAttributes,
		Level,
		AttributesContextHandle );
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf( *SecondaryAttributeSpecHandle.Data.Get() );

	const FGameplayEffectSpecHandle VitalAttributeSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
		CharacterClassInfo->VitalAttributes,
		Level,
		AttributesContextHandle );
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf( *VitalAttributeSpecHandle.Data.Get() );
}

void UAuraAbilitySystemLibrary::GiveStartupAbilities( const UObject* WorldContextObject,
                                                      UAbilitySystemComponent* AbilitySystemComponent, ECharacterClass CharacterClass )
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo( WorldContextObject );

	if (CharacterClassInfo == nullptr)
	{
		return;
	}

	for (const TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec( AbilityClass, 1 );
		AbilitySystemComponent->GiveAbility( AbilitySpec );
	}

	const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo( CharacterClass );
	for (const TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbilities)
	{
		if (AbilitySystemComponent->GetAvatarActor()->Implements<UCombatInterface>())
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec( AbilityClass,
			                                                         ICombatInterface::Execute_GetCharacterLevel(
				                                                         AbilitySystemComponent->GetAvatarActor() ) );
			AbilitySystemComponent->GiveAbility( AbilitySpec );
		}
	}
}

int32 UAuraAbilitySystemLibrary::GetExperienceRewardForClassAndLevel( const UObject* WorldContextObject,
                                                                      const ECharacterClass CharacterClass,
                                                                      const int32 CharacterLevel )
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo( WorldContextObject );

	if (CharacterClassInfo == nullptr)
	{
		return 0;
	}

	const FCharacterClassDefaultInfo& ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo( CharacterClass );
	const float ExperienceReward = ClassDefaultInfo.ExperienceReward.GetValueAtLevel( CharacterLevel );
	return static_cast<int32>(ExperienceReward);
}

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo( const UObject* WorldContextObject )
{
	const AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(
		UGameplayStatics::GetGameMode( WorldContextObject ) );
	if (AuraGameMode == nullptr)
	{
		return nullptr;
	}

	return AuraGameMode->CharacterClassInfo;
}

bool UAuraAbilitySystemLibrary::IsBlockedHit( const FGameplayEffectContextHandle& EffectContextHandle )
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->IsBlockedHit();
	}
	return false;
}

bool UAuraAbilitySystemLibrary::IsCriticalHit( const FGameplayEffectContextHandle& EffectContextHandle )
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->IsCriticalHit();
	}
	return false;
}

void UAuraAbilitySystemLibrary::SetIsBlockedHit( FGameplayEffectContextHandle& EffectContextHandle, const bool bInIsBlockedHit )
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetIsBlockedHit( bInIsBlockedHit );
	}
}

void UAuraAbilitySystemLibrary::SetIsCriticalHit( FGameplayEffectContextHandle& EffectContextHandle, const bool bInIsCriticalHit )
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetIsCriticalHit( bInIsCriticalHit );
	}
}

void UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius( const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors,
                                                            const TArray<AActor*>& ActorsToIgnore, float Radius,
                                                            const FVector& SphereOrigin )
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors( ActorsToIgnore );

	TArray<FOverlapResult> Overlaps;
	if (const UWorld* World = GEngine->GetWorldFromContextObject( WorldContextObject, EGetWorldErrorMode::LogAndReturnNull ))
	{
		World->OverlapMultiByObjectType( Overlaps,
		                                 SphereOrigin,
		                                 FQuat::Identity,
		                                 FCollisionObjectQueryParams( FCollisionObjectQueryParams::InitType::AllDynamicObjects ),
		                                 FCollisionShape::MakeSphere( Radius ),
		                                 SphereParams );
		for (FOverlapResult Overlap : Overlaps)
		{
			if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead( Overlap.GetActor() ))
			{
				OutOverlappingActors.AddUnique( ICombatInterface::Execute_GetAvatar( Overlap.GetActor() ) );
			}
		}
	}
}

bool UAuraAbilitySystemLibrary::IsNotFriend( const AActor* FirstActor, const AActor* SecondActor )
{
	const bool bBothArePlayers = FirstActor->ActorHasTag( FName( "Player" ) ) && SecondActor->ActorHasTag( FName( "Player" ) );
	const bool bBothAreEnemies = FirstActor->ActorHasTag( FName( "Enemy" ) ) && SecondActor->ActorHasTag( FName( "Enemy" ) );

	return !(bBothArePlayers || bBothAreEnemies);
}
