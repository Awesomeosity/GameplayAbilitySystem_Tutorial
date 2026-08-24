// I made this :)

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouseTask.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRetrievedDataSignature, const FGameplayAbilityTargetDataHandle&, Data);

/**
 * Retrieves some data under the mouse cursor at time of execution.
 */
UCLASS()
class AURA_API UTargetDataUnderMouseTask : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta=(HidePin = "OwningAbility", DefaultToSelf="OwningAbility", BlueprintInternalUseOnly, DisplayName = "TargetDataUnderMouse"))
	static UTargetDataUnderMouseTask* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility);
	
	UPROPERTY(BlueprintAssignable)
	FRetrievedDataSignature MouseLocationData;
	
private:
	virtual void Activate() override;
	
	void SendMouseCursorData();
	
	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag);
};
