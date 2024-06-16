// Copyright 2024 AlphDevCode. All Rights Reserved.

#include "Actors/GameModes/DiabloLikeARPGGameMode.h"
#include "UObject/ConstructorHelpers.h"

void ADiabloLikeARPGGameMode::RestartPlayer(AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);
}

void ADiabloLikeARPGGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (!OnPawnDied.IsBound())
	{
		OnPawnDied.AddDynamic(this, &ADiabloLikeARPGGameMode::PawnDied);
	}
}

void ADiabloLikeARPGGameMode::PlayerDied(AController* Controller)
{
}

void ADiabloLikeARPGGameMode::PawnDied(APawn* PawnKilled)
{
}
