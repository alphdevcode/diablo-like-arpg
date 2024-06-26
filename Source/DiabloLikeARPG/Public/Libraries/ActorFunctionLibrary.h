// Copyright 2024 AlphDevCode. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ActorFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOLIKEARPG_API UActorFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "ActorUtils")
	static FRotator LookAtDestination(AActor* Actor, const FVector& Destination, const float InterpSpeed = 10.f);

};
