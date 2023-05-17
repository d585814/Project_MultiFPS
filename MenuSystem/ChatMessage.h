// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatMessage.generated.h"

/**
 * 
 */
UCLASS()
class MULFPSTEST_API UChatMessage : public UUserWidget
{
	GENERATED_BODY()

public:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	FText GetChatText() { return Text; }

	UFUNCTION()
	void SetChatText(FText New) { Text = New; }

protected:
	bool Initialize() override;

private:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ChatText;

	UPROPERTY(Replicated)
	FText Text;
};
