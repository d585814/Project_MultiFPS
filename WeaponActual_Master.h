// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponActual_Master.generated.h"

UCLASS()
class MULFPSTEST_API AWeaponActual_Master : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponActual_Master();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRifle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShotGun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	float WeaponDamage;

	UPROPERTY(EditAnywhere, Category = "Info")
	class UTexture2D* Image;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



};
