// Copyright 2024 AlphDevCode. All Rights Reserved.


#include "ActorFunctionLibrary.h"

#include "Kismet/KismetMathLibrary.h"

FRotator UActorFunctionLibrary::LookAtDestination(AActor* Actor, const FVector& Destination, const float InterpSpeed)
{
	if (Actor == nullptr)
	{
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red,
			TEXT("LookAtDestination: Character is nullptr"));
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
