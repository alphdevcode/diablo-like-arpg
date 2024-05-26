// Copyright 2024 AlphDevCode. All Rights Reserved.


#include "InteractableActor.h"
#include "InteractableInterface.h"
#include "InteractorInterface.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

AInteractableActor::AInteractableActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();
}

void AInteractableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractableActor::NotifyActorOnClicked(FKey ButtonPressed)
{
	Super::NotifyActorOnClicked(ButtonPressed);

	if (IInteractorInterface* Interactor = Cast<IInteractorInterface>(UGameplayStatics::GetPlayerCharacter(this, 0)))
	{
		IInteractableInterface* ThisInteractable = Cast<IInteractableInterface>(this);
		Interactor->SetTargetInteractable(ThisInteractable);
		Interactor->SetCurrentInteractable(ThisInteractable);
	}
}


void AInteractableActor::Interact(ACharacter* InteractorCharacter)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green,
		                                 FString::Printf(TEXT("Interacting with %s"), *GetName()));
}

AActor* AInteractableActor::GetInteractableActor()
{
	return this;
}
