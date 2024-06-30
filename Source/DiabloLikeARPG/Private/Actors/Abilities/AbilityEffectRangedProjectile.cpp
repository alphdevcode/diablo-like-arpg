// Copyright 2024 AlphDevCode. All Rights Reserved.


#include "Actors/Abilities/AbilityEffectRangedProjectile.h"

#include "Actors/ProjectileBase.h"
#include "Actors/Abilities/Ability.h"
#include "GameFramework/Character.h"

void AAbilityEffectRangedProjectile::OnActivate()
{
	Super::OnActivate();

	AProjectileBase* Projectile = GetWorld()->SpawnActorDeferred<AProjectileBase>(ProjectileClass,
		GetActorTransform());
	Projectile->SetOwner(ParentAbility->Caster);
	Projectile->SetTarget(ParentAbility->Target);
	Projectile->SetDamage(BaseDamage);
	Projectile->SetInstigator(ParentAbility->Caster);
	Projectile->InitializeValues(1300.f,
		1500.f,
		0.f,
		false,
		2000.f);
	
	Projectile->FinishSpawning(GetActorTransform());
	Destroy();
}
