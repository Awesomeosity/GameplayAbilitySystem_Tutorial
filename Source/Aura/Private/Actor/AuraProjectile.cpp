// I made this :)


#include "Actor/AuraProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AAuraProjectile::AAuraProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	bReplicates = true;
	
	SphereHitbox = CreateDefaultSubobject<USphereComponent>(TEXT("SphereHitbox"));
	SetRootComponent(SphereHitbox);
	SphereHitbox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereHitbox->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereHitbox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	SphereHitbox->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	SphereHitbox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 550.f;
	ProjectileMovement->MaxSpeed = 550.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

// Called when the game starts or when spawned
void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();
	SphereHitbox->OnComponentBeginOverlap.AddDynamic(this, &AAuraProjectile::OnSphereOverlap);
}

void AAuraProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}
