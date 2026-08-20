// I made this :)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "AuraPlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

/**
 * A Player State manages everything about the player that isn't encapsulated by the Controller or Actor itself.
 */
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AAuraPlayerState();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;


	//IAbilitySystemInterface functions
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//IAbilitySystemInterface END

	UAttributeSet* GetAttributeSet() const { return AttributeSet; };
	
	int32 GetPlayerLevel() const { return Level; };
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComp;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_PlayerLevel)
	int32 Level = 1;
	
	UFUNCTION()
	void OnRep_PlayerLevel(int32 OldLevel);
};
