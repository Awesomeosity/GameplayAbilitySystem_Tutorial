// I made this :)


#include "AbilityTask/TargetDataUnderMouseTask.h"


UTargetDataUnderMouseTask* UTargetDataUnderMouseTask::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouseTask* MyObj = NewAbilityTask<UTargetDataUnderMouseTask>(OwningAbility);
	return MyObj;
}

void UTargetDataUnderMouseTask::Activate()
{
	FHitResult HitResult;
	auto PlayerController = Ability->GetCurrentActorInfo()->PlayerController;
	if (PlayerController.IsValid() == false)
	{
		UE_LOG(LogTemp, Error, TEXT("[%hs] PlayerController was invalid!"), __FUNCTION__);
		return;
	}
	
	PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);
	MouseLocationData.Broadcast(HitResult.Location);
}
