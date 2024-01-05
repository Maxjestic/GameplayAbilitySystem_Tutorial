// Copyright Maxjestic


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController( const UObject* WorldContextObject )
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController( WorldContextObject, 0 );
	if (PlayerController == nullptr)
	{
		return nullptr;
	}

	AAuraHUD* AuraHUD = PlayerController->GetHUD<AAuraHUD>();
	if (AuraHUD == nullptr)
	{
		return nullptr;
	}

	AAuraPlayerState* PlayerState = PlayerController->GetPlayerState<AAuraPlayerState>();
	UAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
	UAttributeSet* AttributeSet = PlayerState->GetAttributeSet();

	const FWidgetControllerParams WidgetControllerParams( PlayerController,
	                                                      PlayerState,
	                                                      AbilitySystemComponent,
	                                                      AttributeSet );
	return AuraHUD->GetOverlayWidgetController( WidgetControllerParams );
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject )
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController( WorldContextObject, 0 );
	if (PlayerController == nullptr)
	{
		return nullptr;
	}

	AAuraHUD* AuraHUD = PlayerController->GetHUD<AAuraHUD>();
	if (AuraHUD == nullptr)
	{
		return nullptr;
	}

	AAuraPlayerState* PlayerState = PlayerController->GetPlayerState<AAuraPlayerState>();
	UAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
	UAttributeSet* AttributeSet = PlayerState->GetAttributeSet();

	const FWidgetControllerParams WidgetControllerParams( PlayerController,
	                                                      PlayerState,
	                                                      AbilitySystemComponent,
	                                                      AttributeSet );
	return AuraHUD->GetAttributeMenuWidgetController( WidgetControllerParams );
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
                                                      UAbilitySystemComponent* AbilitySystemComponent )
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo( WorldContextObject );
	for (const TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec( AbilityClass, 1 );
		AbilitySystemComponent->GiveAbility( AbilitySpec );
	}
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
