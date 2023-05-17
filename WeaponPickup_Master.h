// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponPickup_Master.generated.h"


UCLASS()
class MULFPSTEST_API AWeaponPickup_Master : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponPickup_Master();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* Collision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* Mesh;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultSetting")
	TSubclassOf<class AWeaponActual_Master> ActualWeaponClass;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	


};
