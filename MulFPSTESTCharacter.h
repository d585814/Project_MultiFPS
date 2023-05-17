// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MulFPSTESTCharacter.generated.h"

UCLASS(config=Game)
class AMulFPSTESTCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	AMulFPSTESTCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
		

protected:
	
	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	void Turn(float Rate);

	void LookUp(float Rate);
		
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface
	
	UFUNCTION()
	void CallSpawnWeapon(TSubclassOf<AWeaponActual_Master> Weapon);
	
	UFUNCTION(Server, Reliable)
	void OnServerDestroy(AActor* ActorToDestroy);

	UFUNCTION(Server, Reliable)
	void OnServerSpawnWeapon(TSubclassOf<AActor> ActorToSpawn);

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ARocketProjectile> RocketProjectileClass;

	

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(Server , Reliable)
	void LookUpToServer(float NewLookUpAxis);

	UFUNCTION(Server, Reliable)
	void TurnToServer(float NewTurnAxis);
					
	UFUNCTION(BlueprintPure)
	float GetLookUpAxis() {return LookUpAxis;}

	UFUNCTION(BlueprintPure)
	float GetTurnAxis() { return TurnAxis; }

	UPROPERTY()
	TArray<TSubclassOf<class AWeaponActual_Master>> WeaponInventory;

	UFUNCTION()
	void IncrementInventory();

	UFUNCTION()
	void DecrementInventory();
	
	UPROPERTY(Replicated)
	class AActor* SpawnedActor;

	UPROPERTY(Replicated)
	class AWeaponActual_Master* CurrentWeapon;

	UFUNCTION()
	FTransform CalculateFireTo();

	UFUNCTION()
	void Fire();

	UFUNCTION(Server, Reliable)
	void SpawnProjectile(APawn* Pawn);

	UFUNCTION(Server, Reliable)
	void ToggleCanFire();

	UFUNCTION(BlueprintNativeEvent)
	void CallToggleCombatMode();
	virtual void CallToggleCombatMode_Implementation();

	UFUNCTION(Server, Reliable)
	void ToggleCombatMode();

	UFUNCTION()
	void CombatModeWidget();

	UFUNCTION()
	void OnRep_CombatMode();

	UFUNCTION(BlueprintPure)
	bool GetCombatMode(){return CombatMode;}
		
	UFUNCTION(Server, Reliable)
	void CallGunAnimation();

	UFUNCTION(NetMulticast, Reliable)
	void GunAnimation();

	UFUNCTION(NetMulticast, Reliable)
	void GunReloadAnimation();
	   
	UPROPERTY(Replicated, EditAnyWhere,BlueprintReadWrite)
	bool CanFire;

	UFUNCTION()
	void OnRep_CurrentHealth();
		
	void HealthUpdate();
			
	UFUNCTION(BlueprintPure,Category = "Health")
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }
	
	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetCurrentHealth(float healthValue);

	UFUNCTION(BlueprintCallable, Category = "Health")
	float TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
			
	void ReloadAmmo();	

	UFUNCTION(BlueprintPure, Category = "Weapon")
	int32 GetCurrentAmmo_R();

	UFUNCTION(BlueprintPure, Category = "Weapon")
	int32 GetTotalAmmo_R();

	UFUNCTION(BlueprintPure, Category = "Weapon")
	int32 GetCurrentAmmo_S();

	UFUNCTION(BlueprintPure, Category = "Weapon")
	int32 GetTotalAmmo_S();

	UFUNCTION(BlueprintPure, Category = "Weapon")
	int32 GetCurrentAmmo_Ro();

	UFUNCTION(BlueprintPure, Category = "Weapon")
	int32 GetTotalAmmo_Ro();

	UFUNCTION(BlueprintPure, Category = "Reload")
	bool GetIsReload() { return IsReload; }

private:

	UFUNCTION()
	void Interact();

	void CheckInventory();
	
	UPROPERTY(EditAnywhere, Replicated)
	float LookUpAxis;

	UPROPERTY(EditAnywhere, Replicated)
	float TurnAxis;
	
	UPROPERTY()
	int32 WeaponIndex;

	UPROPERTY()
	class TSubclassOf<AWeaponActual_Master> WeaponToSpawn;

	FTransform FireTo;
	
	FTimerHandle Handle;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth)
	float CurrentHealth;

	float MaxHealth;

	UPROPERTY(Replicated)
	int32 CurrentAmmo_R;

	UPROPERTY(Replicated)
	int32 TotalAmmo_R;

	UPROPERTY(Replicated)
	int32 CurrentAmmo_S;

	UPROPERTY(Replicated)
	int32 TotalAmmo_S;

	UPROPERTY(Replicated)
	int32 CurrentAmmo_Ro;

	UPROPERTY(Replicated)
	int32 TotalAmmo_Ro;

	UPROPERTY(ReplicatedUsing = OnRep_CombatMode)//Using= OnRep_CombatMode
	bool CombatMode;

	class UMulGameInstance* GameInstance;

	UPROPERTY(Replicated)
	bool IsReload;
};

