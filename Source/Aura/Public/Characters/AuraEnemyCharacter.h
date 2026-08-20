// I made this :)

#pragma once

#include "CoreMinimal.h"
#include "Characters/AuraCharacterBase.h"
#include "Interfaces/HighlightInterface.h"
#include "AuraEnemyCharacter.generated.h"

/**
 * Enemy characters 
 */
UCLASS()
class AURA_API AAuraEnemyCharacter : public AAuraCharacterBase, public IHighlightInterface
{
	GENERATED_BODY()

	
public:
	AAuraEnemyCharacter();

	virtual void BeginPlay() override;

	// IHighlightInterface functions
	virtual void HighlightActor(bool bShouldHighlight) override;
	// IHighlightInterface functions end
	
	// ICombatInterface functions
	virtual int32 GetPlayerLevel() const override;
	// ICombatInterface END

protected:
	virtual void SetUpAbilitySystemComp() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;
};
