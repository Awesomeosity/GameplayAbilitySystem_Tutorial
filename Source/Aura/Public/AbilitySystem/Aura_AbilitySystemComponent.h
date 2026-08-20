// I made this :)

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Aura_AbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer&);

/**
 * Ability System Components are typically made in C++.
 * They support a particular Actor's ability to cast Gameplay Abilities.
 * This Aura Ability System Component is the custom base implementation of the Ability System Component.
 */
UCLASS()
class AURA_API UAura_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	// Called in order to setup proper updates of attributes on a client when applied by the server.
	void AbilityActorInfoSet();
	
	// Self-explanatory, it takes a list of abilities from some source and adds those
	// Abilities to the user.
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities);
	
	// Delegate called whenever a Gameplay effect is applied.
	FEffectAssetTags EffectAssetTags;
	
	// Player-oriented input methods for handling ability casts.
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	
protected:
	// This is an RPC. RPCs, or Replicated Function Calls, calls things to happen immediately
	// from anywhere on the network. For example, this one is a Client, Reliable call.
	// Reliable RPCs always execute when broadcast in the same order that they were sent out.
	// They take more bandwidth to use, since they're continually cycled from source to recipient
	// until receipt is acknowledged.
	// "Client" RPCs execute on the owning client who's associated with the object that the RPC is
	// executed on. If called from a client, it's called on that client.
	// This function, specifically, broadcasts changes to attributes applied to the actor by the server,
	// allowing the UI to update based on which tags were bundled in the spec.
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(
		UAbilitySystemComponent* AbilitySystemComponent,
		const FGameplayEffectSpec& EffectSpec,
		FActiveGameplayEffectHandle ActiveGameplayEffect
		);
};
