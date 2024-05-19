// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableInterface.h"

// Add default functionality here for any IInteractableInterface functions that are not pure virtual.

void IInteractableInterface::Interact(ACharacter* Character)
{
}

const AActor* IInteractableInterface::GetInteractableActor() const
{
	return nullptr;
}

