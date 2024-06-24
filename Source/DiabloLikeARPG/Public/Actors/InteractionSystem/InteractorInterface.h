// Copyright 2024 AlphDevCode. All Rights Reserved.

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

public:
	/** Target Interactable would be the one that the player is planning to interact with e.g. hovering */
	virtual void SetTargetInteractable(IInteractableInterface* Interactable) = 0;
	/** Current Interactable would be the one that the player is currently interacting with e.g. clicking */
	virtual void SetCurrentInteractable(IInteractableInterface* Interactable) = 0;
	virtual float GetInteractionRange() = 0;
};
