// Copyright Druid Mechanics


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/AuraProjectile.h"
#include "Iteraction/CombatInterface.h"
#include "AuraGameplayTags.h"


void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if(!bIsServer) return;
	FVector Location = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo());
	FRotator Rotation = (ProjectileTargetLocation - Location).Rotation();
	Rotation.Pitch = 0.f;
	FTransform SpawnTransform;
	Location = Location + FVector(0.f, 0.f, -100.0f);
	SpawnTransform.SetRotation(Rotation.Quaternion());
	SpawnTransform.SetLocation(Location);
	AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);


	const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(
		GetAvatarActorFromActorInfo());

	FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
	EffectContextHandle.SetAbility(this);
	EffectContextHandle.AddSourceObject(Projectile);
	TArray<TWeakObjectPtr<AActor>> Actors;
	EffectContextHandle.AddActors(Actors);
	FHitResult HitResult;
	HitResult.Location = ProjectileTargetLocation;
	EffectContextHandle.AddHitResult(HitResult);

	const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(
		DamageEffectClass, GetAbilityLevel(), EffectContextHandle);
	FGameplayEffectSpec* GameplayEffectSpec = SpecHandle.Data.Get();
	FGameplayAbilitySpec* AbilitySpecHandle = GetActorInfo().AbilitySystemComponent->FindAbilitySpecFromHandle(
		GetCurrentAbilitySpecHandle());
	ApplyAbilityTagsToGameplayEffectSpec(*GameplayEffectSpec, AbilitySpecHandle);
	// in source code, it was be stored in a map. TMap<FGameplayTag, float>	SetByCallerTagMagnitudes.

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, FAuraGameplayTags::Get().Damage_Fire,
	                                                              Damage.GetValueAtLevel(GetAbilityLevel()));

	Projectile->DamageEffectSpecHandle = SpecHandle;
	Projectile->FinishSpawning(SpawnTransform);
}


