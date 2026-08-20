// I made this :)

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AuraWidgetController.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
class APlayerState;
class APlayerController;

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {};
	FWidgetControllerParams(
		UAttributeSet* InAttributeSet,
		UAbilitySystemComponent* InAbilitySystemComponent,
		APlayerState* InPlayerState,
		APlayerController* InPlayerController
	) : AttributeSet(InAttributeSet), AbilitySystemComp(InAbilitySystemComponent), PlayerState(InPlayerState), PlayerController(InPlayerController)
	{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget Controller")
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget Controller")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComp = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget Controller")
	TObjectPtr<APlayerState> PlayerState = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget Controller")
	TObjectPtr<APlayerController> PlayerController = nullptr;

};

/**
 * Base Controller for widgets, connecting any updates to stats on the backend to the frontend.
 */
UCLASS(Blueprintable)
class AURA_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()
	
public:
	UAuraWidgetController();

	UFUNCTION(BlueprintCallable)
	void InitWidgetController(const FWidgetControllerParams& Params);

	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues();

	virtual void BindCallbacksToDependencies();

protected:
	
	UPROPERTY(BlueprintReadOnly, Category = "Widget Controller")
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widget Controller")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComp = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widget Controller")
	TObjectPtr<APlayerState> PlayerState = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widget Controller")
	TObjectPtr<APlayerController> PlayerController = nullptr;
};
