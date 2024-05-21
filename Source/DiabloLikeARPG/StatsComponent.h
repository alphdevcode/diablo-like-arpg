// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, HealthPercent, float, HealthValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnManaChanged, float, ManaPercent, float, ManaValue);

UENUM(BlueprintType)
enum class EPlayerStat : uint8
{
	MaxHealth UMETA(DisplayName = "MaxHealth"),
	MaxMana UMETA(DisplayName = "MaxMana"),
	Strength UMETA(DisplayName = "Strength"),
	Dexterity UMETA(DisplayName = "Dexterity"),
	Intelligence UMETA(DisplayName = "Intelligence"),
	Armor UMETA(DisplayName = "Armor"),
	Damage UMETA(DisplayName = "Damage"),
	CriticalChance UMETA(DisplayName = "CriticalChance"),
	CriticalDamage UMETA(DisplayName = "CriticalDamage"),
	AttackSpeed UMETA(DisplayName = "AttackSpeed"),
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DIABLOLIKEARPG_API UStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStatsComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	float Health = 0.f;
	float Mana = 0.f;

	UPROPERTY(EditAnywhere, Category = "Stats")
	int32 MaxLevel = 20;
	int32 Level = 1;


	UPROPERTY(EditAnywhere, Category = "Stats")
	int32 MaxXp = 100;
	int32 Xp = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	TMap<EPlayerStat, float> Stats = {
		{EPlayerStat::MaxHealth, 100.f},
		{EPlayerStat::MaxMana, 100.f},
		{EPlayerStat::Strength, 10.f},
		{EPlayerStat::Dexterity, 10.f},
		{EPlayerStat::Intelligence, 10.f},
		{EPlayerStat::Armor, 10.f},
		{EPlayerStat::Damage, 10.f},
	};

	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	                     AController* InstigatedBy, AActor* DamageCauser);

	void UpdateStat(EPlayerStat StatToUpdate, float Value);

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;

	UFUNCTION(BlueprintPure)
	float GetManaPercent() const;

	UFUNCTION(BlueprintPure)
	float GetStatValue(EPlayerStat Stat) const;

	UFUNCTION(BlueprintPure)
	float GetHealth() const { return Health; }
	
	UFUNCTION(BlueprintPure)
	float GetMana() const { return Mana; }

	UFUNCTION(BlueprintCallable)
	void LevelUp();

	UFUNCTION(BlueprintCallable)
	void ConsumeMana(float Amount);

	UFUNCTION(BlueprintCallable)
	void ReduceHealth(float Amount);

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnManaChanged OnManaChanged;
};
