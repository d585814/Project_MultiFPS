// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
//#include "MulController.h"
#include "LobbyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MULFPSTEST_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public: 
	ALobbyPlayerController();

	UFUNCTION(Server, Reliable)
	void GetChatMessage(const FText& TextToSend);

	UFUNCTION(Client, Reliable)
	void UpdateChat(const FText& Text);
	
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY()
	class UChatWindow* ChatWidget;
	
	virtual void Destroyed() override;

	UFUNCTION(Server, Reliable)
	void SettingCountNum();

	UPROPERTY(Replicated)
	FString NumText = "";

	void WidgetToViewport();

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(Replicated)//Replicated
	FText SendText;

	TSubclassOf<class UUserWidget> ChatWidgetClass;  	  	
	
	
};
