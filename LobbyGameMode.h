// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MulFPSTESTGameMode.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MULFPSTEST_API ALobbyGameMode : public AMulFPSTESTGameMode
{
	GENERATED_BODY()

public:
	
	void PostLogin(APlayerController* NewPlayer) override;

	void Logout(AController* Exiting) override;

	TArray<class APlayerController*> AllPlayerController;

	UFUNCTION()
	uint16 GetNumTotalPlayer() {return NumTotalPlayer;}

	void CountDown();

	int32 GetCountNum() {return CountNum;}

	FString GetCountText() { return CountText; }


private:
	uint16 NumTotalPlayer = 0;

	FTimerHandle Handle;

	FTimerHandle Count;

	void OpenGame();
	
	int32 CountNum =12;

	FString CountText;
};
