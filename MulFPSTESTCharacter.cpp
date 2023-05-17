// Copyright Epic Games, Inc. All Rights Reserved.

#include "MulFPSTESTCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "WeaponPickup_Master.h"
#include "WeaponActual_Master.h"
#include "Containers/Array.h"
#include "DrawDebugHelpers.h"
#include "Projectile.h"
#include "RocketProjectile.h"
#include "Animation/AnimSequence.h"
#include "MenuSystem/MulGameInstance.h"
#include "Kismet/GameplayStatics.h"


//////////////////////////////////////////////////////////////////////////
// AMulFPSTESTCharacter

AMulFPSTESTCharacter::AMulFPSTESTCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	WeaponIndex = 0;
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	

	//Setting Collsion preset( Mesh, Capsule)
	GetMesh()->SetCollisionProfileName(FName("MyCharacterMeshColl"));
	GetCapsuleComponent()->SetCollisionProfileName(FName("CapsuleColl"));

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	MaxHealth = 100;
	CurrentHealth = MaxHealth;

	TotalAmmo_R = 60;
	CurrentAmmo_R = 20;

	TotalAmmo_S = 16;
	CurrentAmmo_S = 8;

	TotalAmmo_Ro = 5;
	CurrentAmmo_Ro = 1;

}


void AMulFPSTESTCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMulFPSTESTCharacter, LookUpAxis);
	DOREPLIFETIME(AMulFPSTESTCharacter, TurnAxis);
	DOREPLIFETIME(AMulFPSTESTCharacter, SpawnedActor);
	DOREPLIFETIME(AMulFPSTESTCharacter, CanFire);
	DOREPLIFETIME(AMulFPSTESTCharacter, CurrentWeapon);
	DOREPLIFETIME(AMulFPSTESTCharacter, CurrentHealth); 
	DOREPLIFETIME(AMulFPSTESTCharacter, CurrentAmmo_R);
	DOREPLIFETIME(AMulFPSTESTCharacter, TotalAmmo_R);
	DOREPLIFETIME(AMulFPSTESTCharacter, CurrentAmmo_Ro);
	DOREPLIFETIME(AMulFPSTESTCharacter, TotalAmmo_Ro);
	DOREPLIFETIME(AMulFPSTESTCharacter, CurrentAmmo_S);
	DOREPLIFETIME(AMulFPSTESTCharacter, TotalAmmo_S); 
	DOREPLIFETIME(AMulFPSTESTCharacter, CombatMode);	
	DOREPLIFETIME(AMulFPSTESTCharacter, IsReload);	
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMulFPSTESTCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping); 

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AMulFPSTESTCharacter::Interact);
	PlayerInputComponent->BindAction("Check", IE_Pressed, this, &AMulFPSTESTCharacter::CheckInventory);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMulFPSTESTCharacter::Fire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AMulFPSTESTCharacter::ToggleCanFire);
	PlayerInputComponent->BindAction("Combat", IE_Pressed, this, &AMulFPSTESTCharacter::CallToggleCombatMode);

	PlayerInputComponent->BindAction("IncrementInventory", IE_Pressed, this, &AMulFPSTESTCharacter::IncrementInventory);
	PlayerInputComponent->BindAction("DecrementInventory", IE_Pressed, this, &AMulFPSTESTCharacter::DecrementInventory);
	

	PlayerInputComponent->BindAxis("MoveForward", this, &AMulFPSTESTCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMulFPSTESTCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	
	PlayerInputComponent->BindAxis("Turn", this, &AMulFPSTESTCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AMulFPSTESTCharacter::LookUp);
	
}



void AMulFPSTESTCharacter::Turn(float Rate)
{	
	AddControllerYawInput(Rate);
	float Test = UKismetMathLibrary::NormalizedDeltaRotator(GetControlRotation(), GetActorRotation()).Yaw;
	TurnToServer(Test);
}

void AMulFPSTESTCharacter::LookUp(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate);
	float Test = UKismetMathLibrary::NormalizedDeltaRotator(GetControlRotation(), GetActorRotation()).Pitch;
	LookUpToServer(Test);
}

void AMulFPSTESTCharacter::LookUpToServer_Implementation(float NewLookUpAxis)
{
	LookUpAxis = NewLookUpAxis;
}

void AMulFPSTESTCharacter::TurnToServer_Implementation(float NewTurnAxis)
{
	TurnAxis = NewTurnAxis;
}

void AMulFPSTESTCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);

		//ForwardValue = Value;
	}
}

void AMulFPSTESTCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);

		//RightValue = Value;
	}
}

void AMulFPSTESTCharacter::IncrementInventory()
{
	if (WeaponInventory.Num() > 1)
	{
		if (WeaponIndex >= (WeaponInventory.Num() - 1))
		{
			WeaponIndex = 0;
			WeaponToSpawn = WeaponInventory[WeaponIndex];
			CallSpawnWeapon(WeaponToSpawn);
		}
		else
		{
			WeaponIndex++;
			WeaponToSpawn = WeaponInventory[WeaponIndex];
			CallSpawnWeapon(WeaponToSpawn);
		}
	}
	
}

void AMulFPSTESTCharacter::DecrementInventory()
{
	if (WeaponInventory.Num() > 1)
	{
		if (WeaponIndex == 0)
		{
			WeaponIndex = WeaponInventory.Num() - 1;
			WeaponToSpawn = WeaponInventory[WeaponIndex];
			CallSpawnWeapon(WeaponToSpawn);
		}
		else
		{
			WeaponIndex--;
			WeaponToSpawn = WeaponInventory[WeaponIndex];
			CallSpawnWeapon(WeaponToSpawn);
		}
	}
}



void AMulFPSTESTCharacter::Interact()
{
	TArray<AActor*> CollectedActors;	
	GetCapsuleComponent()->GetOverlappingActors(CollectedActors);
	
	for (const auto& WeaponCollection : CollectedActors)
	{
		AWeaponPickup_Master* Weapon = Cast<AWeaponPickup_Master>(WeaponCollection);
		if (Weapon != nullptr)
		{			
			if (Weapon->ActualWeaponClass)
			{	
				int32 Before_Inventroy = WeaponInventory.Num();
				WeaponInventory.AddUnique(Weapon->ActualWeaponClass);
				int32 After_Inventroy = WeaponInventory.Num();

				if (Before_Inventroy == After_Inventroy)
				{
					UE_LOG(LogTemp, Warning, TEXT("Did not Add to!!!"));
					return;
				}
												
				else
				{				
					OnServerDestroy(Weapon);
					WeaponIndex = WeaponInventory.Num() - 1;
					WeaponToSpawn = WeaponInventory[WeaponIndex];
					CallSpawnWeapon(WeaponToSpawn);											
				}										
			}
		}
	}
	

}

void AMulFPSTESTCharacter::OnServerDestroy_Implementation(AActor* ActorToDestroy)
{
	ActorToDestroy->Destroy();
}

void AMulFPSTESTCharacter::CallSpawnWeapon(TSubclassOf<AWeaponActual_Master> Weapon)
{		
	if (!Weapon) return;
	

	if (SpawnedActor != nullptr)
	{
		OnServerDestroy(SpawnedActor);
		OnServerSpawnWeapon(Weapon);				
	}
	else
	{
		OnServerSpawnWeapon(Weapon);
	}
}

void AMulFPSTESTCharacter::OnServerSpawnWeapon_Implementation(TSubclassOf<AActor> ActorToSpawn)
{	
	if (!ActorToSpawn) return;

	UWorld* World = GetWorld();
	check(World);

	//Setting To SpawnActor
	FTransform Trans = GetMesh()->GetSocketTransform("WeaponSocket");
	FActorSpawnParameters params;
	params.Instigator = GetInstigator();
	params.Owner = this;
	

	SpawnedActor = World->SpawnActor<AActor>(ActorToSpawn, Trans, params);
	SpawnedActor->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "WeaponSocket");	
	CurrentWeapon = Cast<AWeaponActual_Master>(SpawnedActor);	

}


void AMulFPSTESTCharacter::CheckInventory()
{
	
	for (const auto& Inventory : WeaponInventory)
	{		
		if (Inventory == nullptr)return;
		UE_LOG(LogTemp, Warning, TEXT("Item : %s"), *Inventory->GetName());
	}
}
void AMulFPSTESTCharacter::Fire()
{
	if (SpawnedActor != nullptr)
	{		
		ToggleCanFire();
		if(CurrentWeapon==nullptr) return;
								
		SpawnProjectile(this);	
	}
}

void AMulFPSTESTCharacter::ToggleCanFire_Implementation()
{	
	if (CurrentWeapon == nullptr)return;

	if (CombatMode)
	{
		if (!CanFire)
		{
			CanFire = true;
		}
		else
		{
			CanFire = false;
			if (CurrentWeapon->bRifle)
			{
				GetWorld()->GetTimerManager().ClearTimer(Handle);
			}
		}
	}
}
void AMulFPSTESTCharacter::CallToggleCombatMode_Implementation()
{
	ToggleCombatMode();
}

void AMulFPSTESTCharacter::ToggleCombatMode_Implementation()//_Implementation
{	
	if (CurrentWeapon == nullptr) return;

	if (!CombatMode)
	{
		CombatMode = true;		
	}

	else
	{
		CombatMode = false;		
	}	
	CombatModeWidget();
	
}
void AMulFPSTESTCharacter::OnRep_CombatMode()
{
	CombatModeWidget();
}

void AMulFPSTESTCharacter::CombatModeWidget()
{	
	if (IsLocallyControlled())
	{
		GameInstance = Cast<UMulGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		if (GameInstance == nullptr) return;

		if (CombatMode)
		{
			GameInstance->CreateGameScreen(this);
		}
		else
		{
			GameInstance->RemoveGameScreen();
		}
	}
	
}

void AMulFPSTESTCharacter::SpawnProjectile_Implementation(APawn* Pawn)
{
	if (ProjectileClass != NULL && RocketProjectileClass !=NULL)
	{
		if (CanFire)
		{
			UWorld* World = GetWorld();
			check(World);
			check(CurrentWeapon);

			FActorSpawnParameters params;
			params.Instigator = Pawn;
			params.Owner = SpawnedActor;

			// Spawn Projectile and AmmoSystem in RifleMode
			if (CurrentWeapon->bRifle)
			{
				if (CurrentAmmo_R >= 1) //CurrentAmmo_R
				{
					World->SpawnActor<AProjectile>(ProjectileClass, CalculateFireTo(), params);
					CurrentAmmo_R -= 1;
					GunAnimation();

					// Reload						
					if (CurrentAmmo_R <1 && TotalAmmo_R > 0)
					{
						IsReload = true;
						FTimerHandle RifleH;
						FTimerHandle Reload;
						GetWorld()->GetTimerManager().SetTimer(Reload, this, &AMulFPSTESTCharacter::GunReloadAnimation, 0.1f, false);
						GetWorld()->GetTimerManager().SetTimer(RifleH, this, &AMulFPSTESTCharacter::ReloadAmmo, 2.0f, false);
					}

					//AutoFire					
					FTimerDelegate Function = FTimerDelegate::CreateUObject(this, &AMulFPSTESTCharacter::SpawnProjectile, Pawn);
					World->GetTimerManager().SetTimer(Handle, Function, 0.1f, true);					
				}
			
			}
			// Spawn Projectile and AmmoSystem in ShotGun
			else if (CurrentWeapon->bShotGun)
			{
				if (CurrentAmmo_S >= 1)
				{
					for (int i = 0; i < 8; i++)
					{
						World->SpawnActor<AProjectile>(ProjectileClass, CalculateFireTo(), params);
					}
					CurrentAmmo_S -= 1;
					GunAnimation();

					// Reload
					if (CurrentAmmo_S < 1 && TotalAmmo_S > 0)
					{
						IsReload = true;
						FTimerHandle ShotGunH;
						FTimerHandle Reload;
						GetWorld()->GetTimerManager().SetTimer(Reload, this, &AMulFPSTESTCharacter::GunReloadAnimation, 0.2f, false);
						GetWorld()->GetTimerManager().SetTimer(ShotGunH, this, &AMulFPSTESTCharacter::ReloadAmmo, 2.0f, false);
					}
					
				}
			}
			else if (CurrentWeapon->bRocket)
			{				
				if (CurrentAmmo_Ro >= 1) 
				{					
					World->SpawnActor<ARocketProjectile>(RocketProjectileClass, CalculateFireTo(), params);
					CurrentAmmo_Ro -= 1;
					GunAnimation();

					// Reload
					if (CurrentAmmo_Ro < 1 && TotalAmmo_Ro > 0)
					{
						IsReload = true;
						FTimerHandle RifleH;
						FTimerHandle Reload;
						GetWorld()->GetTimerManager().SetTimer(Reload, this, &AMulFPSTESTCharacter::GunReloadAnimation, 0.1f, false);
						GetWorld()->GetTimerManager().SetTimer(RifleH, this, &AMulFPSTESTCharacter::ReloadAmmo, 2.0f, false);
					}
				}
			}
		}
	}
}

void AMulFPSTESTCharacter::ReloadAmmo()
{
	if (HasAuthority())
	{		
		if (CurrentWeapon->bRifle)
		{		
			IsReload = false;
			CurrentAmmo_R += 20;
			TotalAmmo_R = FMath::Clamp(TotalAmmo_R - 20, 0, 500);
		}
		else if (CurrentWeapon->bShotGun)
		{
			IsReload = false;
			CurrentAmmo_S += 8;
			TotalAmmo_S = FMath::Clamp(TotalAmmo_S - 8, 0, 500);
		}
		else if (CurrentWeapon->bRocket)
		{
			IsReload = false;
			CurrentAmmo_Ro += 1;
			TotalAmmo_Ro = FMath::Clamp(TotalAmmo_Ro - 1, 0, 500);
		}
	}
}

FTransform AMulFPSTESTCharacter::CalculateFireTo()
{
	if (HasAuthority())
	{
		UWorld* World = GetWorld();
		check(World);

		FVector Start = FollowCamera->GetComponentLocation();
		FVector End = Start + (FollowCamera->GetForwardVector() * 10000.f);
		FVector ShotGunEnd = Start + (FollowCamera->GetForwardVector() * 700.f) + FVector(FMath::RandRange(-100, 100), FMath::RandRange(-100, 100), FMath::RandRange(-100, 100));
		FHitResult Hit;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		World->LineTraceSingleByChannel(Hit, Start, End, ECollisionChannel::ECC_GameTraceChannel4, Params);

		//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 5.0f, 0, 2.0f);
		UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *Hit.GetActor()->GetName());

		if (CurrentWeapon != nullptr)
		{
			FVector FireLoc = CurrentWeapon->SkeletalMesh->GetSocketLocation("MuzzleFlash");
			FVector Scale = FVector(1.0f, 1.0f, 1.0f);

			if (CurrentWeapon->bRifle || CurrentWeapon->bRocket)
			{
				if (Hit.bBlockingHit)
				{
					FRotator Direction = UKismetMathLibrary::FindLookAtRotation(FireLoc, Hit.ImpactPoint);
					FireTo = UKismetMathLibrary::MakeTransform(FireLoc, Direction, Scale);
				}
				else
				{
					FRotator Direction = UKismetMathLibrary::FindLookAtRotation(FireLoc, End);
					FireTo = UKismetMathLibrary::MakeTransform(FireLoc, Direction, Scale);
				}
			}
			else if (CurrentWeapon->bShotGun)
			{
				FRotator Direction = UKismetMathLibrary::FindLookAtRotation(FireLoc, ShotGunEnd);
				FireTo = UKismetMathLibrary::MakeTransform(FireLoc, Direction, Scale);
			}
			return FireTo;
		}
		return FireTo;
	}
	return FireTo;
}



void AMulFPSTESTCharacter::CallGunAnimation_Implementation()
{
	GunAnimation();
}

void AMulFPSTESTCharacter::GunAnimation_Implementation()
{
	if (CurrentWeapon != nullptr)
	{			
		UAnimSequence* AnimRifle = LoadObject<UAnimSequence>(NULL, TEXT("AnimSequence'/Game/MilitaryWeapDark/Weapons/Anims/Fire_Rifle_W.Fire_Rifle_W'"));
		UAnimSequence* AnimShouGun = LoadObject<UAnimSequence>(NULL, TEXT("AnimSequence'/Game/MilitaryWeapDark/Weapons/Anims/Fire_Shotgun_W.Fire_Shotgun_W'"));
		UAnimSequence* AnimRocket = LoadObject<UAnimSequence>(NULL, TEXT("AnimSequence'/Game/MilitaryWeapDark/Weapons/Anims/Fire_RocketLauncher_W.Fire_RocketLauncher_W'"));
		check(AnimRifle);
		check(AnimShouGun);
		check(AnimRocket);
		if (CurrentWeapon->bRifle)
		{
			CurrentWeapon->SkeletalMesh->PlayAnimation(AnimRifle, false);
		}

		else if(CurrentWeapon->bShotGun)
		{
			CurrentWeapon->SkeletalMesh->PlayAnimation(AnimShouGun, false);
		}
		else if (CurrentWeapon->bRocket)
		{
			CurrentWeapon->SkeletalMesh->PlayAnimation(AnimRocket, false);
		}		
	}
}
void AMulFPSTESTCharacter::GunReloadAnimation_Implementation()
{
	if (CurrentWeapon != nullptr)
	{
		UAnimSequence* ReloadRifle = LoadObject<UAnimSequence>(NULL, TEXT("AnimSequence'/Game/MilitaryWeapDark/Weapons/Anims/Reload_Rifle_Ironsights_W.Reload_Rifle_Ironsights_W'"));
		UAnimSequence* ReloadShouGun = LoadObject<UAnimSequence>(NULL, TEXT("AnimSequence'/Game/MilitaryWeapDark/Weapons/Anims/Reload_Shotgun_Ironsights_W.Reload_Shotgun_Ironsights_W'"));
		UAnimSequence* ReloadRocket = LoadObject<UAnimSequence>(NULL, TEXT("AnimSequence'/Game/MilitaryWeapDark/Weapons/Anims/Reload_RocketLauncher_W.Reload_RocketLauncher_W'"));
		check(ReloadRifle);
		check(ReloadShouGun);
		check(ReloadRocket);
		if (CurrentWeapon->bRifle)
		{
			CurrentWeapon->SkeletalMesh->PlayAnimation(ReloadRifle, false);
		}

		else if (CurrentWeapon->bShotGun)
		{
			CurrentWeapon->SkeletalMesh->PlayAnimation(ReloadShouGun, false);
		}
		else if (CurrentWeapon->bRocket)
		{
			CurrentWeapon->SkeletalMesh->PlayAnimation(ReloadRocket, false);
		}
	}
}


void AMulFPSTESTCharacter::OnRep_CurrentHealth()
{	
	HealthUpdate();		
}

void AMulFPSTESTCharacter::HealthUpdate()
{	
	if (IsLocallyControlled())
	{
		if (CurrentHealth <= 0)
		{			
			if (CombatMode)
			{
				GameInstance->RemoveGameScreen();
			}
		}
	}
	//모든 머신에서 실행되는 함수
	if (CurrentHealth <= 0)
	{
		GetMesh()->SetSimulatePhysics(true);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		UWorld* World = GetWorld();
		if (World == nullptr) return;

		FTimerHandle DeathHandle;
		FTimerDelegate DeathDelegate;
		DeathDelegate.BindUFunction(this, "OnServerDestroy", this);
		World->GetTimerManager().SetTimer(DeathHandle, DeathDelegate, 3.0f, false);
	}
	
}


void AMulFPSTESTCharacter::SetCurrentHealth(float healthValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		CurrentHealth = FMath::Clamp(healthValue, 0.f, MaxHealth);
		HealthUpdate();
	}
}

float AMulFPSTESTCharacter::TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{	
	float damageApplied = CurrentHealth - DamageTaken;
	SetCurrentHealth(damageApplied);
	return damageApplied;
}

int32 AMulFPSTESTCharacter::GetCurrentAmmo_R()
{
	if (CurrentWeapon != nullptr)
	{
		return CurrentAmmo_R;
	}
	return NULL;
}

int32 AMulFPSTESTCharacter::GetTotalAmmo_R()
{
	if (CurrentWeapon != nullptr)
	{
		return TotalAmmo_R;
	}
	return NULL;
}

int32 AMulFPSTESTCharacter::GetCurrentAmmo_S()
{
	if (CurrentWeapon != nullptr)
	{
		return CurrentAmmo_S;
	}
	return NULL;
}

int32 AMulFPSTESTCharacter::GetTotalAmmo_S()
{
	if (CurrentWeapon != nullptr)
	{
		return TotalAmmo_S;
	}
	return NULL;
}

int32 AMulFPSTESTCharacter::GetCurrentAmmo_Ro()
{
	if (CurrentWeapon != nullptr)
	{
		return CurrentAmmo_Ro;
	}
	return NULL;
}

int32 AMulFPSTESTCharacter::GetTotalAmmo_Ro()
{
	if (CurrentWeapon != nullptr)
	{
		return TotalAmmo_Ro;
	}
	return NULL;
}