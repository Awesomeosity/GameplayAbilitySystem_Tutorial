// I made this :)

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayAbility.h"
#include "AuraProjectileSpell.generated.h"

class AAuraProjectile;
/**
 * Base Gameplay Ability that shoots a projectile of a given projectile class.
 */
UCLASS()
class AURA_API UAuraProjectileSpell : public UAuraGameplayAbility
{
	GENERATED_BODY()
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	                             const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData
	) override;
	
	// Launches a projectile specifically from a given Socket's location on a mesh.
	// From the set location, creates it in the world where it can handle its own movement.
	UFUNCTION(BlueprintCallable)
	virtual void FireProjectile();
	
	// FireProjectile uses this defined projectile to launch.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AAuraProjectile> ProjectileClass;
};
