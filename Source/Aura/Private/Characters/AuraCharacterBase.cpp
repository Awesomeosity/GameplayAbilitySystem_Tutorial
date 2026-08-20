// I made this :)


#include "Characters/AuraCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/Aura_AbilitySystemComponent.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AAuraCharacterBase::AAuraCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon Mesh");
	WeaponMesh->SetupAttachment(GetMesh(), TEXT("WeaponHandSocket"));

	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}

int32 AAuraCharacterBase::GetPlayerLevel() const
{
	return ICombatInterface::GetPlayerLevel();
}

FVector AAuraCharacterBase::GetWeaponSocketLocation() const
{
	if (WeaponMesh == nullptr)
		return FVector{};
	
	return WeaponMesh->GetSocketLocation(WeaponTipSocketName);
}

void AAuraCharacterBase::SetUpAbilitySystemComp()
{
}

void AAuraCharacterBase::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect> EffectToApply, const float Level /*= 1.f*/) const
{
	if (EffectToApply == nullptr)
		return;
	
	if (AbilitySystemComp == nullptr)
		return;
	
	auto EffectContext = AbilitySystemComp->MakeEffectContext();
	EffectContext.AddSourceObject(this);
	const auto InitialGameplayEffectSpec = AbilitySystemComp->MakeOutgoingSpec(EffectToApply, Level, EffectContext);
	AbilitySystemComp->ApplyGameplayEffectSpecToSelf(*InitialGameplayEffectSpec.Data.Get());
}

void AAuraCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes);
	ApplyEffectToSelf(DefaultSecondaryAttributes);
	ApplyEffectToSelf(InitialVitalAttributes);
}

void AAuraCharacterBase::AddCharacterAbilities()
{
	if (HasAuthority() == false)
		return;
	
	if (AbilitySystemComp == nullptr)
		return;
	
	auto* AuraAbilitySystemComp = Cast<UAura_AbilitySystemComponent>(AbilitySystemComp);
	if (AuraAbilitySystemComp == nullptr)
		return;
	
	AuraAbilitySystemComp->AddCharacterAbilities(StartingAbilities);
}
