// Copyright 2024 AlphDevCode. All Rights Reserved.


#include "Actors/Abilities/AbilityWithSingleAnimationBase.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Libraries/Logger.h"

void AAbilityWithSingleAnimationBase::AbilityActivated()
{
	Super::AbilityActivated();

	if (AbilityAnimMontage != nullptr)
	{
		Caster->PlayAnimMontage(AbilityAnimMontage);
	}
	else
	{
		LOG_ERROR(TEXT("AbilityAnimMontage is not valid!"));
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
		LOG_WARNING(TEXT("CastAbilityEffect is not valid!"));
	}
}
