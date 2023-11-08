// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Iteraction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UDeBuffNiagaraComponent;
class UNiagaraSystem;
class UCharacterClassInfo;
class UGameplayAbility;
class UGameplayEffect;
class UAttributeSet;
class UAbilitySystemComponent;

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:

	AAuraCharacterBase();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditAnywhere, Category="Combat")
	FName WeaponTipSocketName;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName LeftHandSocketName;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName RightHandSocketName;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName TailSocketName;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

	void Dissolve();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 600.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	UNiagaraSystem* BloodEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	USoundBase* DeathSound;

	/* Minions */

	int32 MinionCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	ECharacterClass CharacterType = ECharacterClass::Warrior;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDeBuffNiagaraComponent> BurnDeBuffComponent;
public:
	UAttributeSet* GetAttributeSet() const
	{
		return AttributeSet;
	}

	// Combat interface
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& AttackTag) override;

	virtual UAnimMontage* GetHitReactMontage_Implementation() override;

	virtual void Die(const FVector& DeathImpulse) override;

	virtual AActor* GetAvatar_Implementation() override;

	virtual bool IsDead_Implementation() const override;

	virtual TArray<FTaggedMontage> GetAttackMontage_Implementation() override;

	virtual FTaggedMontage GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag) override;

	virtual int32 GetMinionCount_Implementation() override;

	virtual void IncrementMinionCount_Implementation(int32 Amount) override;

	virtual ECharacterClass GetCharacterClass_Implementation() override;

	virtual FOnASCRegistered GetOnASCRegisteredDelegate() override;
	// end Combat interface

	FOnASCRegistered OnAscRegistered;

	// AttackMontage should be AttackMontages. But I don't want to change it because the renaming action would break assets' links.
	UPROPERTY(EditAnywhere, Category="Combat")
	TArray<FTaggedMontage> AttackMontage;

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath(const FVector& DeathImpulse);

	virtual UNiagaraSystem* GetBloodEffect_Implementation() override;

	// dissolve effect
	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic* MaterialInstanceDynamic);

	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* WeaponMaterialInstanceDynamic);

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:

	virtual void InitAbilityActorInfo();
	
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, int Level) const;

	virtual void InitializeDefaultAttributes();

	bool bDead = false;
};
