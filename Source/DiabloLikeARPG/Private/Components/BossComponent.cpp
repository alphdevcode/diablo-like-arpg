// Copyright 2024 AlphDevCode. All Rights Reserved.


#include "Components/BossComponent.h"

#include "Components/SphereComponent.h"
#include "Actors/Characters/PlayerARPGCharacter.h"
#include "Actors/GameStates/DiabloLikeARPGGameState.h"
#include "Kismet/GameplayStatics.h"

UBossComponent::UBossComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(this);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}


void UBossComponent::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &UBossComponent::OnOverlapBegin);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &UBossComponent::OnOverlapEnd);
}


void UBossComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                    const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == GetOwner() || !OtherComp) return;
	
	if (APlayerARPGCharacter* PlayerCharacter = Cast<APlayerARPGCharacter>(OtherActor))
	{
		if(const ADiabloLikeARPGGameState* GameState = Cast<ADiabloLikeARPGGameState>
			(UGameplayStatics::GetGameState(this)))
		{
			GameState->OnPlayerEntersBossRadius.Broadcast(PlayerCharacter, GetOwner());
		}
	}
}

void UBossComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor || OtherActor == GetOwner() || !OtherComp) return;
		
	if (APlayerARPGCharacter* PlayerCharacter = Cast<APlayerARPGCharacter>(OtherActor))
	{
		if(const ADiabloLikeARPGGameState* GameState = Cast<ADiabloLikeARPGGameState>
			(UGameplayStatics::GetGameState(this)))
		{
			GameState->OnPlayerExitsBossRadius.Broadcast(PlayerCharacter, GetOwner());
		}
	}
}

void UBossComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
