// Copyright 2024 AlphDevCode. All Rights Reserved.


#include "AbilityEffectFX.h"

#include "Ability.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

void AAbilityEffectFX::BeginPlay()
{
	Super::BeginPlay();

	if (AbilityFX == nullptr)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red,
			                                 TEXT("Can't activate AbilityEffectFX, AbilityFX is not valid!"));
		return;
	}

	if (bShouldAttachToCaster)
	{
		if(ParentAbility->Caster == nullptr)
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red,
				                                 TEXT("Can't activate attached AbilityEffectFX, Caster is not valid!"));
			return;
		}
		UGameplayStatics::SpawnEmitterAttached(AbilityFX, ParentAbility->Caster->GetMesh(),
		                                       AttachPointName);
	}
	else
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, AbilityFX, GetActorLocation());
	}
}
