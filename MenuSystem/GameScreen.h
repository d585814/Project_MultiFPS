// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Menu.h"
#include "Components/Widget.h"
#include "Styling/SlateBrush.h"
#include "GameScreen.generated.h"



/**
 * 
 */
UCLASS()
class MULFPSTEST_API UGameScreen : public UMenu
{
	GENERATED_BODY()


public:
	UGameScreen(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintPure)
	FSlateBrush MakeBrush();

	void CastPlayer(class AMulFPSTESTCharacter* Real);
protected:
	bool Initialize() override;

	
private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AmmoText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Health;
	
	UPROPERTY(meta = (BindWidget))
	class UImage* WeaponImage;
	
	UPROPERTY()
	class AMulFPSTESTCharacter* Player;

	UFUNCTION()
	FText GetAmmoText();

	UFUNCTION()
	FText GetHealth();
	
	UFUNCTION()
	class UTexture2D* GetImage();
	
	
};
