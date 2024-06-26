// Copyright 2024 AlphDevCode. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InteractableInterface.h"
#include "GameFramework/Actor.h"
#include "InteractableActor.generated.h"

UCLASS()
class DIABLOLIKEARPG_API AInteractableActor : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorOnClicked(FKey ButtonPressed) override;

	// IInteractableInterface
	virtual void Interact(ACharacter* InteractorCharacter) override;
	virtual AActor* GetInteractableActor() override;
};
