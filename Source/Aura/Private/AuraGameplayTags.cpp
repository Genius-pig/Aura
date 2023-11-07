// Copyright Druid Mechanics

#include "AuraGameplayTags.h"

#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::GameplayTags;

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Strength"), FString("Increases physical damage"));
	GameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Intelligence"), FString("Increases magical damage"));
	GameplayTags.Attributes_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Resilience"), FString("Increases Armor and Armor Penetration"));
	GameplayTags.Attributes_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Vigor"), FString("Increases Health"));
	
	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"), FString("Reduce damage taken, improve Block chance"));
	GameplayTags.Attributes_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.ArmorPenetration"), FString("Ignores Percentage of enemy Armor, increases Critical Hit Chance"));
	GameplayTags.Attributes_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.BlockChance"), FString("Chance to cut incoming damage in half"));
	GameplayTags.Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitChance"), FString("Chance to double damage plus critical hit bonus"));
	GameplayTags.Attributes_Secondary_CriticalHitDamage =  UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalDamage"), FString("Bonus damage added when a critical hit is scored"));
	GameplayTags.Attributes_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitResistance"), FString("Reduces Critical Hit Chance of attacking enemies"));
	GameplayTags.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.HealthRegeneration"), FString("Amount of Health regenerated every 1 second"));
	GameplayTags.Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.ManaRegeneration"), FString("Amount of Mana regenerated every 1 second"));
	GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxHealth"), FString("Maximum amount of Health obtainable"));
	GameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxMana"), FString("Maximum amount of Mana obtainable"));

	
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.LMB"), FString("Input tag for left mouse button"));
	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.RMB"), FString("Input tag for right mouse button"));
	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.1"), FString("Input tag for 1 key"));
	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.2"), FString("Input tag for 2 key"));
	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.3"), FString("Input tag for 3 key"));
	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.4"), FString("Input tag for 4 key"));

	/*
	 * Abilities Status And Types
	 */

	GameplayTags.Abilities_Status_Eligible = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Eligible"),
		FString("Eligible Status")
		);

	GameplayTags.Abilities_Status_Equipped = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Equipped"),
		FString("Equipped Status")
		);

	GameplayTags.Abilities_Status_Locked = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Locked"),
		FString("Locked Status")
		);

	GameplayTags.Abilities_Status_Unlocked = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Unlocked"),
		FString("Unlocked Status")
		);

	GameplayTags.Abilities_Type_None = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Type.None"),
		FString("Type None")
		);

	GameplayTags.Abilities_Type_Offensive = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Type.Offensive"),
		FString("Type Offensive")
		);

	GameplayTags.Abilities_Type_Passive = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Type.Passive"),
		FString("Type Passive")
		);

	GameplayTags.InputTag_Passive_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("InputTag.Passive.1"),
	FString("Input Tag Passive Ability 1")
	);

	GameplayTags.InputTag_Passive_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Passive.2"),
		FString("Input Tag Passive Ability 2")
		);


	/*
	 * Damage Types
	 */
	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage"), FString("Damage"));
	GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Fire"),
		FString("Fire Damage Type")
		);
	GameplayTags.Damage_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Lightning"),
		FString("Lightning Damage Type")
		);
	GameplayTags.Damage_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Arcane"),
		FString("Arcane Damage Type")
		);
	GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Physical"),
		FString("Physical Damage Type")
		);


	/*Resistance to damage*/
	GameplayTags.Attributes_Resistance_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Arcane"),
		FString("Resistance to Arcane damage")
	);
	GameplayTags.Attributes_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Fire"),
		FString("Resistance to Fire damage")
	);
	GameplayTags.Attributes_Resistance_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Lightning"),
		FString("Resistance to Lightning damage")
	);
	GameplayTags.Attributes_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Physical"),
		FString("Resistance to Physical damage")
	);

	/*
 * DeBuffs
 */

	GameplayTags.DeBuff_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("DeBuff.Arcane"),
		FString("DeBuff for Arcane damage")
		);
	GameplayTags.DeBuff_Burn = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("DeBuff.Burn"),
		FString("DeBuff for Fire damage")
		);
	GameplayTags.DeBuff_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("DeBuff.Physical"),
		FString("DeBuff for Physical damage")
		);
	GameplayTags.DeBuff_Stun = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("DeBuff.Stun"),
		FString("DeBuff for Lightning damage")
		);

	/*abilities*/
	GameplayTags.Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Attack"),
		FString("Attack Ability Tag")
	);

	GameplayTags.Abilities_Summon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Summon"),
		FString("Summon Ability Tag")
	);

	GameplayTags.Abilities_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Fire.FireBolt"),
		FString("FireBolt Ability Tag")
	);

	GameplayTags.Abilities_Lightning_Electrocute = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Lightning.Electrocute"),
		FString("Electrocute Ability Tag")
	);

	GameplayTags.Abilities_None = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.None"),
		FString("No Ability - like the nullptr for Ability Tags")
	);

	
	/*
	 * Cooldown
	 */

	GameplayTags.Cooldown_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Fire.FireBolt"),
		FString("FireBolt Cooldown Tag")
	);

	/*effect*/
	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.HitResult"), FString("Hit result of effects"));
	GameplayTags.Effects_FireArea = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.FireArea"), FString("Effects FireArea"));


	/*
	 * Meta Attributes
	 */
	
	GameplayTags.Attributes_Meta_IncomingXP = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Meta.IncomingXP"),
		FString("Incoming XP Meta Attribute")
		);

	/*Attack tags used for montage and get weapon socket location*/
	GameplayTags.Attack_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attack.Weapon"),
		FString("Weapon")
		);

	GameplayTags.Attack_RightHand = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attack.RightHand"),
		FString("Right Hand")
		);

	GameplayTags.Attack_LeftHand = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attack.LeftHand"),
		FString("Left Hand")
		);

	GameplayTags.Attack_Tail = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attack.Tail"),
	FString("Tail")
	);

	GameplayTags.Attack_Summon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attack.Summon"),
		FString("Summon")
	);

	GameplayTags.DeBuff_Chance = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("DeBuff.Chance"),
	FString("DeBuff Chance")
	);
	GameplayTags.DeBuff_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("DeBuff.Damage"),
		FString("DeBuff Damage")
		);
	GameplayTags.DeBuff_Duration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("DeBuff.Duration"),
		FString("DeBuff Duration")
		);
	GameplayTags.DeBuff_Frequency = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("DeBuff.Frequency"),
		FString("DeBuff Frequency")
		);

	/*
     * Map of Damage Types to DeBuffs
     */
	GameplayTags.DamageTypesToDeBuffs.Add(GameplayTags.Damage_Arcane, GameplayTags.DeBuff_Arcane);
	GameplayTags.DamageTypesToDeBuffs.Add(GameplayTags.Damage_Lightning, GameplayTags.DeBuff_Stun);
	GameplayTags.DamageTypesToDeBuffs.Add(GameplayTags.Damage_Physical, GameplayTags.DeBuff_Physical);
	GameplayTags.DamageTypesToDeBuffs.Add(GameplayTags.Damage_Fire, GameplayTags.DeBuff_Burn);
}
