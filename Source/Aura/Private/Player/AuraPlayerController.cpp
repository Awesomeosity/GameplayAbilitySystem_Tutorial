// I made this :)


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "KnownGameplayTags.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/Aura_AbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interfaces/HighlightInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
	
	SplineComp = CreateDefaultSubobject<USplineComponent>("MovementSpline");
	SplineComp->SetupAttachment(GetRootComponent());
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
	
	if (bIsAutoRunning)
	{
		if (SplineComp == nullptr)
			return;
		auto PlayerPawn = GetPawn();
		if (PlayerPawn == nullptr)
			return;
		
		const auto SplineLocation = SplineComp->FindLocationClosestToWorldLocation(PlayerPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const auto SplineMovementVector = SplineComp->FindDirectionClosestToWorldLocation(SplineLocation, ESplineCoordinateSpace::World);
		PlayerPawn->AddMovementInput(SplineMovementVector);
		
		if ((CachedDestination - SplineLocation).SquaredLength() < FMath::Square(AutoRunDistanceThreshold))
		{
			bIsAutoRunning = false;
		}
	}
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);

}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	auto* AuraInputComponent = Cast<UAuraInputComponent>(InputComponent);
	if (AuraInputComponent == nullptr)
		return;
	
	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	
	AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D AxisValue = InputActionValue.Get<FVector2D>();
	FRotator Rotation{ 0.f, GetControlRotation().Yaw, 0.f };
	FVector ForwardDir = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);
	FVector RightDir = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);

	auto* ControlledPawn = GetPawn<APawn>();
	if (ControlledPawn == nullptr)
		return;

	ControlledPawn->AddMovementInput(ForwardDir, AxisValue.Y);
	ControlledPawn->AddMovementInput(RightDir, AxisValue.X);
}

void AAuraPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, CachedHitResult);
	if (CachedHitResult.bBlockingHit == false)
		return;

	auto* HitActor = CachedHitResult.GetActor();
	if (HitActor == nullptr && HighlightedActor != nullptr)
	{
		HighlightedActor->HighlightActor(false);
		HighlightedActor = nullptr;
	}
	if (HitActor != nullptr && HighlightedActor != HitActor)
	{
		if (HighlightedActor != nullptr)
			HighlightedActor->HighlightActor(false);
	
		if (HitActor->Implements<UHighlightInterface>())
		{
			HighlightedActor = HitActor;
			HighlightedActor->HighlightActor(true);
		}
		else
		{
			HighlightedActor = nullptr;
		}
	}
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	auto* AbilitySystemComp = GetAbilitySystemComponent();
	if (AbilitySystemComponent == nullptr)
		return;
	
	if (InputTag == FKnownGameplayTags::Get().Input_LMB)
	{
		bIsTargeting = HighlightedActor != nullptr;
		bIsAutoRunning = false;
	}
	
	AbilitySystemComp->AbilityInputTagPressed(InputTag);
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	auto* AbilitySystemComp = GetAbilitySystemComponent();
	if (AbilitySystemComponent == nullptr)
		return;
	
	if (InputTag == FKnownGameplayTags::Get().Input_LMB)
	{
		if (bIsTargeting)
		{
			AbilitySystemComp->AbilityInputTagReleased(InputTag);
		}
		else
		{
			if (MouseHoldTime < MinPressTime)
			{
				UE_VLOG_SPHERE(this, LogTemp, Log, GetPawn()->GetActorLocation(), 5.f, FColor::Green, TEXT("Start Point"));
				UE_VLOG_SPHERE(this, LogTemp, Log, CachedDestination, 5.f, FColor::Cyan, TEXT("End Point"));

				auto* Path = UNavigationSystemV1::FindPathToLocationSynchronously(
					this,
					GetPawn()->GetActorLocation(),
					CachedDestination);
				if (Path == nullptr)
					return;
				
				SplineComp->ClearSplinePoints();
				
				for (auto& PathPoint : Path->PathPoints)
				{
					SplineComp->AddSplinePoint(PathPoint, ESplineCoordinateSpace::World);
					UE_VLOG_SPHERE(this, LogTemp, Log, PathPoint, 5.f, FColor::Red, TEXT("Path Point"));
				}
				
				CachedDestination = Path->PathPoints.Last();
				
				MouseHoldTime = 0.f;
				bIsAutoRunning = true;
			}
		}
	}

	AbilitySystemComp->AbilityInputTagReleased(InputTag);
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	auto* AbilitySystemComp = GetAbilitySystemComponent();
	if (AbilitySystemComponent == nullptr)
		return;
	
	if (InputTag == FKnownGameplayTags::Get().Input_LMB)
	{
		if (bIsTargeting)
		{
			AbilitySystemComp->AbilityInputTagHeld(InputTag);
		}
		else
		{
			MouseHoldTime += GetWorld()->GetDeltaSeconds();
			
			if (CachedHitResult.bBlockingHit)
			{
				CachedDestination = CachedHitResult.ImpactPoint;
			}
			
			auto* AuraPawn = GetPawn<APawn>();
			if (AuraPawn == nullptr)
				return;
			
			auto MoveDirection = CachedDestination - AuraPawn->GetActorLocation();
			AuraPawn->AddMovementInput(MoveDirection);
		}
		
	}
	else
	{
		AbilitySystemComp->AbilityInputTagHeld(InputTag);
	}
}

UAura_AbilitySystemComponent* AAuraPlayerController::GetAbilitySystemComponent()
{
	if (AbilitySystemComponent == nullptr)
	{
		AbilitySystemComponent = Cast<UAura_AbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	
	return AbilitySystemComponent;
}
