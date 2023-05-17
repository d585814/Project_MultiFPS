// Copyright Epic Games, Inc. All Rights Reserved.

#include "MulFPSTESTGameMode.h"
#include "MulFPSTESTCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"



AMulFPSTESTGameMode::AMulFPSTESTGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;		
	}
}

void AMulFPSTESTGameMode::SpawnSelected_Implementation(AController* Controller, TSubclassOf<ACharacter> Character)
{	
	if (Controller == nullptr) return;	
	if (Character == nullptr) return;	
	UWorld* World = GetWorld();	
	
	if (Controller->GetPawn())
	{
		FTransform Trans;
		Trans = Controller->GetPawn()->GetActorTransform();
		Controller->GetPawn()->Destroy();

		FActorSpawnParameters params;
		params.Instigator = GetInstigator();
		params.Owner = this;

		AMulFPSTESTCharacter* Selected =World->SpawnActor<AMulFPSTESTCharacter>(Character, Trans, params);
		Controller->Possess(Selected);
	}
		
}



void AMulFPSTESTGameMode::ReSpawn_Implementation(AController* Controller, TSubclassOf<class ACharacter> cha)
{	
	
	if (Controller != nullptr)
	{
		if (HasAuthority())
		{
			UWorld* World = GetWorld();
			check(World);

			//FVector(0.0f, 0.0f, 50.f), FRotator(0.0f,0.0f,0.0f), FVector(0.0f, 0.0f, 0.0f)
			FActorSpawnParameters params;
			params.Instigator = GetInstigator();
			params.Owner = this;

			AMulFPSTESTCharacter* Player = World->SpawnActor<AMulFPSTESTCharacter>(cha, RespawnTransform, params);
			if (Player != nullptr)
			{
				Controller->Possess(Player);
			}
		}			
	}	
}

//UWorld* AMulFPSTESTGameMode::WorldGet()
//{
//	return this->GetWorld();
//}

