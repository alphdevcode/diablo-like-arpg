// Copyright 2024 AlphDevCode. All Rights Reserved.


#include "Libraries/ActorFunctionLibrary.h"

#include "Kismet/KismetMathLibrary.h"
#include "Libraries/Logger.h"

FRotator UActorFunctionLibrary::LookAtDestination(AActor* Actor, const FVector& Destination, const float InterpSpeed)
{
	if (Actor == nullptr)
	{
		LOG_ERROR(TEXT("LookAtDestination: Character is nullptr"));
		return FRotator::ZeroRotator;
	}

	const FRotator LookAtRotator = UKismetMathLibrary::FindLookAtRotation(
		Actor->GetActorLocation(), Destination);

	const FRotator InterpRotator = UKismetMathLibrary::RInterpTo(
		Actor->GetActorRotation(),
		FRotator(0.f, LookAtRotator.Yaw, 0.f),
		Actor->GetWorld()->GetDeltaSeconds(),
		InterpSpeed);

	Actor->SetActorRotation(InterpRotator);
	return LookAtRotator;
}
