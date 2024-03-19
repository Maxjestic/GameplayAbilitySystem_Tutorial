// Copyright Maxjestic


#include "Character/AuraCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "AbilitySystem/Passive/PassiveNiagaraComponent.h"
#include "Aura/Aura.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	BurnDebuffComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>( FName( "BurnDebuffComponent" ) );
	BurnDebuffComponent->SetupAttachment( GetRootComponent() );
	BurnDebuffComponent->DebuffTag = GameplayTags.Debuff_Burn;

	StunDebuffComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>( FName( "StunDebuffComponent" ) );
	StunDebuffComponent->SetupAttachment( GetRootComponent() );
	StunDebuffComponent->DebuffTag = GameplayTags.Debuff_Stun;

	GetCapsuleComponent()->SetCollisionResponseToChannel( ECC_Camera, ECR_Ignore );
	GetCapsuleComponent()->SetGenerateOverlapEvents( false );
	GetMesh()->SetCollisionResponseToChannel( ECC_Camera, ECR_Ignore );
	GetMesh()->SetCollisionResponseToChannel( ECC_Projectile, ECR_Overlap );
	GetMesh()->SetGenerateOverlapEvents( true );

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>( "Weapon" );
	Weapon->SetupAttachment( GetMesh(), FName( "WeaponHandSocket" ) );
	Weapon->SetCollisionEnabled( (ECollisionEnabled::NoCollision) );

	EffectAttachComponent = CreateDefaultSubobject<USceneComponent>( "EffectAttachPoint" );
	EffectAttachComponent->SetupAttachment( GetRootComponent() );

	HaloOfProtectionNiagaraComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>( "HaloOfProtectionComponent" );
	HaloOfProtectionNiagaraComponent->SetupAttachment( EffectAttachComponent );
	LifeSiphonNiagaraComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>( "LifeSiphonComponent" );
	LifeSiphonNiagaraComponent->SetupAttachment( EffectAttachComponent );
	ManaSiphonNiagaraComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>( "ManaSiphonComponent" );
	ManaSiphonNiagaraComponent->SetupAttachment( EffectAttachComponent );
}

void AAuraCharacterBase::GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );

	DOREPLIFETIME( AAuraCharacterBase, bIsBurned );
	DOREPLIFETIME( AAuraCharacterBase, bIsStunned );
	DOREPLIFETIME( AAuraCharacterBase, bIsBeingShocked );
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraCharacterBase::Tick( float DeltaSeconds )
{
	Super::Tick( DeltaSeconds );

	EffectAttachComponent->SetWorldRotation( FRotator::ZeroRotator );
}

float AAuraCharacterBase::TakeDamage( float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                                      AActor* DamageCauser )
{
	const float DamageTaken = Super::TakeDamage( DamageAmount, DamageEvent, EventInstigator, DamageCauser );
	OnDamage.Broadcast( DamageTaken );
	return DamageTaken;
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

FOnDeathSignature& AAuraCharacterBase::GetOnDeathDelegate()
{
	return OnDeath;
}

FOnDamageSignature& AAuraCharacterBase::GetOnDamageDelegate()
{
	return OnDamage;
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

FOnAbilitySystemComponentRegisteredSignature& AAuraCharacterBase::GetOnAbilitySystemComponentRegistered()
{
	return OnAbilitySystemComponentRegistered;
}

USkeletalMeshComponent* AAuraCharacterBase::GetWeapon_Implementation()
{
	return Weapon;
}

bool AAuraCharacterBase::IsBeingShocked_Implementation() const
{
	return bIsBeingShocked;
}

void AAuraCharacterBase::SetIsBeingShocked_Implementation( bool bInIsBeingShocked )
{
	bIsBeingShocked = bInIsBeingShocked;
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
	StunDebuffComponent->Deactivate();
	OnDeath.Broadcast( this );
}

void AAuraCharacterBase::StunTagChanged( const FGameplayTag CallbackTag, int32 NewCount )
{
	bIsStunned = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bIsStunned ? 0.f : BaseWalkSpeed;
}

void AAuraCharacterBase::OnRep_Burned()
{
}

void AAuraCharacterBase::OnRep_Stunned()
{
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
