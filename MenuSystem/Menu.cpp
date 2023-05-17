// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu.h"
#include "GameFramework/PlayerController.h"

void UMenu::SetMenuInterface(IMenuInterface* MMenuInterface)
{
	this->MenuInterface = MMenuInterface;
}

void UMenu::Setup()
{
	this->AddToViewport();

	UWorld* World = GetWorld();
	check(World);
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (PlayerController == nullptr) return;

	FInputModeUIOnly InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetWidgetToFocus(this->TakeWidget());
	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;
}

void UMenu::Teardown()
{
	UWorld* World = GetWorld();
	check(World);
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (PlayerController == nullptr) return;

	FInputModeGameOnly InputModeData;
	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = false;

	this->RemoveFromParent();
}


