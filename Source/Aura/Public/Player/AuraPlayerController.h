// I made this :)

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class USplineComponent;
class UAuraInputConfig;
class UInputMappingContext;
class UInputAction;
class UAura_AbilitySystemComponent;
class IHighlightInterface;

struct FInputActionValue;

/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AAuraPlayerController();

	virtual void PlayerTick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	void SetupInputComponent() override;

private:
	void Move(const FInputActionValue& InputActionValue);
	
	void CursorTrace();

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UAuraInputConfig> InputConfig;
	
	UAura_AbilitySystemComponent* GetAbilitySystemComponent();
	
	UPROPERTY()
	TObjectPtr<UAura_AbilitySystemComponent> AbilitySystemComponent;

	TScriptInterface<IHighlightInterface> HighlightedActor;
	
	FVector CachedDestination = FVector::ZeroVector;
	float MouseHoldTime = 0.f;
	bool bIsAutoRunning = false;
	bool bIsTargeting = false;
	
	UPROPERTY(EditAnywhere)
	float MinPressTime = 0.5f;
	
	UPROPERTY(EditAnywhere)
	float AutoRunDistanceThreshold = 50.f;
	
	FHitResult CachedHitResult;
	
	UPROPERTY()
	TObjectPtr<USplineComponent> SplineComp;
	
};
