// Copyright 2024 AlphDevCode. All Rights Reserved.

#include "Ability.h"

#include "AbilityEffect.h"
#include "../StatsComponent.h"
#include "DiabloLikeARPG/DiabloLikeARPGAnimInstance.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

AAbility::AAbility()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAbility::BeginPlay()
{
	Super::BeginPlay();

	// Currently we wait for the next tick so properties are injected before calling Initialize
	// TODO: Consider using SpawnActorDeferred https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Runtime/Engine/Engine/UWorld/SpawnActorDeferred?application_version=5.3
	GetWorldTimerManager().SetTimerForNextTick(this, &AAbility::Initialize);
	// Initialize();
}

/** Initializes Ability specific properties and behaviors. Should be implemented in child classes */
void AAbility::Initialize()
{
	CasterAnimInstance = Cast<UDiabloLikeARPGAnimInstance>(Caster->GetMesh()->GetAnimInstance());
	OnAbilityActivated.AddDynamic(this, &AAbility::AbilityActivated);
}

/** Triggered before the ability is activated. Should be implemented in child classes */
void AAbility::BeforeActivateAbility()
{
}

/** Triggered when the ability is activated. Should be implemented in child classes */
void AAbility::AbilityActivated()
{
}

/** Reset Ability specific properties and behavior to their default state. Should be implemented in child classes */
void AAbility::ResetAbility()
{
}

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

float AAbility::GetRemainingCooldown() const
{
	return RemainingCooldown;
}

float AAbility::GetRange() const
{
	return Range;
}

void AAbility::SpawnAbilityEffects()
{
	SpawnAbilityEffectsWithLocation(EffectsSpawnLocation);
}

void AAbility::SpawnAbilityEffectsWithLocation(const FVector& SpawnLocation)
{
	for (TSubclassOf<AAbilityEffect> Effect : Effects)
	{
		if (Effect.GetDefaultObject() != nullptr && Caster != nullptr)
		{
			const FTransform SpawnTransform = FTransform(Caster->GetActorRotation(), SpawnLocation);
			AAbilityEffect* AbilityEffect = GetWorld()->
				SpawnActorDeferred<AAbilityEffect>(Effect, SpawnTransform);
			AbilityEffect->SetOwner(Caster);
			AbilityEffect->SetParentAbility(this);
			AbilityEffect->FinishSpawning(SpawnTransform);
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

	BeforeActivateAbility();
	RemainingCooldown = Cooldown;
	OnAbilityActivated.Broadcast();

	UStatsComponent* StatsComponent = GetOwner()->GetComponentByClass<UStatsComponent>();
	if (StatsComponent != nullptr)
	{
		StatsComponent->ConsumeMana(ManaCost);
	}

	EffectsSpawnLocation = NewEffectsSpawnLocation;
	if (bAutoActivateAbilityEffects)
	{
		SpawnAbilityEffects();
	}

	if (bAutoActivateCastSound && CastSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, CastSound, GetActorLocation());
	}
}
