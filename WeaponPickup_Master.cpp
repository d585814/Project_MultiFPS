// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponPickup_Master.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "WeaponActual_Master.h"

// Sets default values
AWeaponPickup_Master::AWeaponPickup_Master()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//SetReplicates(true);
	bReplicates = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	Collision->SetupAttachment(RootComponent);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Collision);
		
	ActualWeaponClass = AWeaponActual_Master::StaticClass();
}

// Called when the game starts or when spawned
void AWeaponPickup_Master::BeginPlay()
{
	Super::BeginPlay();
	
}



