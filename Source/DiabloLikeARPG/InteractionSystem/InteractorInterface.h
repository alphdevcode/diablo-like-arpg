// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableInterface.h"
#include "UObject/Interface.h"
#include "InteractorInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DIABLOLIKEARPG_API IInteractorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SetInteractableTarget(IInteractableInterface* Interactable);
	virtual float GetInteractionRange();
};
