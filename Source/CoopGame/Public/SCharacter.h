// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class ASWeapon;

UCLASS()
class COOPGAME_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Functions that moves the character forward and right keyboard movements. Value needs to be between [-1,1]
	void MoveForward(float Value);
	void MoveRight(float Value);

	void BeginCrouch();

	void EndCrouch();	

	void BeginZoom();
	void EndZoom();

	void StartFire();
	void StopFire();

	void Reload();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UCameraComponent* CameraComponent = nullptr;

	//Spring arm help us to move the camera in a nicer way, avoid Wall break the camera view and more things like add Camera Lag
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USpringArmComponent* SpringArmComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	float ZoomedFOV = 65.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Player", meta = (ClampMin = 0.1, ClampMax = 100))
	float ZoomInterpSpeed = 20.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Player")
	bool ReloadRequested = false;

	UFUNCTION()
	void FinishReload(class UAnimMontage*Montage, bool bInterrupted);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	int GetBullets();

	ASWeapon* CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	TSubclassOf<ASWeapon> StarterWeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	FName WeaponAttachSocketName;

	bool bFiring = false;

	bool bCanFire = true;

	bool bReloading = false;

	bool bWantsToZoom;

	// FOV when we begin Play
	float DefaultFOV;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Begin APawn Interface

	/** @return	Pawn's eye location */
	// In our situation it returns the camera position
	virtual FVector GetPawnViewLocation() const override;

	//End APawn Interface
};
