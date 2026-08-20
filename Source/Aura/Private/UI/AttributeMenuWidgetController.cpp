// I made this :)


#include "UI/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	auto* AuraAttributes = Cast<UAuraAttributeSet>(AttributeSet);
	if (AuraAttributes == nullptr)
		return;
	
	for (auto& Data : AttributeDataAsset->AttributeInfos)
	{
		Data.AttributeValue = AuraAttributes->TagsToAttributes[Data.AttributeTag]().GetNumericValue(AuraAttributes);
		OnAttributeChanged.Broadcast(Data);
	}
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	auto* AuraAttributes = Cast<UAuraAttributeSet>(AttributeSet);
	if (AuraAttributes == nullptr)
		return;

	for (auto& Pair : AuraAttributes->TagsToAttributes)
	{
		AbilitySystemComp->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda([this, Pair, AuraAttributes](const FOnAttributeChangeData& Data)
		{
			auto AttributeInfo = AttributeDataAsset->GetAttributeInfo(Pair.Key);
			AttributeInfo.AttributeValue = Pair.Value().GetNumericValue(AuraAttributes);
			OnAttributeChanged.Broadcast(AttributeInfo);
		});
	}

}
