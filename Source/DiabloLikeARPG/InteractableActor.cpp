// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActor.h"
#include "InteractableInterface.h"
#include "InteractorInterface.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

// Sets default values
AInteractableActor::AInteractableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractableActor::NotifyActorOnClicked(FKey ButtonPressed)
{
	Super::NotifyActorOnClicked(ButtonPressed);
	
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	if(PlayerCharacter)
	{
		IInteractorInterface* Interactor = Cast<IInteractorInterface>(PlayerCharacter);
		if(Interactor)
		{
			Interactor->SetInteractableTarget(Cast<IInteractableInterface>(this));
		}
	}
}


void AInteractableActor::Interact(ACharacter* Character)
{
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green,
			FString::Printf(TEXT("Interacting with %s"), *GetName()));	
}

const AActor* AInteractableActor::GetInteractableActor() const
{
	return this;
}

