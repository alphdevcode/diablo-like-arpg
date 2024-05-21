// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ability.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAbilityActivated);

UCLASS()
class DIABLOLIKEARPG_API AAbility : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAbility();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Ability")
	float ManaCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	bool bIsActive = false;

	UPROPERTY(EditAnywhere, Category = "Ability")
	float Cooldown = 0.f;

	UPROPERTY(EditAnywhere, Category = "Ability")
	TArray<TSubclassOf<class AAbilityEffect>> Effects;

	UPROPERTY(EditAnywhere, Category = "AbilityInfo")
	FText AbilityName;

	UPROPERTY(EditAnywhere, Category = "AbilityInfo")
	FText AbilityDescription;

	UPROPERTY(EditAnywhere, Category = "AbilityInfo")
	UTexture2D* AbilityIcon;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityTargetting")
	ACharacter* Caster;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityTargetting")
	AActor* Target;
	
	UFUNCTION(BlueprintPure)
	UTexture2D* GetAbilityIcon() const { return AbilityIcon; }

	UFUNCTION(BlueprintPure)
	FText GetAbilityName() const { return AbilityName; }

	UFUNCTION(BlueprintPure)
	FText GetAbilityDescription() const { return AbilityDescription; }

	UFUNCTION(BlueprintPure)
	bool CanActivateAbility() const;

	UFUNCTION(BlueprintCallable)
	void ActivateAbility();

	// add an event dispatcher
    UPROPERTY(BlueprintAssignable, Category = "Ability")
	FOnAbilityActivated OnAbilityActivated;
};
