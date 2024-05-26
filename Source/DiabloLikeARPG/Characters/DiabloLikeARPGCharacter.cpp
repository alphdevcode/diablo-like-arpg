// Copyright 2024 AlphDevCode. All Rights Reserved.

#include "DiabloLikeARPGCharacter.h"

#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "DiabloLikeARPG/DiabloLikeARPGPlayerController.h"
#include "DiabloLikeARPG/StatsComponent.h"
#include "DiabloLikeARPG/AbilitiesSystem/AbilitiesComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

ADiabloLikeARPGCharacter::ADiabloLikeARPGCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Attach Stat Component
	StatsComponent = CreateDefaultSubobject<UStatsComponent>(TEXT("StatsComponent"));

	// Attach Abilities Component
	AbilitiesComponent = CreateDefaultSubobject<UAbilitiesComponent>(TEXT("AbilitiesComponent"));

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ADiabloLikeARPGCharacter::BeginPlay()
{
	Super::BeginPlay();

	const FTimerDelegate CheckForInteractionsTimerDelegate =
		FTimerDelegate::CreateUObject(this, &ADiabloLikeARPGCharacter::CheckForInteractions);

	GetWorldTimerManager().SetTimer(InteractionTimerHandle, CheckForInteractionsTimerDelegate, .05f, true);
}

void ADiabloLikeARPGCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

float ADiabloLikeARPGCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator,
                                           AActor* DamageCauser)
{
	const float DamageToApply = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	// DamageToApply = FMath::Min(Health, DamageToApply);

	if (HitAnimMontage != nullptr)
	{
		GetMesh()->GetAnimInstance()->Montage_Play(HitAnimMontage, 1.f,
		                                           EMontagePlayReturnType::MontageLength, 0.f, false);
	}
	StatsComponent->ReduceHealth(DamageToApply);

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,
		                                 FString::Printf(
			                                 TEXT("Taking Damage. Health: %f"), StatsComponent->GetHealth()));

	if (IsDead())
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		DetachFromControllerPendingDestroy();

		// UnPossessed();

		const FName PelvisBone = "pelvis";
		GetMesh()->SetAllBodiesBelowSimulatePhysics(PelvisBone, true, true);
		GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(PelvisBone,
		                                               1.f, false, true);

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,
			                                 TEXT("Dead"));

		// Destroy the character after 2 seconds
		GetWorldTimerManager().SetTimer(DestroyActorTimerHandle, this,
		                                &ADiabloLikeARPGCharacter::DestroyCharacter, 2.f, false);
	}

	return DamageToApply;
}

bool ADiabloLikeARPGCharacter::IsDead() const
{
	return StatsComponent->GetHealth() <= 0;
}

void ADiabloLikeARPGCharacter::CheckForInteractions()
{
	if (CurrentInteractable == nullptr) return;

	const float DistanceToTarget = GetDistanceTo(CurrentInteractable->GetInteractableActor());

	// if (GEngine)
	// 	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green,
	// 	FString::Printf(TEXT("Distance: %f"), DistanceToTarget));

	if (DistanceToTarget <= GetInteractionRange())
	{
		GetController()->StopMovement();
		CurrentInteractable->Interact(this);

		CurrentInteractable = nullptr;
	}
	else
	{
		if (ADiabloLikeARPGPlayerController* PlayerController = Cast<ADiabloLikeARPGPlayerController>(GetController()))
		{
			PlayerController->ContinuouslyMoveToLocation(
				CurrentInteractable->GetInteractableActor()->GetActorLocation());
		}
	}
}

void ADiabloLikeARPGCharacter::DestroyCharacter()
{
	const FVector MeshLocation = GetMesh()->GetBoneLocation("pelvis");
	if (DestroyFX != nullptr)
	{
		// play particle system
		UGameplayStatics::SpawnEmitterAtLocation(this,
		                                         DestroyFX,
		                                         MeshLocation);
	}

	if (DestroySound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DestroySound,
		                                      MeshLocation);
	}


	Destroy();
	DestroyActorTimerHandle.Invalidate();
}

void ADiabloLikeARPGCharacter::SetTargetInteractable(IInteractableInterface* Interactable)
{
	if (Interactable != nullptr)
	{
		if (TargetInteractable != Interactable)
		{
			TargetInteractable = Interactable;
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue,
				                                 FString::Printf(TEXT("Updating Target Interactable from %s to %s"),
				                                                 (TargetInteractable != nullptr
					                                                  ? *TargetInteractable->GetInteractableActor()->
					                                                  GetName()
					                                                  : TEXT("null")),
				                                                 (Interactable != nullptr
					                                                  ? *Interactable->GetInteractableActor()->
					                                                  GetName()
					                                                  : TEXT("null"))));
			}
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,
			                                 TEXT("Can not set InteractableTarget. Interactable is null"));
		}
	}
}

void ADiabloLikeARPGCharacter::SetCurrentInteractable(IInteractableInterface* Interactable)
{
	CurrentInteractable = Interactable;
}

float ADiabloLikeARPGCharacter::GetInteractionRange()
{
	// TODO: Implement a way to get the interaction range from the Active Ability
	return 200.f;
}

void ADiabloLikeARPGCharacter::ActivatePrimaryAttackAbility() const
{
	AbilitiesComponent->ActivatePrimaryAttackAbility();
}

AActor* ADiabloLikeARPGCharacter::GetTargetInteractableActor() const
{
	if (TargetInteractable == nullptr)
	{
		return nullptr;
	}
	return TargetInteractable->GetInteractableActor();
}
