// Copyright Maxjestic


#include "Character/AuraCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Aura/Aura.h"
#include "Components/CapsuleComponent.h"

AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel( ECC_Camera, ECR_Ignore );
	GetCapsuleComponent()->SetGenerateOverlapEvents( false );
	GetMesh()->SetCollisionResponseToChannel( ECC_Camera, ECR_Ignore );
	GetMesh()->SetCollisionResponseToChannel( ECC_Projectile, ECR_Overlap );
	GetMesh()->SetGenerateOverlapEvents( true );

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>( "Weapon" );
	Weapon->SetupAttachment( GetMesh(), FName( "WeaponHandSocket" ) );
	Weapon->SetCollisionEnabled( (ECollisionEnabled::NoCollision) );
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAnimMontage* AAuraCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void AAuraCharacterBase::Die()
{
	Weapon->DetachFromComponent( FDetachmentTransformRules( EDetachmentRule::KeepWorld, true ) );
	MulticastHandleDeath();
}

bool AAuraCharacterBase::IsDead_Implementation() const
{
	return bDead;
}

AActor* AAuraCharacterBase::GetAvatar_Implementation()
{
	return this;
}

TArray<FTaggedMontage> AAuraCharacterBase::GetAttackMontages_Implementation()
{
	return AttackMontages;
}

UNiagaraSystem* AAuraCharacterBase::GetBloodEffect_Implementation()
{
	return BloodEffect;
}

void AAuraCharacterBase::MulticastHandleDeath_Implementation()
{
	Weapon->SetSimulatePhysics( true );
	Weapon->SetEnableGravity( true );
	Weapon->SetCollisionEnabled( ECollisionEnabled::PhysicsOnly );

	GetMesh()->SetSimulatePhysics( true );
	GetMesh()->SetEnableGravity( true );
	GetMesh()->SetCollisionEnabled( ECollisionEnabled::PhysicsOnly );
	GetMesh()->SetCollisionResponseToChannel( ECC_WorldStatic, ECR_Block );

	GetCapsuleComponent()->SetCollisionEnabled( ECollisionEnabled::NoCollision );

	Dissolve();

	bDead = true;
}

void AAuraCharacterBase::InitAbilityActorInfo()
{
}

void AAuraCharacterBase::ApplyEffectToSelf( const TSubclassOf<UGameplayEffect> GameplayEffectClass, const float Level ) const
{
	check( IsValid(GetAbilitySystemComponent()) )
	check( GameplayEffectClass );

	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject( this );
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec( GameplayEffectClass, Level, ContextHandle );
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget( *SpecHandle.Data.Get(), GetAbilitySystemComponent() );
}

void AAuraCharacterBase::InitializeDefaultAttributes() const
{
}

void AAuraCharacterBase::AddCharacterAbilities() const
{
	if (!HasAuthority()) return;

	UAuraAbilitySystemComponent* AuraAbilitySystemComponent = CastChecked<UAuraAbilitySystemComponent>( AbilitySystemComponent );
	AuraAbilitySystemComponent->AddCharacterAbilities( StartupAbilities );
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

FVector AAuraCharacterBase::GetCombatSocketLocation_Implementation( const FGameplayTag& MontageTag )
{
	// TODO change to more versatile solution (TMap montage tags to socket names)
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	if (MontageTag.MatchesTagExact( GameplayTags.Montage_Attack_Weapon ) && IsValid( Weapon ))
	{
		return Weapon->GetSocketLocation( WeaponTipSocketName );
	}
	if (MontageTag.MatchesTagExact( GameplayTags.Montage_Attack_LeftHand ))
	{
		return GetMesh()->GetSocketLocation( LeftHandSocketName );
	}
	if (MontageTag.MatchesTagExact( GameplayTags.Montage_Attack_RightHand ))
	{
		return GetMesh()->GetSocketLocation( RightHandSocketName );
	}
	return FVector();
}

void AAuraCharacterBase::Dissolve()
{
	if (IsValid( MeshDissolveMaterialInstance ))
	{
		UMaterialInstanceDynamic* DynamicMaterialInstance = UMaterialInstanceDynamic::Create( MeshDissolveMaterialInstance, this );
		GetMesh()->SetMaterial( 0, DynamicMaterialInstance );
		MeshStartDissolveTimeline( DynamicMaterialInstance );
	}
	if (IsValid( WeaponDissolveMaterialInstance ))
	{
		UMaterialInstanceDynamic* DynamicMaterialInstance = UMaterialInstanceDynamic::Create( WeaponDissolveMaterialInstance, this );
		Weapon->SetMaterial( 0, DynamicMaterialInstance );
		WeaponStartDissolveTimeline( DynamicMaterialInstance );
	}
}
