// I made this :)


#include "AbilitySystem/Aura_AbilitySystemComponent.h"

#include "AbilitySystem/Ability/AuraGameplayAbility.h"

void UAura_AbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAura_AbilitySystemComponent::ClientEffectApplied);
}

void UAura_AbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities)
{
	for (auto& Ability : Abilities)
	{
		FGameplayAbilitySpec AbilitySpec{Ability, 1};
		if (UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StartupInputTag);
		}
		GiveAbility(AbilitySpec);
	}
}

void UAura_AbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid() == false)
		return;
	
	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTag(InputTag) == false)
			continue;
		
		AbilitySpecInputPressed(AbilitySpec);
		
		if (AbilitySpec.IsActive())
			continue;
		
		TryActivateAbility(AbilitySpec.Handle);
	}
}

void UAura_AbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid() == false)
		return;
	
	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTag(InputTag) == false)
			continue;
		
		AbilitySpecInputReleased(AbilitySpec);
	}
}

void UAura_AbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	
}

// The Implementation of an RPC refers to the actual execution that occurs on the recipient's end.
// This is opposed to the _Verify function that may also be implemented.
void UAura_AbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
	const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffect)
{
	FGameplayTagContainer OutTags;
	EffectSpec.GetAllAssetTags(OutTags);
	
	EffectAssetTags.Broadcast(OutTags);
}