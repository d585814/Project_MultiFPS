// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "ServerRaw.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "MulFPSTEST/LobbyPlayerController.h"

UMainMenu::UMainMenu(const FObjectInitializer & ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowWidgetClass(TEXT("/Game/Cpp_BP/Widget/WBP_ServerRow"));
	if (ServerRowWidgetClass.Class == nullptr) return;

	ServerRowWidget = ServerRowWidgetClass.Class;
	
}
bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if(SoloGameButton == nullptr) return false;
	SoloGameButton->OnClicked.AddDynamic(this, &UMainMenu::SoloGame);

	if (MainOpenMultiMenuButton == nullptr) return false;
	MainOpenMultiMenuButton->OnClicked.AddDynamic(this, &UMainMenu::MainOpenMultiMenu);

	if (QuitButton == nullptr) return false;
	QuitButton->OnClicked.AddDynamic(this, &UMainMenu::Quit);

	if (CreateSessionButton == nullptr) return false;
	CreateSessionButton->OnClicked.AddDynamic(this, &UMainMenu::CreateSession);
	
	if (OpenJoinMenuButton == nullptr) return false;
	OpenJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if (OpenMainMenuButton == nullptr) return false;
	OpenMainMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);
	
	if (JoinOpenMultiMenuButton == nullptr) return false;
	JoinOpenMultiMenuButton->OnClicked.AddDynamic(this, &UMainMenu::JoinOpenMultiMenu);
	
	return true;
}

void UMainMenu::SoloGame()
{
	UWorld* World = GetWorld();
	check(World);
	//World->ServerTravel("/Game/Cpp_BP/Map/GameMap");
	Teardown();
	UGameplayStatics::OpenLevel(World, "/Game/Cpp_BP/Map/SoloMap");
}

void UMainMenu::MainOpenMultiMenu()
{
	if (MenuSwitcher == nullptr) return;
	if (MultiMenu == nullptr) return;

	MenuSwitcher->SetActiveWidget(MultiMenu);
}

void UMainMenu::Quit()
{
	UWorld* World = GetWorld();
	check(World);
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (PlayerController == nullptr) return;

	PlayerController->ConsoleCommand(FString("quit"));
}

void UMainMenu::CreateSession()
{
	MenuInterface->Host();
	Teardown(); 
}

void UMainMenu::SetServerList(TArray<FServerData> ServerNames)
{
	UWorld* World = GetWorld();
	check(World);

	Serverlist->ClearChildren();

	uint32 i = 0;
	for (const FServerData& ServerData : ServerNames)
	{
		UServerRaw* Row = CreateWidget<UServerRaw>(GetWorld(), ServerRowWidget);
		if (Row == nullptr) return;

		Row->ServerNameText->SetText(FText::FromString(ServerData.Name));
		Row->PingText->SetText(FText::FromString(FString::Printf(TEXT("%d ms"),ServerData.Ping)));
		FString CurrentInfo = FString::Printf(TEXT("%d / %d"), ServerData.CurrentPlayer , ServerData.MaxPlayer );
		Row->PlayerNumText->SetText(FText::FromString(CurrentInfo));
		Row->RowSetup(this, i);
		++i;

		Serverlist->AddChild(Row);
	}
}

void UMainMenu::OpenJoinMenu()
{
	if (MenuSwitcher == nullptr) return;
	if (JoinMenu == nullptr) return;

	MenuInterface->RefreshServer();
	
	MenuSwitcher->SetActiveWidget(JoinMenu);
}

void UMainMenu::JoinSession(uint32 Index)
{
	MenuInterface->Join(Index);
}

void UMainMenu::OpenMainMenu()
{
	if (MenuSwitcher == nullptr) return;
	if (MainMenu == nullptr) return;

	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::JoinOpenMultiMenu()
{
	if (MenuSwitcher == nullptr) return;
	if (MultiMenu == nullptr) return;

	MenuSwitcher->SetActiveWidget(MultiMenu);
}