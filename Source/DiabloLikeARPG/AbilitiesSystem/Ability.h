// Copyright 2024 AlphDevCode. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ability.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAbilityActivated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActivateAbilityEffects);

UCLASS()
class DIABLOLIKEARPG_API AAbility : public AActor
{
	GENERATED_BODY()

public:
	AAbility();

protected:
	virtual void BeginPlay() override;

	virtual void Initialize();
	
	UPROPERTY(BlueprintReadWrite)
	class UDiabloLikeARPGAnimInstance* CasterAnimInstance;
	
	UPROPERTY(EditAnywhere, Category = "Ability")
	bool bAutoActivateAbilityEffects = false;
	
	UPROPERTY(EditAnywhere, Category = "Ability")
	float ManaCost;

	UPROPERTY(EditAnywhere, Category = "Ability")
	float Range = 100.f;
	
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	// bool bIsActive = false;

	UPROPERTY(EditAnywhere, Category = "Ability")
	float Cooldown = 0.5f;
	
	float RemainingCooldown;

	UPROPERTY(EditAnywhere, Category = "Ability")
	TArray<TSubclassOf<class AAbilityEffect>> Effects;

	UPROPERTY(EditAnywhere, Category = "AbilityInfo")
	FText AbilityName;

	UPROPERTY(EditAnywhere, Category = "AbilityInfo")
	FText AbilityDescription;

	UPROPERTY(EditAnywhere, Category = "AbilityInfo")
	UTexture2D* AbilityIcon;

	FVector EffectsSpawnLocation;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, Category = "AbilityTargetting")
	ACharacter* Caster;

	UPROPERTY(BlueprintReadWrite, Category = "AbilityTargetting")
	AActor* Target;

	// UPROPERTY(BlueprintReadWrite, Category = "AbilityTargetting")
	// FVector EffectsSpawnLocation;
	
	UFUNCTION(BlueprintPure)
	UTexture2D* GetAbilityIcon() const { return AbilityIcon; }

	UFUNCTION(BlueprintPure)
	FText GetAbilityName() const { return AbilityName; }

	UFUNCTION(BlueprintPure)
	FText GetAbilityDescription() const { return AbilityDescription; }

	UFUNCTION(BlueprintPure)
	bool CanActivateAbility() const;

	// Note we can't overload a UFUNCTION
	UFUNCTION(BlueprintCallable)
	void SpawnAbilityEffects();
	UFUNCTION(BlueprintCallable)
	void SpawnAbilityEffectsWithLocation(const FVector& SpawnLocation);

	// UFUNCTION(BlueprintPure)
	// bool IsActive() const;
	
	UFUNCTION(BlueprintCallable)
	void ActivateAbility(const FVector& NewEffectsSpawnLocation);

	virtual void ResetAbility();
	
    UPROPERTY(BlueprintAssignable, Category = "Ability")
	FOnAbilityActivated OnAbilityActivated;
	
    UPROPERTY(BlueprintAssignable, Category = "Ability")
	FActivateAbilityEffects ActivateAbilityEffects;

};
