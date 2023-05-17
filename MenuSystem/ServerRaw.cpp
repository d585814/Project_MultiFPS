// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerRaw.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "MainMenu.h"


void UServerRaw::RowSetup(class UMainMenu* Menu, uint32 Index)
{
	Parent = Menu;
	MyIndex = Index;

	if (JoinSessionButton == nullptr) return;
	JoinSessionButton->OnClicked.AddDynamic(this, &UServerRaw::CallJoinSession);
	
}

void UServerRaw::CallJoinSession()
{
	if (Parent != nullptr)
	{		
		Parent->JoinSession(MyIndex);
	}
}