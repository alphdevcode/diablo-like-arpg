// Fill out your copyright notice in the Description page of Project Settings.


#include "StatsComponent.h"

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


// Called when the game starts
void UStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = Stats[EPlayerStat::MaxHealth];
	Mana = Stats[EPlayerStat::MaxMana];

	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UStatsComponent::OnTakeAnyDamage);
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
