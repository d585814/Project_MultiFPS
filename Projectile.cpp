// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "MulFPSTESTCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"


// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SetReplicates(true);
	SetReplicateMovement(true);

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	RootComponent = Collision;

	Pariticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	Pariticle->SetupAttachment(Collision);

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->ProjectileGravityScale = 0.0f;

	Collision->SetCollisionProfileName(FName("ProjectileColl"));

	//
	Collision->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	
	Damage = 10.f;
	DamageType = UDamageType::StaticClass();
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}



void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if(Hit.bBlockingHit)
	{
		UWorld* World = GetWorld();
		check(World);
		AMulFPSTESTCharacter* Player = Cast<AMulFPSTESTCharacter>(OtherActor);
		UParticleSystem* BloodEffect = LoadObject<UParticleSystem>(NULL, TEXT("ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Blood/P_Blood_Splat_Cone.P_Blood_Splat_Cone'"));
		UParticleSystem* ExplosionEffect = LoadObject<UParticleSystem>(NULL, TEXT("ParticleSystem'/Game/ParagonTwinblast/FX/Particles/Abilities/Primary/FX/P_TwinBlast_Primary_HitWorld.P_TwinBlast_Primary_HitWorld'"));

		if (Player != nullptr)
		{			
			UGameplayStatics::SpawnEmitterAtLocation(World, BloodEffect, Hit.ImpactPoint);
			UGameplayStatics::ApplyPointDamage(OtherActor, Damage, NormalImpulse, Hit, GetInstigator()->Controller, this, DamageType);
			Destroy();
		}
			
		else
		{
			UGameplayStatics::SpawnEmitterAtLocation(World, ExplosionEffect, Hit.ImpactPoint);
			Destroy();
		}
	}
}

