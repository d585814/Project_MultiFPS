// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MulController.generated.h"

/**
 * 
 */
UCLASS()
class MULFPSTEST_API AMulController : public APlayerController
{
	GENERATED_BODY()

public:
	AMulController(const FObjectInitializer& ObjectInitializer);

	//void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void CallCheckController();

	void CheckController();

	UFUNCTION(Server, Reliable)
	void RequestRespawn();
	
	UFUNCTION(Server, Reliable)
	void RequestSpawnSelected(TSubclassOf<class ACharacter> Cha);

	class UCharacterSelect* SelectWidget;

protected:
	virtual void BeginPlay() override;
	
private:
	TSubclassOf<class UUserWidget> SelectWidgetClass;

	UPROPERTY()
	TSubclassOf<class ACharacter> SpawnToCharacter;

	
};
