// Copyright 2024 AlphDevCode. All Rights Reserved.


#include "Actors/Abilities/AbilityWithSingleAnimationBase.h"

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
		
		if(bShouldAttachCastEffect)
		{
			UGameplayStatics::SpawnEmitterAttached(CastEffect,
				Caster->GetMesh(), MeshPointNameToSpawnCastEffectOn,
				FVector(ForceInit), FRotator::ZeroRotator, FVector(1),
				EAttachLocation::KeepRelativeOffset, true,
				EPSCPoolMethod::AutoRelease);
		}
		else
		{
			FVector SpawnLocation;
			FRotator SpawnRotation;
			Caster->GetMesh()->GetSocketWorldLocationAndRotation(MeshPointNameToSpawnCastEffectOn,
					SpawnLocation, SpawnRotation);
			UGameplayStatics::SpawnEmitterAtLocation(this, CastEffect,
			          SpawnLocation, SpawnRotation, FVector(1), true, EPSCPoolMethod::AutoRelease);
		}
	}
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow,
			                                 TEXT("CastAbilityEffect is not valid!"));
	}
}
