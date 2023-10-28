// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

struct FAuraGameplayTags
{
public:
	static const FAuraGameplayTags& Get() {return GameplayTags;}
	static void InitializeNativeGameplayTags();

	// primary tags
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Resilience;
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Vigor;

	// secondary tags
	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_ArmorPenetration;
	FGameplayTag Attributes_Secondary_BlockChance;
	FGameplayTag Attributes_Secondary_CriticalHitChance;
	FGameplayTag Attributes_Secondary_CriticalHitResistance;
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	FGameplayTag Attributes_Secondary_ManaRegeneration;
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;

	//input tags
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;
	FGameplayTag InputTag_Passive_1;
	FGameplayTag InputTag_Passive_2;

	// damage types
	FGameplayTag Damage;
	FGameplayTag Damage_Lightning;
	FGameplayTag Damage_Arcane;
	FGameplayTag Damage_Physical;
	FGameplayTag Damage_Fire;

	// resistance to damage
	FGameplayTag Attributes_Resistance_Fire;
	FGameplayTag Attributes_Resistance_Lightning;
	FGameplayTag Attributes_Resistance_Arcane;
	FGameplayTag Attributes_Resistance_Physical;

	// DeBuff
	FGameplayTag DeBuff_Burn;
	FGameplayTag DeBuff_Stun;
	FGameplayTag DeBuff_Arcane;
	FGameplayTag DeBuff_Physical;

	FGameplayTag DeBuff_Chance;
	FGameplayTag DeBuff_Damage;
	FGameplayTag DeBuff_Duration;
	FGameplayTag DeBuff_Frequency;

	// abilities
	FGameplayTag Abilities_Attack;
	FGameplayTag Abilities_Summon;
	FGameplayTag Abilities_Fire_FireBolt;
	FGameplayTag Abilities_Lightning_Electrocute;
	FGameplayTag Abilities_None;

	//Abilities Status
	FGameplayTag Abilities_Status_Locked;
	FGameplayTag Abilities_Status_Eligible;
	FGameplayTag Abilities_Status_Unlocked;
	FGameplayTag Abilities_Status_Equipped;

	//Abilities Type
	FGameplayTag Abilities_Type_Offensive;
	FGameplayTag Abilities_Type_Passive;
	FGameplayTag Abilities_Type_None;

	FGameplayTag Cooldown_Fire_FireBolt;

	// Effect actor
	FGameplayTag Effects_FireArea;
	FGameplayTag Effects_HitResult;

    // Attack for montages and sounds
	FGameplayTag Attack_Weapon;
	FGameplayTag Attack_RightHand;
	FGameplayTag Attack_LeftHand;
	FGameplayTag Attack_Tail;
	FGameplayTag Attack_Summon;

	// meta attributes
	FGameplayTag Attributes_Meta_IncomingXP;

	
	TMap<FGameplayTag, FGameplayTag> DamageTypesToDeBuffs;

private:
	
	static FAuraGameplayTags GameplayTags;
	
};
