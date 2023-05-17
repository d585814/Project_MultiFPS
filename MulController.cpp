// Fill out your copyright notice in the Description page of Project Settings.


#include "MulController.h"
#include "MulFPSTESTGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "MenuSystem/CharacterSelect.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "MulFPSTESTGameMode.h"
#include "Net/UnrealNetwork.h"


AMulController::AMulController(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> CharacterSelectWidgetClassFinder(TEXT("/Game/Cpp_BP/Widget/WBP_CharacterSelect"));
	if (CharacterSelectWidgetClassFinder.Class == nullptr) return;

	SelectWidgetClass = CharacterSelectWidgetClassFinder.Class;
}


void  AMulController::BeginPlay()
{
	Super::BeginPlay();

	if (SelectWidgetClass != nullptr)
	{
		if (IsLocalController())
		{
			SelectWidget = CreateWidget<UCharacterSelect>(GetWorld(), SelectWidgetClass);
			SelectWidget->AddToViewport();
			FInputModeUIOnly InputModeData;
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			InputModeData.SetWidgetToFocus(SelectWidget->TakeWidget());
			this->SetInputMode(InputModeData);
			this->bShowMouseCursor = true;
		}
	}	
}


void AMulController::RequestSpawnSelected_Implementation(TSubclassOf<class ACharacter> Cha)
{	
	SpawnToCharacter = Cha;
	AMulFPSTESTGameMode* GameMode = Cast<AMulFPSTESTGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->SpawnSelected(this, SpawnToCharacter);
	
}


void AMulController::CallCheckController()
{
	UWorld* World = GetWorld();
	FTimerHandle Handle;
	if (World != nullptr)
	{
		World->GetTimerManager().SetTimer(Handle, this, &AMulController::CheckController, 1.0f, true);
	}
}

void AMulController::CheckController()
{	
	if (GetPawn() == nullptr)
	{
		RequestRespawn();
	}	
}

void AMulController::RequestRespawn_Implementation()
{
	if (HasAuthority())
	{
		
		AMulFPSTESTGameMode* GameMode = Cast<AMulFPSTESTGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (GameMode != nullptr)
		{
			GameMode->ReSpawn(this, SpawnToCharacter);
		}
	}	
}