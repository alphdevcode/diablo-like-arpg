// Copyright 2024 AlphDevCode. All Rights Reserved.


#include "Actors/Abilities/AbilityEffectHealSelf.h"

#include "Actors/Abilities/Ability.h"
#include "Components/StatsComponent.h"
#include "GameFramework/Character.h"


void AAbilityEffectHealSelf::OnActivate()
{
	Super::OnActivate();

	if(UStatsComponent* StatsComponent = ParentAbility->Caster->GetComponentByClass<UStatsComponent>())
	{
		StatsComponent->RecoverHealth(HealAmount);
	}
	else
	{
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red,
				TEXT("Can't find StatsComponent on Ability's Caster!"));
	}

	FTimerHandle DestroyTimer;
	GetWorldTimerManager().SetTimer(DestroyTimer, this, &AAbilityEffectHealSelf::DestroyEffect, .2f, false);
}

void AAbilityEffectHealSelf::DestroyEffect()
{
	Destroy();
}
