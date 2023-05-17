// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Character.h"
#include "MulFPSTESTGameMode.generated.h"

UCLASS(minimalapi)
class AMulFPSTESTGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMulFPSTESTGameMode();

	UFUNCTION(Server,Reliable) 
	void ReSpawn(AController* Controller, TSubclassOf<class ACharacter> cha);

	//UWorld* WorldGet();

	UFUNCTION(Server, Reliable)
	void SpawnSelected(AController* Controller, TSubclassOf<ACharacter> Character);

protected:
	UPROPERTY(EditAnywhere)
	FTransform RespawnTransform;
};



