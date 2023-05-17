// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatMessage.h"
#include "Components/TextBlock.h"
#include "Net/UnrealNetwork.h"

void UChatMessage::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UChatMessage, Text);
}

bool UChatMessage::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (ChatText == nullptr) return false;
	ChatText->TextDelegate.BindDynamic(this, &UChatMessage::GetChatText);

	return true;
}
