// Fill out your copyright notice in the Description page of Project Settings.


#include "RocketProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "RocketExprosion.h"

// Sets default values
ARocketProjectile::ARocketProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SetReplicates(true);
	SetReplicateMovement(true);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->ProjectileGravityScale = 0.0f;
}

// Called when the game starts or when spawned
void ARocketProjectile::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &ARocketProjectile::SpawnDamage, 3.0f, false);
	
}

void ARocketProjectile::SpawnDamage()
{
	if (HasAuthority())
	{		
		FActorSpawnParameters params;
		params.Instigator = GetInstigator();
		params.Owner = this;

		FTransform trans = Mesh->GetComponentTransform();
		GetWorld()->SpawnActor<ARocketExprosion>(ExplosionClass, trans, params);

		Destroy();
	}
}
