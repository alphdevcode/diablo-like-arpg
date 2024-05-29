// Copyright 2024 AlphDevCode. All Rights Reserved.


#include "ProjectileBase.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"

AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Root Component"));
	RootComponent = BoxComponent;
	
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	ProjectileMesh->SetupAttachment(RootComponent);
	
	ProjectileParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Projectile Particle System"));
	ProjectileParticleSystemComponent->SetupAttachment(RootComponent);
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile Movement Component");
	ArrowComponent = CreateDefaultSubobject<UArrowComponent>("Arrow Component");
	ArrowComponent->SetupAttachment(RootComponent);

	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->InitialSpeed = InitialSpeed;
	ProjectileMovementComponent->MaxSpeed = MaxSpeed;
	ProjectileMovementComponent->ProjectileGravityScale = GravityScale;
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	if(BoxComponent != nullptr)
	{
		BoxComponent->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
	}

	if(bIsHomingTarget && Target != nullptr)
	{
		ProjectileMovementComponent->bIsHomingProjectile = true;
		ProjectileMovementComponent->HomingTargetComponent = Target->GetRootComponent();
		ProjectileMovementComponent->HomingAccelerationMagnitude = HomingAccelerationMagnitude;
	}

	RotateToTarget();
}

void AProjectileBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if(BoxComponent != nullptr)
	{
		BoxComponent->OnComponentHit.RemoveDynamic(this, &AProjectileBase::OnHit);
	}
}

void AProjectileBase::RotateToTarget()
{
	if(Target != nullptr)
	{
		ProjectileMovementComponent->Velocity = UKismetMathLibrary::GetDirectionUnitVector(
			GetActorLocation(), Target->GetActorLocation()) * InitialSpeed;
	}
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                            FVector NormalImpulse, const FHitResult& Hit)
{
	if(OtherActor == GetOwner())
	{
		return;
	}
	
	OnProjectileHit.Broadcast(OtherActor, Hit);

	if(ImpactFX != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactFX, Hit.Location,
			GetActorRotation());
	}

	if(ImpactSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	}

	UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(),
		this, nullptr);
	
	Destroy();
}

void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectileBase::SetDamage(const float NewDamage)
{
	Damage = NewDamage;
}

