// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystem/DeBuff/DeBuffNiagaraComponent.h"
#include "Aura/Aura.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


AAuraCharacterBase::AAuraCharacterBase()
{
 	
	PrimaryActorTick.bCanEverTick = false;
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	BurnDeBuffComponent = CreateDefaultSubobject<UDeBuffNiagaraComponent>("BurnDeBuffComponent");
	BurnDeBuffComponent->SetupAttachment(GetRootComponent());
	BurnDeBuffComponent->DeBuffTag = GameplayTags.DeBuff_Burn;

	StunDeBuffComponent = CreateDefaultSubobject<UDeBuffNiagaraComponent>("StunDeBuffComponent");
	StunDeBuffComponent->SetupAttachment(GetRootComponent());
	StunDeBuffComponent->DeBuffTag = GameplayTags.DeBuff_Stun;
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");

	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AAuraCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAuraCharacterBase, bIsStunned);
	DOREPLIFETIME(AAuraCharacterBase, bIsBurned);
}


void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraCharacterBase::Dissolve()
{
	if(IsValid(DissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* InstanceDynamic = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
		GetMesh()->SetMaterial(0, InstanceDynamic);
		StartDissolveTimeline(InstanceDynamic);
	}
	if(IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* WeaponInstanceDynamic = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);
		Weapon->SetMaterial(0, WeaponInstanceDynamic);
		StartWeaponDissolveTimeline(WeaponInstanceDynamic);
	}
}

void AAuraCharacterBase::OnRep_Stunned()
{
}

void AAuraCharacterBase::OnRep_Burned()
{
}

UNiagaraSystem* AAuraCharacterBase::GetBloodEffect_Implementation()
{
	return BloodEffect;
}

FVector AAuraCharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag& AttackTag)
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	if (AttackTag.MatchesTagExact(GameplayTags.Attack_Weapon) && IsValid(Weapon))
	{
		return Weapon->GetSocketLocation(WeaponTipSocketName);
	}
	if (AttackTag.MatchesTagExact(GameplayTags.Attack_LeftHand))
	{
		return GetMesh()->GetSocketLocation(LeftHandSocketName);
	}
	if (AttackTag.MatchesTagExact(GameplayTags.Attack_RightHand))
	{
		return GetMesh()->GetSocketLocation(RightHandSocketName);
	}
	if (AttackTag.MatchesTagExact(GameplayTags.Attack_Tail))
	{
		return GetMesh()->GetSocketLocation(TailSocketName);
	}
	return FVector();
}

UAnimMontage* AAuraCharacterBase::GetHitReactMontage_Implementation()
{
	return  HitReactMontage;
}

void AAuraCharacterBase::Die(const FVector& DeathImpulse)
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MulticastHandleDeath(DeathImpulse);
}

FOnDeathSignature& AAuraCharacterBase::GetOnDeathDelegate()
{
	return OnDeathDelegate;
}

AActor* AAuraCharacterBase::GetAvatar_Implementation()
{
	return this;
}

bool AAuraCharacterBase::IsDead_Implementation() const
{
	return bDead;
}

TArray<FTaggedMontage> AAuraCharacterBase::GetAttackMontage_Implementation()
{
	return AttackMontage;
}

FTaggedMontage AAuraCharacterBase::GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag)
{
	for (FTaggedMontage TaggedMontage : AttackMontage)
	{
		if (TaggedMontage.AttackTag == MontageTag)
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

void AAuraCharacterBase::IncrementMinionCount_Implementation(int32 Amount)
{
	MinionCount += Amount;
}

ECharacterClass AAuraCharacterBase::GetCharacterClass_Implementation()
{
	return CharacterType;
}

FOnASCRegistered& AAuraCharacterBase::GetOnASCRegisteredDelegate()
{
	return OnAscRegistered;
}

USkeletalMeshComponent* AAuraCharacterBase::GetWeapon_Implementation()
{
	return Weapon;
}

void AAuraCharacterBase::SetIsBeingShocked_Implementation(bool bInShock)
{
	bIsBeingShocked = bInShock;
}

bool AAuraCharacterBase::IsBeingShocked_Implementation() const
{
	return bIsBeingShocked;
}

void AAuraCharacterBase::MulticastHandleDeath_Implementation(const FVector& DeathImpulse)
{
	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation(), GetActorRotation());
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	// why physicsOnly? 
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Weapon->AddImpulse(DeathImpulse * 0.1f, NAME_None, true);
	
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	GetMesh()->AddImpulse(DeathImpulse, NAME_None, true);

	Dissolve();
	bDead = true;
	BurnDeBuffComponent->Deactivate();
	StunDeBuffComponent->Deactivate();
	OnDeathDelegate.Broadcast(this);
}


UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraCharacterBase::InitAbilityActorInfo()
{
}

void AAuraCharacterBase::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect> GameplayEffectClass, const int Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void AAuraCharacterBase::InitializeDefaultAttributes()
{
}

void AAuraCharacterBase::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bIsStunned = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bIsStunned ? 0.f : BaseWalkSpeed;
}



