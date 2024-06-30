// Copyright 2024 AlphDevCode. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

class UProjectileMovementComponent;
class UBoxComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProjectileHit, AActor*, OtherActor, const FHitResult&, Hit);

UCLASS()
class DIABLOLIKEARPG_API AProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	AProjectileBase();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	           FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* ArrowComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* ProjectileParticleSystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Visual", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ImpactFX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Sound", meta = (AllowPrivateAccess = "true"))
	USoundBase* ImpactSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ProjectileSettings",
		meta = (AllowPrivateAccess = "true", ExposeOnSpawn=true))
	float InitialSpeed = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ProjectileSettings",
		meta = (AllowPrivateAccess = "true", ExposeOnSpawn=true))
	float MaxSpeed = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ProjectileSettings",
		meta = (AllowPrivateAccess = "true", ExposeOnSpawn=true))
	float GravityScale = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ProjectileSettings",
		meta = (AllowPrivateAccess = "true", ExposeOnSpawn=true))
	float Damage = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ProjectileSettings",
		meta = (AllowPrivateAccess = "true", ExposeOnSpawn=true))
	bool bIsHomingTarget = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ProjectileSettings",
		meta = (AllowPrivateAccess = "true", ExposeOnSpawn=true, EditCondition = "bIsHomingTarget == true"))
	float HomingAccelerationMagnitude = 2000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Targetting",
		meta = (AllowPrivateAccess = "true", ExposeOnSpawn=true))
	AActor* Target;

private:
	void RotateToTarget();

	void IgnoreCollisionsWithCaster();

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetDamage(float NewDamage);

	UFUNCTION(BlueprintCallable)
	void SetTarget(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void InitializeValues(float NewInitialSpeed = 1300.f,
	                float NewMaxSpeed = 1500.f, float NewGravityScale = 0.f,
	                bool bNewIsHomingTarget = false,
	                float NewHomingAccelerationMagnitude = 2000.f);

	UPROPERTY(BlueprintAssignable)
	FOnProjectileHit OnProjectileHit;
};
