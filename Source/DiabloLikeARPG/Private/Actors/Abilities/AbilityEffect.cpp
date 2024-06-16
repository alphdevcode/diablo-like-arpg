// Copyright 2024 AlphDevCode. All Rights Reserved.


#include "Actors/Abilities/AbilityEffect.h"

AAbilityEffect::AAbilityEffect()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AAbilityEffect::BeginPlay()
{
	Super::BeginPlay();

	// Currently we wait for the next tick so properties are injected before calling Initialize
	// TODO: Consider using SpawnActorDeferred https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Runtime/Engine/Engine/UWorld/SpawnActorDeferred?application_version=5.3
	GetWorldTimerManager().SetTimerForNextTick(this, &AAbilityEffect::OnActivate);
}

void AAbilityEffect::OnActivate()
{
}

void AAbilityEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAbilityEffect::SetParentAbility(AAbility* Ability)
{
	ParentAbility = Ability;
}

