// I made this :)


#include "AbilityTask/TargetDataUnderMouseTask.h"

#include "AbilitySystemComponent.h"


UTargetDataUnderMouseTask* UTargetDataUnderMouseTask::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouseTask* MyObj = NewAbilityTask<UTargetDataUnderMouseTask>(OwningAbility);
	return MyObj;
}

void UTargetDataUnderMouseTask::Activate()
{
	bool bIsLocallyControlled = Ability->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		SendMouseCursorData();
	}
	else
	{
		const auto SpecHandle = GetAbilitySpecHandle();
		const auto ActivationPredictionKey = GetActivationPredictionKey();
		// Called if we have yet to receive the target data.
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).AddUObject(this, &UTargetDataUnderMouseTask::OnTargetDataReplicatedCallback);
		// Called if we have already received the target data.
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationPredictionKey);
		
		// Now if we're waiting on some player data that is incoming, make us wait for it.
		if (bCalledDelegate == false)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UTargetDataUnderMouseTask::SendMouseCursorData()
{
	FHitResult HitResult;
	auto PlayerController = Ability->GetCurrentActorInfo()->PlayerController;
	if (PlayerController.IsValid() == false)
	{
		UE_LOG(LogTemp, Error, TEXT("[%hs] PlayerController was invalid!"), __FUNCTION__);
		return;
	}
	
	PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);
	
	FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(HitResult);
	FGameplayAbilityTargetDataHandle DataHandle;
	
	DataHandle.Add(TargetData);
	AbilitySystemComponent->ServerSetReplicatedTargetData(GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey
	);
	
	// This needs to be checked before broadcasting things back to the Ability Graph.
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		MouseLocationData.Broadcast(DataHandle);
	}
	
}

void UTargetDataUnderMouseTask::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,
	FGameplayTag ActivationTag)
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		MouseLocationData.Broadcast(DataHandle);
	}

}
