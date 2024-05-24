// Fill out your copyright notice in the Description page of Project Settings.


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

void AEnemyARPGCharacter::NotifyActorOnClicked(FKey ButtonPressed)
{
	Super::NotifyActorOnClicked(ButtonPressed);

	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, TEXT("ENEMY CLICKED"));
	}

	if(ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0))
	{
		if(IInteractorInterface* Interactor = Cast<IInteractorInterface>(PlayerCharacter))
		{
			Interactor->SetInteractableTarget(Cast<IInteractableInterface>(this));
		}
	}
}

void AEnemyARPGCharacter::NotifyActorBeginCursorOver()
{
	Super::NotifyActorBeginCursorOver();
	GetMesh()->SetOverlayMaterial(OverlayMaterial);
	HealthBar->SetVisibility(true);
}

void AEnemyARPGCharacter::NotifyActorEndCursorOver()
{
	Super::NotifyActorEndCursorOver();
	GetMesh()->SetOverlayMaterial(nullptr);
	HealthBar->SetVisibility(false);
}

void AEnemyARPGCharacter::Interact(ACharacter* InteractorCharacter)
{
	if(ADiabloLikeARPGCharacter* DiabloLikeARPGCharacter = Cast<ADiabloLikeARPGCharacter>(InteractorCharacter))
	{
		DiabloLikeARPGCharacter->ActivatePrimaryAttackAbility();
	}
}

AActor* AEnemyARPGCharacter::GetInteractableActor()
{
	return this;
}