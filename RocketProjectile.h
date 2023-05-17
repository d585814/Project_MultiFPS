// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RocketProjectile.generated.h"

UCLASS()
class MULFPSTEST_API ARocketProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARocketProjectile();
	
	UPROPERTY(EditAnywhere, Category = "Setting")
	class UProjectileMovementComponent* Movement;

	UPROPERTY(EditAnywhere, Category = "Setting")
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = "Setting")
	TSubclassOf<class ARocketExprosion> ExplosionClass;

	void SpawnDamage();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



};
