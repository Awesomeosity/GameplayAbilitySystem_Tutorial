// I made this :)


#include "AbilitySystem/Ability/AuraProjectileSpell.h"

#include "Actor/AuraProjectile.h"
#include "Interfaces/CombatInterface.h"

void  UAuraProjectileSpell::FireProjectile(const FVector& ProjectileTargetLocation)
{
	auto ActivationInfo = GetCurrentActivationInfo();
	bool bHasAuthority = HasAuthority(&ActivationInfo);
	if (bHasAuthority == false)
		return;
	
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	FVector SocketLocation = FVector::ZeroVector;
	
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	// Removes up/down angles
	Rotation.Pitch = 0.f;
	
	if (CombatInterface)
	{
		SocketLocation = CombatInterface->GetWeaponSocketLocation();
	}
	
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SocketLocation);
	SpawnTransform.SetRotation(Rotation.Quaternion());
	
	AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass,
		SpawnTransform, 
		GetOwningActorFromActorInfo(), 
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);
	
	// TODO: Give the Projectile a gameplay effect spec for causing dmg.
	
	Projectile->FinishSpawning(SpawnTransform);
	return;
}

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}
