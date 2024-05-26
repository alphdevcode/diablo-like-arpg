// Copyright 2024 AlphDevCode. All Rights Reserved.

#include "EnemyARPGCharacter.h"

#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"

AEnemyARPGCharacter::AEnemyARPGCharacter()
{
	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBar->SetupAttachment(RootComponent);
	HealthBar->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
}

void AEnemyARPGCharacter::BeginPlay()
{
	Super::BeginPlay();

	HealthBar->SetVisibility(false);
}
 
IInteractorInterface* AEnemyARPGCharacter::GetPlayerInteractor() const
{
	return Cast<IInteractorInterface>
		(UGameplayStatics::GetPlayerCharacter(this, 0));
}

void AEnemyARPGCharacter::NotifyActorOnClicked(FKey ButtonPressed)
{
	Super::NotifyActorOnClicked(ButtonPressed);

	// if(GEngine)
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, TEXT("ENEMY CLICKED"));
	// }

	if (IInteractorInterface* PlayerInteractor = GetPlayerInteractor())
	{
		PlayerInteractor->SetCurrentInteractable(Cast<IInteractableInterface>(this));
	}
	// SetThisAsPlayerInteractableTarget();
}

void AEnemyARPGCharacter::NotifyActorBeginCursorOver()
{
	Super::NotifyActorBeginCursorOver();
	GetMesh()->SetOverlayMaterial(OverlayMaterial);
	HealthBar->SetVisibility(true);

	if (IInteractorInterface* PlayerInteractor = GetPlayerInteractor())
	{
		PlayerInteractor->SetTargetInteractable(Cast<IInteractableInterface>(this));
	}
}

void AEnemyARPGCharacter::NotifyActorEndCursorOver()
{
	Super::NotifyActorEndCursorOver();
	GetMesh()->SetOverlayMaterial(nullptr);
	HealthBar->SetVisibility(false);
}

void AEnemyARPGCharacter::Interact(ACharacter* InteractorCharacter)
{
	if (ADiabloLikeARPGCharacter* DiabloLikeARPGCharacter = Cast<ADiabloLikeARPGCharacter>(InteractorCharacter))
	{
		DiabloLikeARPGCharacter->ActivatePrimaryAttackAbility();
	}
}

AActor* AEnemyARPGCharacter::GetInteractableActor()
{
	return this;
}
