// Fill out your copyright notice in the Description page of Project Settings.


#include "RocketExprosion.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARocketExprosion::ARocketExprosion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SetReplicates(true);
	//SetReplicateMovement(true);

	Pariticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	RootComponent = Pariticle;

	DamageType = UDamageType::StaticClass();
}

// Called when the game starts or when spawned
void ARocketExprosion::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		FVector loc = GetActorLocation() + FVector(0.f, 0.f, 100.f);
		UGameplayStatics::ApplyRadialDamage(GetWorld(), 1000.f, loc, 400.f, DamageType, TArray<AActor*>(), this, false, ECC_Visibility);
		SetLifeSpan(3.0f);
	}
}



