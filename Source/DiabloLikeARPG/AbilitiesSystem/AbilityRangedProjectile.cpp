// Copyright 2024 AlphDevCode. All Rights Reserved.


#include "AbilityRangedProjectile.h"

#include "DiabloLikeARPG/DiabloLikeARPGAnimInstance.h"
#include "DiabloLikeARPG/Characters/DiabloLikeARPGCharacter.h"
#include "DiabloLikeARPG/Utils/ActorFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

void AAbilityRangedProjectile::Initialize()
{
	Super::Initialize();
}

void AAbilityRangedProjectile::BeforeActivateAbility()
{
	// Only assign the target to last interactable for the player
	if(const ADiabloLikeARPGCharacter* Character = Cast<ADiabloLikeARPGCharacter>(Caster); Character != nullptr
		&& Character == UGameplayStatics::GetPlayerCharacter(this, 0))
	{
		Target = Character->GetTargetInteractableActor();
	}
}

void AAbilityRangedProjectile::AbilityActivated()
{
	Super::AbilityActivated();
	bShouldLookAtTarget = true;

	CasterAnimInstance->OnAnimNotifyAttackPeak
		.AddDynamic(this, &AAbilityRangedProjectile::HandleAbilityEffectsSpawning);
}

void AAbilityRangedProjectile::ResetAbility()
{
	Super::ResetAbility();

	bShouldLookAtTarget = false;
}

void AAbilityRangedProjectile::HandleAbilityEffectsSpawning()
{
	SpawnAbilityEffectsWithLocation(Caster->GetMesh()->GetSocketLocation(AbilityEffectsSocket));

	ResetAbility();
	CasterAnimInstance->OnAnimNotifyAttackPeak
		.RemoveDynamic(this, &AAbilityRangedProjectile::HandleAbilityEffectsSpawning);
}

void AAbilityRangedProjectile::RotateToFaceTarget()
{
	if (Caster != nullptr && Target != nullptr)
	{
		UActorFunctionLibrary::LookAtDestination(Caster, Target->GetActorLocation(), 15.f);
	}
}

void AAbilityRangedProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bShouldLookAtTarget)
	{
		RotateToFaceTarget();
	}
}