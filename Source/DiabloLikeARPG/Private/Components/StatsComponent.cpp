// Copyright 2024 AlphDevCode. All Rights Reserved.


#include "Components/StatsComponent.h"

#include "Libraries/Logger.h"

// Sets default values for this component's properties
UStatsComponent::UStatsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UStatsComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                      AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0) return;

	// if(Stats.Contains(EPlayerStat::Armor))
	// {
	// 	Damage -= Stats[EPlayerStat::Armor];
	// }

	Health -= Damage;
	UE_LOG(LogTemp, Warning, TEXT("Taking Damage... %s"), *GetOwner()->GetName());
}


void UStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = Stats[EPlayerStat::MaxHealth];
	if (OnHealthChanged.IsBound())
	{
		OnHealthChanged.Broadcast(GetHealthPercent(), Health);
	}

	Mana = Stats[EPlayerStat::MaxMana];
	if (OnManaChanged.IsBound())
	{
		OnManaChanged.Broadcast(GetManaPercent(), Mana);
	}
	// create a timer delegate for mana recovery
	const FTimerDelegate RecoverManaOverTimeDelegate =
		FTimerDelegate::CreateUObject(this,
		                              &UStatsComponent::RecoverMana,
		                              GetManaRecoveryAmount());
	GetWorld()->GetTimerManager().SetTimer(ManaRecoveryTimerHandle, RecoverManaOverTimeDelegate,
	                               GetManaRecoveryRate(), true);

	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UStatsComponent::OnTakeAnyDamage);
}

float UStatsComponent::GetManaRecoveryRate() const
{
	return 5/Stats[EPlayerStat::Intelligence];
}

float UStatsComponent::GetManaRecoveryAmount() const
{
	return 1.f * Stats[EPlayerStat::Intelligence]/10;
}


void UStatsComponent::UpdateStat(const EPlayerStat StatToUpdate, const float Value)
{
	//if StatToUpdate exists in the map, update it
	if (Stats.Contains(StatToUpdate))
	{
		Stats[StatToUpdate] += Value;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Stat %s does not exist in the map"),
		       *UEnum::GetDisplayValueAsText(StatToUpdate).ToString());
	}
}

// Called every frame
void UStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UStatsComponent::GetHealthPercent() const
{
	return Health / Stats[EPlayerStat::MaxHealth];
}

float UStatsComponent::GetManaPercent() const
{
	return Mana / Stats[EPlayerStat::MaxMana];
}

float UStatsComponent::GetStatValue(const EPlayerStat Stat) const
{
	if (Stats.Contains(Stat))
	{
		return Stats[Stat];
	}

	UE_LOG(LogTemp, Warning, TEXT("Trying to get stat '%s' which does not exist in the map. Returning 0"),
	       *UEnum::GetDisplayValueAsText(Stat).ToString());
	return 0.f;
}

void UStatsComponent::LevelUp()
{
	if (Level < MaxLevel)
	{
		Level++;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't level up. Max level reached"));
	}
}

void UStatsComponent::ConsumeMana(const float Amount)
{
	if (Mana >= Amount)
	{
		Mana -= Amount;
		if (OnManaChanged.IsBound())
		{
			OnManaChanged.Broadcast(GetManaPercent(), Mana);
		}
	}
	else
	{
		LOG_INFO(TEXT("Not enough mana to consume."));
	}
}

void UStatsComponent::ReduceHealth(const float Amount)
{
	Health -= Amount;
	if (Health <= 0.f)
	{
		Health = 0.f;
	}
	if (OnHealthChanged.IsBound())
	{
		OnHealthChanged.Broadcast(GetHealthPercent(), Health);
	}
}

void UStatsComponent::RecoverMana(const float Amount)
{
	Mana += Amount;
	if (Mana > Stats[EPlayerStat::MaxMana])
	{
		Mana = Stats[EPlayerStat::MaxMana];
	}

	if (OnManaChanged.IsBound())
	{
		OnManaChanged.Broadcast(GetManaPercent(), Mana);
	}
}

void UStatsComponent::RecoverHealth(const float Amount)
{
	Health += Amount;
	if (Health > Stats[EPlayerStat::MaxHealth])
	{
		Health = Stats[EPlayerStat::MaxHealth];
	}

	if (OnHealthChanged.IsBound())
	{
		OnHealthChanged.Broadcast(GetHealthPercent(), Health);
	}
}
