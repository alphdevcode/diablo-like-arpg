// Copyright 2024 AlphDevCode. All Rights Reserved.

#include "Actors/Characters/DiabloLikeARPGCharacter.h"

#include "UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"
#include "Actors/PlayerControllers/DiabloLikeARPGPlayerController.h"
#include "Components/AbilitiesComponent.h"
#include "Components/StatsComponent.h"
#include "Actors/GameModes/SoloARPGGameMode.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Logger.h"

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

	// Currently we only check for interactions if we're a player controlled character
	if (Cast<ADiabloLikeARPGPlayerController>(GetController()))
	{
		const FTimerDelegate CheckForInteractionsTimerDelegate =
			FTimerDelegate::CreateUObject(this, &ADiabloLikeARPGCharacter::CheckForInteractions);

		GetWorldTimerManager().SetTimer(InteractionTimerHandle, CheckForInteractionsTimerDelegate, .2f, true);
	}
}

void ADiabloLikeARPGCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	DestroyActorTimerHandle.Invalidate();
	InteractionTimerHandle.Invalidate();
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

	// If we have a valid HitAnimMontage, and we're currently not playing any other animation montage
	if (const UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		HitAnimMontage != nullptr && AnimInstance != nullptr
		&& !AnimInstance->Montage_IsPlaying(nullptr))
	{
		GetMesh()->GetAnimInstance()->Montage_Play(HitAnimMontage, 1.f,
		                                           EMontagePlayReturnType::MontageLength, 0.f, false);
	}
	StatsComponent->ReduceHealth(DamageToApply);

	LOG_ERROR(TEXT("Taking Damage. Health: %f"), StatsComponent->GetHealth());

	if (IsDead())
	{
		HandleCharacterDeath();
	}

	return DamageToApply;
}

// void ADiabloLikeARPGCharacter::HandleRespawn()
// {
// 	GetMesh()->SetSimulatePhysics(false);
// 	GetMesh()->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
// }

bool ADiabloLikeARPGCharacter::IsDead() const
{
	return StatsComponent->GetHealth() <= 0;
}

void ADiabloLikeARPGCharacter::CheckForInteractions()
{
	if (CurrentInteractable == nullptr) return;

	const float DistanceToTarget = GetDistanceTo(CurrentInteractable->GetInteractableActor());

	// LOG_INFO(TEXT("Distance: %f"), DistanceToTarget);

	if (DistanceToTarget <= GetInteractionRange() && GetController())
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

void ADiabloLikeARPGCharacter::HandleCharacterDeath()
{
	AbilitiesComponent->CleanAbilities();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetMesh()->SetSimulatePhysics(true);
	// const FName PelvisBone = "pelvis";
	// GetMesh()->SetAllBodiesBelowSimulatePhysics(PelvisBone, true, true);
	// GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(PelvisBone,
	//                                                1.f, false, true);

	LOG_INFO(TEXT("%s has died!"), *GetName());

	if (const ASoloARPGGameMode* GameMode = Cast<ASoloARPGGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->GetOnPawnDied().Broadcast(this);
	}

	if (DeathSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	}

	if (!bCanRespawn)
	{
		DetachFromControllerPendingDestroy();
		UnPossessed();

		// Destroy the character after 2 seconds
		GetWorldTimerManager().SetTimer(DestroyActorTimerHandle, this,
		                                &ADiabloLikeARPGCharacter::DestroyCharacter, 2.f, false);
	}
}

void ADiabloLikeARPGCharacter::DestroyCharacter()
{
	const FVector MeshLocation = GetMesh()->GetBoneLocation("pelvis");
	if (DestroyFX != nullptr)
	{
		// play particle system
		UGameplayStatics::SpawnEmitterAtLocation(this, DestroyFX,
		                                         MeshLocation, FRotator::ZeroRotator,
		                                         FVector(1), true, EPSCPoolMethod::AutoRelease);
	}

	if (DestroySound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DestroySound,
		                                      MeshLocation);
	}

	DestroyActorTimerHandle.Invalidate();
	Destroy();
}

void ADiabloLikeARPGCharacter::SetTargetInteractable(IInteractableInterface* Interactable)
{
	if (Interactable != nullptr)
	{
		if (TargetInteractable != Interactable)
		{
			TargetInteractable = Interactable;

			LOG_INFO(TEXT("Updating Target Interactable from %s to %s"),
			         TargetInteractable != nullptr ? *TargetInteractable->GetInteractableActor()->GetName() : TEXT("null"),
			         Interactable != nullptr ? *Interactable->GetInteractableActor()->GetName() : TEXT("null"));
		}
	}
	else
	{
		LOG_ERROR(TEXT("Can not set InteractableTarget. Interactable is null"));
	}
}

void ADiabloLikeARPGCharacter::SetCurrentInteractable(IInteractableInterface* Interactable)
{
	CurrentInteractable = Interactable;
}

float ADiabloLikeARPGCharacter::GetInteractionRange()
{
	// TODO: Implement a way to get the interaction range based on the interactable
	return 300.f;
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
