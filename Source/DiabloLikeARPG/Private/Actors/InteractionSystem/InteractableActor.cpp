// Copyright 2024 AlphDevCode. All Rights Reserved.


#include "DiabloLikeARPG/Public/Actors/InteractionSystem/InteractableActor.h"

#include "DiabloLikeARPG/Public/Actors/InteractionSystem/InteractorInterface.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Libraries/Logger.h"

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
	LOG_INFO(TEXT("Interacting with %s"), *GetName());
}

AActor* AInteractableActor::GetInteractableActor()
{
	return this;
}
