// I made this :)


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag,
	bool bLogNotFound /*= false*/) const
{
	for (auto& Action : Actions)
	{
		if (Action.InputTag.MatchesTagExact(InputTag) && Action.InputAction != nullptr)
			return Action.InputAction;
	}
	
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("[%hs] Failed to find the action related to tag %s"), __FUNCTION__, *InputTag.ToString());
	}
	
	return nullptr;
}
