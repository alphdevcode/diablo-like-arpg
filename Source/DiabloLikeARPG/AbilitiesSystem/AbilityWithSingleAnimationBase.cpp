// Copyright 2024 AlphDevCode. All Rights Reserved.


#include "AbilityWithSingleAnimationBase.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

void AAbilityWithSingleAnimationBase::AbilityActivated()
{
	Super::AbilityActivated();

	if (AbilityAnimMontage != nullptr)
	{
		Caster->PlayAnimMontage(AbilityAnimMontage);
	}
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red,
			                                 TEXT("AbilityAnimMontage is not valid!"));
	}

	if (CastEffect != nullptr)
	{
		FVector SpawnLocation;
		FRotator SpawnRotation;
		Caster->GetMesh()->GetSocketWorldLocationAndRotation(MeshPointNameToSpawnCastEffectOn,
				SpawnLocation, SpawnRotation);
		
		UGameplayStatics::SpawnEmitterAtLocation(this, CastEffect,
			SpawnLocation, SpawnRotation);
	}
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow,
			                                 TEXT("CastAbilityEffect is not valid!"));
	}
}
