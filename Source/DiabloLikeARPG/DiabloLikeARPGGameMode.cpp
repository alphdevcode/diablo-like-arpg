// Copyright 2024 AlphDevCode. All Rights Reserved.

#include "DiabloLikeARPGGameMode.h"
#include "DiabloLikeARPGPlayerController.h"
#include "UObject/ConstructorHelpers.h"

ADiabloLikeARPGGameMode::ADiabloLikeARPGGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ADiabloLikeARPGPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}