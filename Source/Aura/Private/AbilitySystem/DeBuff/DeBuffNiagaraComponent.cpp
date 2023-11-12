// Copyright Druid Mechanics


#include "AbilitySystem/DeBuff/DeBuffNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Iteraction/CombatInterface.h"

UDeBuffNiagaraComponent::UDeBuffNiagaraComponent()
{
	bAutoActivate = false;
}

void UDeBuffNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();

	
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner());
	if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner()))
	{
		ASC->RegisterGameplayTagEvent(DeBuffTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UDeBuffNiagaraComponent::DeBuffTagChanged);
	}
	else if (CombatInterface)
	{
		CombatInterface->GetOnASCRegisteredDelegate().AddWeakLambda(this, [this](UAbilitySystemComponent* InASC)
		{
			InASC->RegisterGameplayTagEvent(DeBuffTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UDeBuffNiagaraComponent::DeBuffTagChanged);
		});
	}
}

void UDeBuffNiagaraComponent::DeBuffTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	const bool bOwnerValid = IsValid(GetOwner());
	const bool bOwnerAlive = GetOwner()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(GetOwner());
	if (NewCount > 0 && bOwnerValid && bOwnerAlive)
	{
		Activate();
	}
	else
	{
		Deactivate();
	}
}
