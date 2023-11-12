// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraDamageGameplayAbility.generated.h"

/**
 * 
 */

struct FDamageEffectParams;
struct FTaggedMontage;
UCLASS()
class AURA_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure)
	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(
		AActor* TargetActor = nullptr,
		FVector InRadialDamageOrigin = FVector::ZeroVector,
		bool bOverrideKnockBackDirection = false,
		FVector KnockBackDirectionOverride = FVector::ZeroVector,
		bool bOverrideDeathImpulse = false,
		FVector DeathImpulseDirectionOverride = FVector::ZeroVector,
		bool bOverridePitch = false,
		float PitchOverride = 0.f) const;
	

	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);

	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const;
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
	UPROPERTY(EditDefaultsOnly, Category="Damage")
	FScalableFloat Damage;

public:
	FScalableFloat GetDamage() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DeBuffChance = 20.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DeBuffDamage = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DeBuffFrequency = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DeBuffDuration = 5.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DeathImpulseMagnitude = 60.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag DamageType;
};
