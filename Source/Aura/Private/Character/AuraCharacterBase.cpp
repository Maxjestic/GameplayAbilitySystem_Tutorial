// Copyright Maxjestic


#include "Character/AuraCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "Aura/Aura.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	BurnDebuffComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>( FName("BurnDebuffComponent") );
	BurnDebuffComponent->SetupAttachment( GetRootComponent() );
	BurnDebuffComponent->DebuffTag = FAuraGameplayTags::Get().Debuff_Burn;

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

void AAuraCharacterBase::Die( const FVector& DeathImpulse )
{
	Weapon->DetachFromComponent( FDetachmentTransformRules( EDetachmentRule::KeepWorld, true ) );
	MulticastHandleDeath( DeathImpulse );
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

FTaggedMontage AAuraCharacterBase::GetTaggedMontageByTag_Implementation( const FGameplayTag& MontageTag )
{
	for (const FTaggedMontage TaggedMontage : AttackMontages)
	{
		if (TaggedMontage.MontageTag.MatchesTagExact( MontageTag ))
		{
			return TaggedMontage;
		}
	}
	return FTaggedMontage();
}

int32 AAuraCharacterBase::GetMinionCount_Implementation()
{
	return MinionCount;
}

void AAuraCharacterBase::UpdateMinionCount_Implementation( const int32 Amount )
{
	MinionCount += Amount;
}

ECharacterClass AAuraCharacterBase::GetCharacterClass_Implementation()
{
	return CharacterClass;
}

FOnAbilitySystemComponentRegisteredSignature AAuraCharacterBase::GetOnAbilitySystemComponentDelegate()
{
	return OnAbilitySystemComponentRegistered;
}

USkeletalMeshComponent* AAuraCharacterBase::GetWeapon_Implementation()
{
	return Weapon;
}

void AAuraCharacterBase::MulticastHandleDeath_Implementation( const FVector& DeathImpulse )
{
	UGameplayStatics::PlaySoundAtLocation( this, DeathSound, GetActorLocation(), GetActorRotation() );

	Weapon->SetSimulatePhysics( true );
	Weapon->SetEnableGravity( true );
	Weapon->SetCollisionEnabled( ECollisionEnabled::PhysicsOnly );
	Weapon->AddImpulse( DeathImpulse * 0.1f, NAME_None, true );

	GetMesh()->SetSimulatePhysics( true );
	GetMesh()->SetEnableGravity( true );
	GetMesh()->SetCollisionEnabled( ECollisionEnabled::PhysicsOnly );
	GetMesh()->SetCollisionResponseToChannel( ECC_WorldStatic, ECR_Block );
	GetMesh()->AddImpulse( DeathImpulse, NAME_None, true );

	GetCapsuleComponent()->SetCollisionEnabled( ECollisionEnabled::NoCollision );

	Dissolve();

	bDead = true;
	BurnDebuffComponent->Deactivate();
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
	AuraAbilitySystemComponent->AddCharacterPassiveAbilities( StartupPassiveAbilities );
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

FVector AAuraCharacterBase::GetCombatSocketLocation_Implementation( const FGameplayTag& MontageTag )
{
	// TODO change to more versatile solution (TMap montage tags to socket names)
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	if (MontageTag.MatchesTagExact( GameplayTags.CombatSocket_Weapon ) && IsValid( Weapon ))
	{
		return Weapon->GetSocketLocation( WeaponTipSocketName );
	}
	if (MontageTag.MatchesTagExact( GameplayTags.CombatSocket_LeftHand ))
	{
		return GetMesh()->GetSocketLocation( LeftHandSocketName );
	}
	if (MontageTag.MatchesTagExact( GameplayTags.CombatSocket_RightHand ))
	{
		return GetMesh()->GetSocketLocation( RightHandSocketName );
	}
	if (MontageTag.MatchesTagExact( GameplayTags.CombatSocket_Tail ))
	{
		return GetMesh()->GetSocketLocation( TailSocketName );
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
