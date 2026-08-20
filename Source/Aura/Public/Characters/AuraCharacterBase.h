// I made this :)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interfaces/CombatInterface.h"

#include "AuraCharacterBase.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class USkeletalMeshComponent;
class UAbilitySystemComponent;
class UAttributeSet;

/**
 * Base character class.
 */
UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAuraCharacterBase();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// IAbilitySystemInterface functions
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// IAbilitySystemInterface END
	
	// ICombatInterface functions
	virtual int32 GetPlayerLevel() const override;
	virtual FVector GetWeaponSocketLocation() const override;
	// ICombatInterface END
	
protected:

	UAttributeSet* GetAttributeSet() const { return PrimaryAttributeSet; };
	
	virtual void SetUpAbilitySystemComp();
	
	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect> EffectToApply, const float Level = 1.f) const;
	virtual void InitializeDefaultAttributes() const;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComp;

	UPROPERTY()
	TObjectPtr<UAttributeSet> PrimaryAttributeSet;
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> SecondaryAttributeSet;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UGameplayEffect> InitialVitalAttributes;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Weapon")
	FName WeaponTipSocketName;
	
	UFUNCTION()
	void AddCharacterAbilities();
	
private:
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartingAbilities;
};
