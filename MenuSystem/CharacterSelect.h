// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Menu.h"
#include "CharacterSelect.generated.h"

/**
 * 
 */
UCLASS()
class MULFPSTEST_API UCharacterSelect : public UMenu
{
	GENERATED_BODY()

public:
	UCharacterSelect(const FObjectInitializer & ObjectInitializer);

protected:
	bool Initialize() override;

private:

	UPROPERTY(meta = (BindWidget))
	class UButton* CharacterButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* Character1Button;

	UPROPERTY(meta = (BindWidget))
	class UButton* Character2Button;

	UPROPERTY(meta = (BindWidget))
	class UButton* Character3Button;

	UPROPERTY(meta = (BindWidget))
	class UButton* Character4Button;

	UPROPERTY(meta = (BindWidget))
	class UButton* SelectButton;

	UFUNCTION()
	void SelectCharacter();

	UFUNCTION()
	void SelectCharacter1();

	UFUNCTION()
	void SelectCharacter2();

	UFUNCTION()
	void SelectCharacter3();

	UFUNCTION()
	void SelectCharacter4();

	UFUNCTION()
	void Select();
		
	TSubclassOf<class ACharacter> CharacterClass;

	TSubclassOf<class ACharacter> CharacterClass1;

	TSubclassOf<class ACharacter> CharacterClass2;

	TSubclassOf<class ACharacter> CharacterClass3;

	TSubclassOf<class ACharacter> CharacterClass4;

	class AMulController* Cont;
};
