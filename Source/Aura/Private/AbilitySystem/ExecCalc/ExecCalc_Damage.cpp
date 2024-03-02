// Copyright Druid Mechanics


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Iteraction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"

//FAuraDamageStatics is used for getting Attribute value
struct FAuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);

	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);

	FAuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitDamage, Source, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, FireResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, LightningResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArcaneResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, PhysicalResistance, Target, false);
	}
	
};

static const FAuraDamageStatics& DamageStatics()
{
	static const FAuraDamageStatics DamageStatics;
	return DamageStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	/** Attributes to capture that are relevant to the calculation */
	/** Vitial Attributes */
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);

	RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC? TargetASC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTagContainer = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTagContainer = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTagContainer;
	EvaluateParameters.TargetTags = TargetTagContainer;
	//get Aura Effect Context handle
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();

	int32 SourcePlayerLevel = 1;
	int32 TargetPlayerLevel = 1;

	if (SourceAvatar->Implements<UCombatInterface>())
	{
		SourcePlayerLevel = ICombatInterface::Execute_GetPlayerLevel(SourceAvatar);
	}
	if (TargetAvatar->Implements<UCombatInterface>())
	{
		TargetPlayerLevel = ICombatInterface::Execute_GetPlayerLevel(TargetAvatar);
	}

	float Damage = 0.f;

	if(SourceTagContainer->HasTagExact(FAuraGameplayTags::Get().Damage_Fire))
	{
		DetermineDeBuff(ExecutionParams, Spec, EvaluateParameters, FAuraGameplayTags::Get().Damage_Fire, FAuraGameplayTags::Get().Attributes_Resistance_Fire, DamageStatics().FireResistanceDef);
		float DamageTypeValue = Spec.GetSetByCallerMagnitude(FAuraGameplayTags::Get().Damage_Fire);
		float TargetFireResistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().FireResistanceDef, EvaluateParameters, TargetFireResistance);
		DamageTypeValue *= ( 100.f - TargetFireResistance ) / 100.f;
		Damage += DamageTypeValue;
	}
	else if (SourceTagContainer->HasTagExact(FAuraGameplayTags::Get().Damage_Physical))
	{
		DetermineDeBuff(ExecutionParams, Spec, EvaluateParameters, FAuraGameplayTags::Get().Damage_Physical, FAuraGameplayTags::Get().Attributes_Resistance_Physical, DamageStatics().PhysicalResistanceDef);
		float DamageTypeValue = Spec.GetSetByCallerMagnitude(FAuraGameplayTags::Get().Damage_Physical);
		float TargetPhysicalResistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().PhysicalResistanceDef, EvaluateParameters, TargetPhysicalResistance);
		DamageTypeValue *= ( 100.f - TargetPhysicalResistance ) / 100.f;
		Damage += DamageTypeValue;
	}
	else if (SourceTagContainer->HasTagExact(FAuraGameplayTags::Get().Damage_Lightning))
	{
		DetermineDeBuff(ExecutionParams, Spec, EvaluateParameters, FAuraGameplayTags::Get().Damage_Lightning, FAuraGameplayTags::Get().Attributes_Resistance_Lightning, DamageStatics().LightningResistanceDef);
		float DamageTypeValue = Spec.GetSetByCallerMagnitude(FAuraGameplayTags::Get().Damage_Lightning);
		float TargetLightningResistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().LightningResistanceDef, EvaluateParameters, TargetLightningResistance);
		DamageTypeValue *= ( 100.f - TargetLightningResistance ) / 100.f;
		Damage += DamageTypeValue;
	}
	else if (SourceTagContainer->HasTagExact(FAuraGameplayTags::Get().Damage_Arcane))
	{
		DetermineDeBuff(ExecutionParams, Spec, EvaluateParameters, FAuraGameplayTags::Get().Damage_Arcane, FAuraGameplayTags::Get().Attributes_Resistance_Arcane, DamageStatics().ArcaneResistanceDef);
		float DamageTypeValue = Spec.GetSetByCallerMagnitude(FAuraGameplayTags::Get().Damage_Arcane);
		float TargetArcaneResistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArcaneResistanceDef, EvaluateParameters, TargetArcaneResistance);
		DamageTypeValue *= ( 100.f - TargetArcaneResistance ) / 100.f;

		if (UAuraAbilitySystemLibrary::IsRadialDamage(EffectContextHandle))
		{
			// 1. override TakeDamage in AuraCharacterBase. *
			// 2. create delegate OnDamageDelegate, broadcast damage received in TakeDamage *
			// 3. Bind lambda to OnDamageDelegate on the Victim here. *
			// 4. Call UGameplayStatics::ApplyRadialDamageWithFalloff to cause damage (this will result in TakeDamage being called
			//		on the Victim, which will then broadcast OnDamageDelegate)
			// 5. In Lambda, set DamageTypeValue to the damage received from the broadcast *
			if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(TargetAvatar))
			{
				CombatInterface->GetOnDamageSignature().AddLambda([&](float DamageAmount)
				{
					DamageTypeValue = DamageAmount;
				});
			}
			UGameplayStatics::ApplyRadialDamageWithFalloff(
				TargetAvatar,
				DamageTypeValue,
				0.f,
				UAuraAbilitySystemLibrary::GetRadialDamageOrigin(EffectContextHandle),
				UAuraAbilitySystemLibrary::GetRadialDamageInnerRadius(EffectContextHandle),
				UAuraAbilitySystemLibrary::GetRadialDamageOuterRadius(EffectContextHandle),
				1.f,
				UDamageType::StaticClass(),
				TArray<AActor*>(),
				SourceAvatar,
				nullptr);
			Damage += DamageTypeValue;
		}
	}

	const UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	
	
	// Armor Penetration Coefficient
	const FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"), FString());
	const float SourceArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourcePlayerLevel);

	// Armor Coefficient
	const FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmor"), FString());
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetPlayerLevel);

	const FRealCurve* CriticalHitResistanceCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("CriticalHitResistance"), FString());
	const float CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval(TargetPlayerLevel);

	//  Block chance
	float BlockTargetChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluateParameters, BlockTargetChance);
	BlockTargetChance = FMath::Max<float>(BlockTargetChance, 0.f);
	const bool bBlock = FMath::RandRange(1, 100) < BlockTargetChance;
	Damage = bBlock? Damage / 2.0 : Damage;

	// set Aura effect bBlock
	UAuraAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle, bBlock);

	// get target armor
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluateParameters, TargetArmor);
	TargetArmor = FMath::Max<float>(TargetArmor, 0.f);

	//get source armor penetration
	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluateParameters, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(SourceArmorPenetration, 0.f);

	//get criticalHit damage
	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluateParameters, SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(SourceCriticalHitDamage, 0.f);

	//get criticalHit resistance
	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef, EvaluateParameters, TargetCriticalHitResistance);
	TargetCriticalHitResistance= FMath::Max<float>(TargetCriticalHitResistance, 0.f);

	//get CriticalHit Chance
	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluateParameters, SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(SourceArmorPenetration, 0.f);
	
	
	// ArmorPenetration ignores a percentage of the Target's Armor.	
	const float EffectiveArmor = TargetArmor * ( 100 - SourceArmorPenetration * SourceArmorPenetrationCoefficient ) / 100.f;

	// Armor ignores a percentage of incoming Damage.
	Damage *= ( 100 - EffectiveArmor * EffectiveArmorCoefficient ) / 100.f;

	// Critical Hit Resistance reduces Critical Hit Chance by a certain percentage
	const float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResistanceCoefficient;
	const bool bCriticalHit = FMath::RandRange(1, 100) < EffectiveCriticalHitChance;

	// Aura game Effect is critical hit
	UAuraAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCriticalHit);
	
	// Double damage plus a bonus if critical hit
	Damage = bCriticalHit ? 2.f * Damage + SourceCriticalHitDamage : Damage;
	
	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}

void UExecCalc_Damage::DetermineDeBuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	const FGameplayEffectSpec& Spec, const FAggregatorEvaluateParameters& EvaluationParameters, const FGameplayTag DamageType, const FGameplayTag& ResistanceTag, const FGameplayEffectAttributeCaptureDefinition& InCaptureDef) const
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	const float TypeDamage = Spec.GetSetByCallerMagnitude(DamageType, false, -1.f);
	if (TypeDamage > -.5f) // .5 padding for floating point [im]precision
	{
		// Determine if there was a successful deBuff
		const float SourceDeBuffChance = Spec.GetSetByCallerMagnitude(GameplayTags.DeBuff_Chance, false, -1.f);

		float TargetDeBuffResistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(InCaptureDef, EvaluationParameters,
		                                                           TargetDeBuffResistance);
		TargetDeBuffResistance = FMath::Max<float>(TargetDeBuffResistance, 0.f);
		const float EffectiveDeBuffChance = SourceDeBuffChance * (100 - TargetDeBuffResistance) / 100.f;
		const bool bDeBuff = FMath::RandRange(1, 100) < EffectiveDeBuffChance;
		if (bDeBuff)
		{
			FGameplayEffectContextHandle ContextHandle = Spec.GetContext();

			UAuraAbilitySystemLibrary::SetIsSuccessfulDeBuff(ContextHandle, true);
			UAuraAbilitySystemLibrary::SetDamageType(ContextHandle, DamageType);

			const float DeBuffDamage = Spec.GetSetByCallerMagnitude(GameplayTags.DeBuff_Damage, false, -1.f);
			const float DeBuffDuration = Spec.GetSetByCallerMagnitude(GameplayTags.DeBuff_Duration, false, -1.f);
			const float DeBuffFrequency = Spec.GetSetByCallerMagnitude(GameplayTags.DeBuff_Frequency, false, -1.f);

			UAuraAbilitySystemLibrary::SetDeBuffDamage(ContextHandle, DeBuffDamage);
			UAuraAbilitySystemLibrary::SetDeBuffDuration(ContextHandle, DeBuffDuration);
			UAuraAbilitySystemLibrary::SetDeBuffFrequency(ContextHandle, DeBuffFrequency);
		}
	}
}
