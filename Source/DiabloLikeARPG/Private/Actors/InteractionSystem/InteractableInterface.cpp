// Copyright 2024 AlphDevCode. All Rights Reserved.


#include "Actors/InteractionSystem/InteractableInterface.h"

// Add default functionality here for any IInteractableInterface functions that are not pure virtual.

void IInteractableInterface::Interact(ACharacter* InteractorCharacter)
{
}

AActor* IInteractableInterface::GetInteractableActor()
{
	return nullptr;
}


