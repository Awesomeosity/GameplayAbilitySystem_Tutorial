// I made this :)

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameFramework/Actor.h"
#include "AuraEffectActor.generated.h"

class UGameplayEffect;

/**
 * This applies a given effect to an actor, whose trigger conditions can be defined in Blueprints.
 */
UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAuraEffectActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bDestroyOnEffectRemoval = false;
	
	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor);
	
	UFUNCTION(BlueprintCallable)
	void RemoveEffectFromTarget(AActor* TargetActor, int32 StackCount = -1);
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UGameplayEffect> AppliedEffectOnOverlap;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Level = 1.f;
	
	UPROPERTY()
	TMap<UAbilitySystemComponent*, FActiveGameplayEffectHandle> SavedHandles;
};
