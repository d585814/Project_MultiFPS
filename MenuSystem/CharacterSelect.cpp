// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelect.h"
#include "Components/Button.h"
#include "GameFramework/Character.h"
#include "MulFPSTEST/MulFPSTESTCharacter.h"
#include "MulFPSTEST/MulController.h"

UCharacterSelect::UCharacterSelect(const FObjectInitializer & ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<ACharacter> Character0Class(TEXT("/Game/ThirdPersonCPP/Blueprints/Character"));
	if (Character0Class.Class == nullptr) return;
	CharacterClass = Character0Class.Class;

	ConstructorHelpers::FClassFinder<ACharacter> Character1Class(TEXT("/Game/ThirdPersonCPP/Blueprints/Character1"));
	if (Character1Class.Class == nullptr) return;
	CharacterClass1 = Character1Class.Class;

	ConstructorHelpers::FClassFinder<ACharacter> Character2Class(TEXT("/Game/ThirdPersonCPP/Blueprints/Character2"));
	if (Character2Class.Class == nullptr) return;
	CharacterClass2 = Character2Class.Class;

	ConstructorHelpers::FClassFinder<ACharacter> Character3Class(TEXT("/Game/ThirdPersonCPP/Blueprints/Character3"));
	if (Character3Class.Class == nullptr) return;
	CharacterClass3 = Character3Class.Class;

	ConstructorHelpers::FClassFinder<ACharacter> Character4Class(TEXT("/Game/ThirdPersonCPP/Blueprints/Character4"));
	if (Character4Class.Class == nullptr) return;
	CharacterClass4 = Character4Class.Class;

	


}

bool UCharacterSelect::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	Cont = Cast<AMulController>(GetOwningPlayer());
	if (Cont == nullptr) return false;

	//CharacterButton Setting
	if (CharacterButton == nullptr) return false;
	CharacterButton->OnClicked.AddDynamic(this, &UCharacterSelect::SelectCharacter);

	if (Character1Button == nullptr) return false;
	Character1Button->OnClicked.AddDynamic(this, &UCharacterSelect::SelectCharacter1);

	if (Character2Button == nullptr) return false;
	Character2Button->OnClicked.AddDynamic(this, &UCharacterSelect::SelectCharacter2);

	if (Character3Button == nullptr) return false;
	Character3Button->OnClicked.AddDynamic(this, &UCharacterSelect::SelectCharacter3);

	if (Character4Button == nullptr) return false;
	Character4Button->OnClicked.AddDynamic(this, &UCharacterSelect::SelectCharacter4);

	//--------------------------------------------------------------------------------
	if (SelectButton == nullptr) return false;
	SelectButton->OnClicked.AddDynamic(this, &UCharacterSelect::Select);

	return true;
}

void UCharacterSelect::SelectCharacter()
{
	if (CharacterClass == nullptr) return;
	Cont->RequestSpawnSelected(CharacterClass);
}

void UCharacterSelect::SelectCharacter1()
{
	if (CharacterClass1 == nullptr) return;
	Cont->RequestSpawnSelected(CharacterClass1);
}

void UCharacterSelect::SelectCharacter2()
{
	if (CharacterClass2 == nullptr) return;
	Cont->RequestSpawnSelected(CharacterClass2);
}

void UCharacterSelect::SelectCharacter3()
{
	if (CharacterClass3 == nullptr) return;
	Cont->RequestSpawnSelected(CharacterClass3);
}

void UCharacterSelect::SelectCharacter4()
{
	if (CharacterClass4 == nullptr) return;
	Cont->RequestSpawnSelected(CharacterClass4);
}

void UCharacterSelect::Select()
{	
	Cont->CallCheckController();
	FInputModeGameOnly InputModeData;
	Cont->SetInputMode(InputModeData);
	Cont->bShowMouseCursor = false;
	this->RemoveFromParent();
}