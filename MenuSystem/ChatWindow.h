// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Menu.h"
#include "ChatWindow.generated.h"

/**
 * 
 */
UCLASS()
class MULFPSTEST_API UChatWindow : public UMenu
{
	GENERATED_BODY()

public:
	UChatWindow(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(Client, Reliable)
	void UpdateChatWindow(const FText& NewText);

	UFUNCTION(NetMulticast, Reliable)
	void MultiRemove();
		
protected:
	bool Initialize() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ChatLog;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* ChatEnter;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CountDown;
	
	TSubclassOf<class UUserWidget> MessageClass;

	UFUNCTION()
	void OnChatTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	
	UFUNCTION()
	FText CheckStart();
	
	FTimerHandle Handle;
	
};
