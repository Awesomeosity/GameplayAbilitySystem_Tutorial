// I made this :)

#pragma once

#include "CoreMinimal.h"
#include "Characters/AuraCharacterBase.h"
#include "AuraPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

/**
 * The main player character.
 */
UCLASS()
class AURA_API AAuraPlayerCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()

public:
	AAuraPlayerCharacter();

	virtual void BeginPlay() override;
	
	virtual void PossessedBy(AController* NewController) override;
	
	// ICombatInterface functions
	virtual int32 GetPlayerLevel() const override;
	// ICombatInterface END

protected:
	virtual void OnRep_PlayerState() override;

	virtual void SetUpAbilitySystemComp() override;

	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UCameraComponent> MainCamera;
	
};
