// Fill out your copyright notice in the Description page of Project Settings.


#include "MulGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "GameScreen.h"
#include "MainMenu.h"
#include "MulFPSTEST/MulFPSTESTCharacter.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"
#include "MulFPSTEST/LobbyGameMode.h"
#include "Kismet/GameplayStatics.h"


const static FName SESSION_NAME = TEXT("Game");
const static FName SERVER_NAME_SETTING_KEY = TEXT("ServerName");

UMulGameInstance::UMulGameInstance(const FObjectInitializer& ObjectInitializer)
{	
	ConstructorHelpers::FClassFinder<UUserWidget> GameScreenWidgetClassFinder(TEXT("/Game/Cpp_BP/Widget/ClientHud"));
	if (GameScreenWidgetClassFinder.Class == nullptr) return;

	GameScreenClass = GameScreenWidgetClassFinder.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> MainMenuWidgetClassFinder(TEXT("/Game/Cpp_BP/Widget/WBP_MainMenu"));
	if (MainMenuWidgetClassFinder.Class == nullptr) return;

	MainMenuClass = MainMenuWidgetClassFinder.Class;	
}

void UMulGameInstance::Init()
{	
	IOnlineSubsystem* Online = IOnlineSubsystem::Get();
	if (Online != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("success : %s"), *Online->GetSubsystemName().ToString());
		SessionInterface = Online->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UMulGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UMulGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UMulGameInstance::OnFindSessionComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UMulGameInstance::OnJoinSessionComplete);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Found OnlineSubsystem"));
	}
	
}

void UMulGameInstance::Shutdown()
{
	Super::Shutdown();
}

void UMulGameInstance::StartGameInstance()
{
	Super::StartGameInstance();
}

void UMulGameInstance::LoadMainMenu()
{
	if (MainMenuClass == nullptr) return;
	MainMenu = CreateWidget<UMainMenu>(this, MainMenuClass);
	MainMenu->Setup();
	MainMenu->SetMenuInterface(this);
}


void UMulGameInstance::CreateGameScreen(class AMulFPSTESTCharacter* Player)
{		
	if(GameScreenClass == nullptr) return;
	GameScreen = CreateWidget<UGameScreen>(this, GameScreenClass);
	GameScreen->CastPlayer(Player);
	GameScreen->AddToViewport();
	GameScreen->SetMenuInterface(this);
	
}

void UMulGameInstance::RemoveGameScreen()
{
	if (GameScreen != nullptr)
	{
		GameScreen->RemoveFromParent();
	}
}

void UMulGameInstance::Host()
{
	if (SessionInterface.IsValid())
	{
		CreateSession();
		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (ExistingSession != nullptr)
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			CreateSession();
		}
	}
	
}
void UMulGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Fail Destroy the Session"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Destroy Session"));
	CreateSession();

}

void UMulGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{				
		SessionSetting.bIsLANMatch = true;
		SessionSetting.NumPublicConnections = 3;
		SessionSetting.bShouldAdvertise = true;
				
		SessionInterface->CreateSession(0, SESSION_NAME, SessionSetting);
	}
}

void UMulGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Fail Create the Session"));
		return;
	}
	
	UWorld* World = GetWorld();
	check(World);

	World->ServerTravel("/Game/Cpp_BP/Map/LobbyMap?listen");
}

void UMulGameInstance::RefreshServer()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->MaxSearchResults = 100;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	if (SessionSearch.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("---Start Find Session---"));
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UMulGameInstance::OnFindSessionComplete(bool Success)
{
	if (Success && SessionSearch.IsValid() && MainMenu != nullptr)
	{
		if (!SessionInterface->UpdateSession(SESSION_NAME, SessionSetting, true)) return;
		TArray<FServerData> ServerNames;
		FString* null=nullptr;
		for (const FOnlineSessionSearchResult& Results : SessionSearch->SearchResults)
		{
			UE_LOG(LogTemp, Warning, TEXT("Find Session ID:%s"), *Results.GetSessionIdStr());
			FServerData Data;

			Data.MaxPlayer = Results.Session.SessionSettings.NumPublicConnections ;
			Data.CurrentPlayer = Data.MaxPlayer - Results.Session.NumOpenPublicConnections +1;
			Data.Ping = Results.PingInMs;
			//Data.Name = Results.Session.OwningUserName;
			Results.Session.OwningUserName.Split("-", &Data.Name,  null, ESearchCase::IgnoreCase, ESearchDir::FromStart);
			
			ServerNames.Add(Data);
		}

		MainMenu->SetServerList(ServerNames);
	}
}

void UMulGameInstance::Join(uint32 Index)
{
	if (!SessionInterface.IsValid()) return;

	if (MainMenu != nullptr)
	{
		MainMenu->Teardown();
	}

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
}

void UMulGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	APlayerController* Controller = GetFirstLocalPlayerController();
	FString URL;
	if (Controller != nullptr && SessionInterface->GetResolvedConnectString(SessionName, URL))
	{
		Controller->ClientTravel(URL, ETravelType::TRAVEL_Absolute);
	}
}

void UMulGameInstance::StartSession()
{
	SessionInterface->StartSession(SESSION_NAME);
}

