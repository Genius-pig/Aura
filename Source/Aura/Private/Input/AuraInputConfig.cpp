// Copyright Druid Mechanics


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound)
{
	for(auto Action : AbilityInputActions)
	{
		if(Action.InputTag.MatchesTagExact(InputTag) && Action.InputAction)
		{
			return Action.InputAction;
		}
	}

	if(bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("can't find AbilityInputAction for InputTag[%s], On InputConfig[%s]"), *InputTag.ToString(), *GetNameSafe(this))
	}

	return nullptr;
}
