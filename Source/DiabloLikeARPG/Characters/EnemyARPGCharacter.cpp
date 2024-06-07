// Copyright 2024 AlphDevCode. All Rights Reserved.

#include "EnemyARPGCharacter.h"

#include "Components/WidgetComponent.h"
#include "DiabloLikeARPG/Components/AbilitiesComponent.h"
#include "DiabloLikeARPG/GameManagement/DiabloLikeARPGGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISense_Damage.h"

AEnemyARPGCharacter::AEnemyARPGCharacter()
{
	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBar->SetupAttachment(RootComponent);
	HealthBar->SetRelativeLocation(FVector(0.f, 0.f, 100.f));

	EnemyName = "Enemy";
}

float AEnemyARPGCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	// Broadcast a report damage event for the AI Perception System
	UAISense_Damage::ReportDamageEvent(this, this, DamageCauser, Damage,
		GetActorLocation(), GetActorLocation());

	return Damage;
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

void AEnemyARPGCharacter::Attack()
{
	AbilitiesComponent->ActivatePrimaryAttackAbility();
}

void AEnemyARPGCharacter::ActivateMainAbility()
{
	AbilitiesComponent->ActivateAbility(0);
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
	// HealthBar->SetVisibility(true);

	
	if(const ADiabloLikeARPGGameState* GameState = Cast<ADiabloLikeARPGGameState>
			(UGameplayStatics::GetGameState(this)))
	{
		GameState->OnBeginEnemyHover.Broadcast(this);
	}

	if (IInteractorInterface* PlayerInteractor = GetPlayerInteractor())
	{
		PlayerInteractor->SetTargetInteractable(Cast<IInteractableInterface>(this));
	}
}

void AEnemyARPGCharacter::NotifyActorEndCursorOver()
{
	Super::NotifyActorEndCursorOver();
	GetMesh()->SetOverlayMaterial(nullptr);
	// HealthBar->SetVisibility(false);

	if(const ADiabloLikeARPGGameState* GameState = Cast<ADiabloLikeARPGGameState>
			(UGameplayStatics::GetGameState(this)))
	{
		GameState->OnEndEnemyHover.Broadcast(this);
	}
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

FName AEnemyARPGCharacter::GetEnemyName() const
{
	return EnemyName;
}
