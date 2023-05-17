// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Menu.h"
#include "MainMenu.generated.h"

USTRUCT()
struct FServerData
{
	GENERATED_BODY()

	FString Name;
	uint16 CurrentPlayer;
	uint16 MaxPlayer;
	uint16 Ping;
};
UCLASS()
class MULFPSTEST_API UMainMenu : public UMenu
{
	GENERATED_BODY()
public:
	UMainMenu(const FObjectInitializer & ObjectInitializer);

	UFUNCTION()
	void SetServerList(TArray<FServerData> ServerNames);

	UFUNCTION()
	void JoinSession(uint32 Index);

protected:
	bool Initialize() override;

private:

	UPROPERTY(meta = (BindWidget))
	class UButton* SoloGameButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* MainOpenMultiMenuButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* CreateSessionButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* OpenJoinMenuButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* OpenMainMenuButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinOpenMultiMenuButton;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* Serverlist;

	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* MultiMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* JoinMenu;

	UFUNCTION()
	void SoloGame();
	
	UFUNCTION()
	void MainOpenMultiMenu();

	UFUNCTION()
	void Quit();

	UFUNCTION()
	void CreateSession();

	UFUNCTION()
	void OpenJoinMenu();

	UPROPERTY()
	TSubclassOf<class UUserWidget> ServerRowWidget;

	UFUNCTION()
	void OpenMainMenu();

	UFUNCTION()
	void JoinOpenMultiMenu();
	
};
