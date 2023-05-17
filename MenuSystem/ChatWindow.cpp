// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatWindow.h"
#include "Components/EditableTextBox.h"
#include "UObject/ConstructorHelpers.h"
#include "ChatMessage.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "Kismet/GameplayStatics.h"
#include "MulFPSTEST/LobbyPlayerController.h"
#include "MulFPSTEST/LobbyGameMode.h"
#include "Components/TextBlock.h"
#include "Math/UnrealMathUtility.h"


UChatWindow::UChatWindow(const FObjectInitializer& ObjectInitializer)
{

	ConstructorHelpers::FClassFinder<UUserWidget> ChatMessageWidgetClassFinder(TEXT("/Game/Cpp_BP/Widget/Chat/WBP_ChatMessage"));
	if (ChatMessageWidgetClassFinder.Class == nullptr) return;

	MessageClass = ChatMessageWidgetClassFinder.Class;
	
}

bool UChatWindow::Initialize()
{	
	bool Success = Super::Initialize();
	if (!Success) return false;
	
	if (ChatEnter == nullptr) return false;
	ChatEnter->OnTextCommitted.AddDynamic(this, &UChatWindow::OnChatTextCommitted);

	if (CountDown == nullptr) return false;
	CountDown->TextDelegate.BindDynamic(this, &UChatWindow::CheckStart);

	return true;
}


void UChatWindow::UpdateChatWindow_Implementation(const FText& NewText)
{
	UChatMessage* ChatText = CreateWidget<UChatMessage>(GetWorld(), MessageClass);
	ChatText->SetChatText(NewText);
	ChatLog->AddChild(ChatText);
	ChatLog->ScrollToEnd();

}

void UChatWindow::OnChatTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{	
	if (ETextCommit::OnEnter)
	{		
		ALobbyPlayerController* Controller = Cast<ALobbyPlayerController>(UUserWidget::GetOwningPlayer());
		Controller->GetChatMessage(Text);
		FText Empty= FText::FromString(FString::Printf(TEXT("")));
		ChatEnter->SetText(Empty);
	}	
}

void UChatWindow::MultiRemove_Implementation()
{
	Teardown();
}


FText UChatWindow::CheckStart()
{
	ALobbyPlayerController* Cont = Cast<ALobbyPlayerController>(GetOwningPlayer());
	if (Cont == nullptr) return FText::FromString("Fail!!!");
	Cont->SettingCountNum();
	return FText::FromString(Cont->NumText);
}


