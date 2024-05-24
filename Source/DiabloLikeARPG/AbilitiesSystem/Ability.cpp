// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability.h"

#include "AbilityEffect.h"
#include "../StatsComponent.h"
#include "GameFramework/Character.h"

// Sets default values
AAbility::AAbility()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAbility::BeginPlay()
{
	Super::BeginPlay();

	// ActivateAbility();
}

// Called every frame
void AAbility::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (RemainingCooldown > 0)
	{
		RemainingCooldown -= DeltaTime;
	}
}

bool AAbility::CanActivateAbility() const
{
	if (RemainingCooldown > 0)
	{
		return false;
	}

	const UStatsComponent* StatsComponent = GetOwner()->GetComponentByClass<UStatsComponent>();
	if (StatsComponent != nullptr)
	{
		return StatsComponent->GetMana() >= ManaCost;
	}

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
		                                 TEXT("Can not find StatsComponent when trying to activate ability."));
	return false;
}

void AAbility::SpawnAbilityEffects()
{
	for (TSubclassOf<AAbilityEffect> Effect : Effects)
	{
		if (Effect.GetDefaultObject() != nullptr && Caster != nullptr)
		{
			AAbilityEffect* AbilityEffect = GetWorld()->
				SpawnActor<AAbilityEffect>(Effect, EffectsSpawnLocation, FRotator::ZeroRotator);
			AbilityEffect->SetOwner(Caster);
			AbilityEffect->SetPatentAbility(this);
		}
	}
}

void AAbility::ActivateAbility(const FVector& NewEffectsSpawnLocation)
{
	if (!CanActivateAbility())
	{
		// if (GEngine)
		// 	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
		// 	                                 TEXT("Can not activate ability"));
		return;
	}
	// if (!bIsActive)
	// {
	// 	if(GEngine)
	// 		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
	// 			TEXT("Ability is already active."));
	// 	return;
	// }
	// bIsActive = true;
	RemainingCooldown = Cooldown;
	OnAbilityActivated.Broadcast();

	UStatsComponent* StatsComponent = GetOwner()->GetComponentByClass<UStatsComponent>();
	if (StatsComponent != nullptr)
	{
		StatsComponent->ConsumeMana(ManaCost);
	}
	
	EffectsSpawnLocation = NewEffectsSpawnLocation;
	if(bAutoActivateAbilityEffects)
	{
		SpawnAbilityEffects();
	}
}
