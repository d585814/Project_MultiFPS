// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"
#include "MenuSystem/MulGameInstance.h"
#include "GameFramework/PlayerController.h"
#include "MenuSystem/ChatWindow.h"
#include "LobbyPlayerController.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (HasAuthority())
	{
		AllPlayerController.Add(NewPlayer);
	}

	NumTotalPlayer++;

	if (NumTotalPlayer >= 2)
	{		
		bUseSeamlessTravel = true;
		
		GetWorld()->GetTimerManager().SetTimer(Handle, this, &ALobbyGameMode::OpenGame, 12.0f, false);
		GetWorld()->GetTimerManager().SetTimer(Count, this, &ALobbyGameMode::CountDown, 1.0f, true, 0.0f);
	}
}
void ALobbyGameMode::CountDown()
{
	if(CountNum > 0)
	{
		CountNum -= 1;
		CountText = FString::Printf(TEXT("Start Game Soon : %d s"), CountNum);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(Count);
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	NumTotalPlayer--;
}


void ALobbyGameMode::OpenGame()
{
	auto GameInstance = Cast<UMulGameInstance>(GetGameInstance());
	GameInstance->StartSession();

	UWorld* World = GetWorld();
	check(World);
	bUseSeamlessTravel = true;
	
	World->ServerTravel("/Game/Cpp_BP/Map/GameMap");
}