// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "Menu.generated.h"

/**
 * 
 */
UCLASS()
class MULFPSTEST_API UMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	void Setup();

	void Teardown();

	void SetMenuInterface(IMenuInterface* MMenuInterface);

protected:

	IMenuInterface* MenuInterface;

	
};
