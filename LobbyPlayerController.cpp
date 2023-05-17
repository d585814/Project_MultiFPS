// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "LobbyGameMode.h"
#include "Net/UnrealNetwork.h"
#include "UObject/ConstructorHelpers.h"
#include "MenuSystem/ChatWindow.h"
#include "Blueprint/UserWidget.h"
#include "MulFPSTESTCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"


ALobbyPlayerController::ALobbyPlayerController()
{
	ConstructorHelpers::FClassFinder<UUserWidget> ChatWidgetClassFinder(TEXT("/Game/Cpp_BP/Widget/Chat/WBP_ChatWindow"));
	if (ChatWidgetClassFinder.Class == nullptr) return;

	ChatWidgetClass = ChatWidgetClassFinder.Class;
	
}

void  ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ChatWidgetClass == nullptr) return;
	ChatWidget = CreateWidget<UChatWindow>(GetWorld(), ChatWidgetClass);
	
	WidgetToViewport();
}

void ALobbyPlayerController::WidgetToViewport()
{
	if (IsLocalController())
	{
		ChatWidget->AddToViewport();
		UWorld* World = GetWorld();
		check(World);
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController == nullptr) return;

		PlayerController->bShowMouseCursor = true;
	}
}

void ALobbyPlayerController::Destroyed()
{
	ChatWidget->RemoveFromParent();
	UWorld* World = GetWorld();
	check(World);
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (PlayerController == nullptr) return;

	PlayerController->bShowMouseCursor = false;
}


void ALobbyPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ALobbyPlayerController, SendText);
	DOREPLIFETIME(ALobbyPlayerController, NumText);
}

void ALobbyPlayerController::GetChatMessage_Implementation(const FText& TextToSend)
{   	
	SendText = TextToSend;
	ALobbyGameMode* Lobby = Cast<ALobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (Lobby == nullptr) return;
	

	for (auto Controller : Lobby->AllPlayerController)
	{
		ALobbyPlayerController* LobbyController = Cast<ALobbyPlayerController>(Controller);
		LobbyController->UpdateChat(SendText);
	}	
}

void ALobbyPlayerController::UpdateChat_Implementation(const FText& Text)
{
	ChatWidget->UpdateChatWindow(Text);
}

void ALobbyPlayerController::SettingCountNum_Implementation()
{
	ALobbyGameMode* GameMode = Cast<ALobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode == nullptr) return;

	NumText = GameMode->GetCountText();
}