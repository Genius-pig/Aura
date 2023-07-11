// Copyright Druid Mechanics


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}


void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	if (TargetActor->ActorHasTag(FName("Enemy")) && !bApplyEffectsToEnemies) return;
	UAbilitySystemComponent* TargetASC =  UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if(TargetASC == nullptr) return;

	check(GameplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle);
	TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	FGameplayTagContainer Container;
	EffectSpecHandle.Data->GetAllAssetTags(Container);

	const bool bIsFireAreaEffect = Container.HasTagExact(FAuraGameplayTags::Get().Effects_FireArea);

	if(!bIsFireAreaEffect)
	{
		Destroy();
	}
}

void AAuraEffectActor::OnAreaEndOverlap(AActor* TargetActor)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	
	if(!IsValid(TargetASC)) return;
	FGameplayTagContainer Container;
	Container.AddTag(FAuraGameplayTags::Get().Effects_FireArea);
	TargetASC->RemoveActiveEffectsWithTags(Container);
}



