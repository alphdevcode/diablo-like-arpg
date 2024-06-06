// Copyright 2024 AlphDevCode. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DiabloLikeARPGGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPawnDiedSignature, APawn*, Pawn);

UCLASS(minimalapi)
class ADiabloLikeARPGGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	const FOnPawnDiedSignature& GetOnPawnDied() const { return OnPawnDied; }

	//Tries to Spawn the player's pawn.
	virtual void RestartPlayer(AController* NewPlayer) override;
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void PawnDied(APawn* PawnKilled);
	
	//Called when Player character has died.
	UFUNCTION()
	virtual void PlayerDied(AController* Controller);
	
	UPROPERTY()
	FOnPawnDiedSignature OnPawnDied;
};



