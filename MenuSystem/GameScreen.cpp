// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScreen.h"
#include "Components/TextBlock.h"
#include "MulFPSTEST/MulFPSTESTCharacter.h"
#include "MulFPSTEST/WeaponActual_Master.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Texture2D.h"
#include "Components/Image.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "MulFPSTEST/LobbyGameMode.h"


UGameScreen::UGameScreen(const FObjectInitializer& ObjectInitializer)
{
		
	
}

bool UGameScreen::Initialize()
{	
	bool Success = Super::Initialize();
	if (!Success) return false;
			
	if (AmmoText == nullptr) return false;
	AmmoText->TextDelegate.BindDynamic(this, &UGameScreen::GetAmmoText);
		
	if (Health == nullptr) return false;
	Health->TextDelegate.BindDynamic(this, &UGameScreen::GetHealth);
		
	if (WeaponImage == nullptr) return false;
	WeaponImage->BrushDelegate.BindDynamic(this, &UGameScreen::MakeBrush);
	
	return true;
}

FText UGameScreen::GetAmmoText()
{	
	FString Name="";
	FText null = FText::FromString(Name);
	
	if (Player == nullptr) return null;
	if (Player->CurrentWeapon == nullptr) return null;
	

	if (Player->CurrentWeapon->bRifle)
	{
		Name = FString::Printf(TEXT("%d / %d"), Player->GetCurrentAmmo_R(), Player->GetTotalAmmo_R());
		return FText::FromString(Name);
	}
	else if (Player->CurrentWeapon->bRocket)
	{
		Name = FString::Printf(TEXT("%d / %d"), Player->GetCurrentAmmo_Ro(), Player->GetTotalAmmo_Ro());
		return FText::FromString(Name);
	}
	else if (Player->CurrentWeapon->bShotGun)
	{
		Name = FString::Printf(TEXT("%d / %d"), Player->GetCurrentAmmo_S(), Player->GetTotalAmmo_S());
		return FText::FromString(Name);
	}

	return null;
}

FText UGameScreen::GetHealth()
{
	FString mu = "";
	FText null = FText::FromString(mu);
	if (Player == nullptr) return null;

	int32 Num = Player->GetCurrentHealth();
	FString Name = FString::Printf(TEXT("%d"), Num);
	FText CurrentHealth = FText::FromString(Name);
	return CurrentHealth;
}

UTexture2D* UGameScreen::GetImage()
{
	if (Player == nullptr) return nullptr;

	if (Player->CurrentWeapon != nullptr)
	{
		if (Player->CurrentWeapon->Image != nullptr)
		{
			return Player->CurrentWeapon->Image;
		}
	}
	
	return nullptr;
}

FSlateBrush UGameScreen::MakeBrush()
{	
	FSlateBrush Brush = UWidgetBlueprintLibrary::MakeBrushFromTexture(GetImage(), 0, 0);

	return Brush;	
}

void UGameScreen::CastPlayer(class AMulFPSTESTCharacter* Real)
{
	Player = Cast<AMulFPSTESTCharacter>(Real);
}

