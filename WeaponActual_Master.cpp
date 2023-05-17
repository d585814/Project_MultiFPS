// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponActual_Master.h"
#include "Components/SkeletalMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Texture2D.h"

// Sets default values
AWeaponActual_Master::AWeaponActual_Master()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//SetReplicates(true);
	bReplicates = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(RootComponent);
}


// Called when the game starts or when spawned
void AWeaponActual_Master::BeginPlay()
{
	Super::BeginPlay();
	
}



