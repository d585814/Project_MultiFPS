// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuInterface.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "MulGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MULFPSTEST_API UMulGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:
	UMulGameInstance(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;

	virtual void Shutdown() override;

	virtual void StartGameInstance() override;
		
	UFUNCTION(BlueprintCallable)
	void CreateGameScreen(class AMulFPSTESTCharacter* Player);

	UFUNCTION(BlueprintCallable)
	void RemoveGameScreen();

	UPROPERTY()
	class UGameScreen* GameScreen;

	UFUNCTION(BlueprintCallable)
	void LoadMainMenu();

	void RefreshServer();

	void StartSession();
		

private:
	class UMainMenu* MainMenu;

	TSubclassOf<UUserWidget> GameScreenClass;

	TSubclassOf<UUserWidget> MainMenuClass;

	IOnlineSessionPtr SessionInterface;

	void Host();

	void Join(uint32 Index);

	void CreateSession();

	void OnCreateSessionComplete(FName SessionName, bool Success);

	TSharedPtr <class FOnlineSessionSearch> SessionSearch;

	void OnFindSessionComplete(bool Success);

	void OnDestroySessionComplete(FName SessionName, bool Success);
	
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	FOnlineSessionSettings SessionSetting;
};
