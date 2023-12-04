// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Iteraction/PlayerInterface.h"
#include "AuraCharacter.generated.h"

class UNiagaraComponent;
/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase,  public IPlayerInterface
{
	GENERATED_BODY()
public:
	AAuraCharacter();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	/** Combat Interface */
	virtual int32 GetPlayerLevel_Implementation() override;
	/** end Combat Interface */

	/** Player Interface */
	virtual void AddToXP_Implementation(int32 InXP) override;

	virtual void LevelUp_Implementation() override;

	virtual int32 FindLevelForXP_Implementation(int32 InXP) const override;

	virtual int32 GetXP_Implementation() const override;

	virtual int32 GetAttributePointsReward_Implementation(int32 Level) const override;

	virtual int32 GetSpellPointsReward_Implementation(int32 Level) const override;

	virtual void AddToPlayerLevel_Implementation(int32 InPlayerLevel) override;

	virtual void AddToAttributePoints_Implementation(int32 InAttributePoints) override;

	virtual void AddToSpellPoints_Implementation(int32 InSpellPoints) override;

	virtual int32 GetAttributePoints_Implementation() const override;

	virtual int32 GetSpellPoints_Implementation() const override;
	/** end Player Interface */

	virtual void InitializeDefaultAttributes() override;

	virtual void OnRep_Stunned() override;
	virtual void OnRep_Burned() override;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	UPROPERTY(EditAnywhere, Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;
private:
	virtual void InitAbilityActorInfo() override;

	void AddCharacterAbilities() const;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUpParticles() const;
};
