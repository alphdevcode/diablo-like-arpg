// Copyright 2024 AlphDevCode. All Rights Reserved.


#include "Actors/Abilities/AbilityEffectSound.h"

#include "Actors/Abilities/Ability.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Logger.h"

void AAbilityEffectSound::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySound == nullptr)
	{
		LOG_ERROR(TEXT("Can't activate AbilityEffectSound, AbilitySound is not valid!"));
		return;
	}

	if (bShouldAttachToCaster)
	{
		if(ParentAbility->Caster == nullptr)
		{
			LOG_ERROR(TEXT("Can't activate attached AbilityEffectSound, Caster is not valid!"));
			return;
		}
		UGameplayStatics::SpawnSoundAttached(AbilitySound, ParentAbility->Caster->GetMesh(),
											   AttachPointName);
	}
	else
	{
		UGameplayStatics::SpawnSoundAtLocation(this, AbilitySound, GetActorLocation());
	}

	FTimerHandle DestroyTimer;
	GetWorldTimerManager().SetTimer(DestroyTimer, this,
		&AAbilityEffectSound::DestroyEffect, .2f, false);
}

void AAbilityEffectSound::DestroyEffect()
{
	Destroy();
}