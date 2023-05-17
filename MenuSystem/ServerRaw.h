// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Menu.h"
#include "ServerRaw.generated.h"

/**
 * 
 */
UCLASS()
class MULFPSTEST_API UServerRaw : public UMenu
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	class UButton* JoinSessionButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ServerNameText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayerNumText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PingText;

	void RowSetup(class UMainMenu* Menu, uint32 Index);
	


private:
	
	UFUNCTION()
	void CallJoinSession();
		
	UPROPERTY()
	class UMainMenu* Parent;

	uint32 MyIndex;
	
};
