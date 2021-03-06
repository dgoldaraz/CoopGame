// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWeapon.generated.h"


class USkeletalMeshComponent;
class UDamageType;
class UParticleSystem;
class UAnimMontage;

UCLASS()
class COOPGAME_API ASWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASWeapon();

	void StartFire();

	void StopFire();

	void Reload();

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UAnimMontage* ReloadMontage;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	int GetCurrentBullets() { return CurrentNumBullets; }


protected:

	virtual void BeginPlay() override;

	virtual void Fire();

	virtual void ApplyDispersion(FVector& ShotDirection);

	void IncreaseDispersion();

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,  Category = "Component")
	USkeletalMeshComponent* MeshComponent = nullptr;

	//Damage type
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	TSubclassOf<UDamageType> DamageType;

	void PlayFireEffects(const FVector& TraceDirection, const FHitResult& Hit);

	//Muzzle effect particle system when the weapon is fired
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* MuzzleEffect = nullptr;

	//Muzzle effect particle system when the weapon hit something
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* DefaultImpactEffect = nullptr;

	//Muzzle effect particle system when the weapon hit something on Flesh
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* FleshImpactEffect = nullptr;

	//Tracer effect particle from Muzzle point to hit point
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* TracerEffect = nullptr;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly, Category = "Effects")
	FName MuzzleSocketName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	FName TracerStartName;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TSubclassOf<UCameraShake> FireCamShake;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float BaseDamage = 20.0f;

	// RPM - Bullets per minute
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float RateOfFire = 600.0f;

	// Number of Bullets in each Loader
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float BulletsLoader = 60.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float MaxDispersionRadius = 0.1f;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float RateOfDispersion = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float IncrementDispersion = 0.001f;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float MaxRecoil = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float MinRecoil = 0.0f;

	FTimerHandle TimerHandle_TimeBetweenShots;

	FTimerHandle TimerHandle_TimeIncreaseDispersion;

	float LastFireTime = 0.0f;

	//Derive from RateOfFire
	float TimeBetweenShots = 0.0f; 

	int CurrentNumBullets = 0;

	float CurrentDispersionRadius = 0.0f;

	float TimeToIncreaseDispersion = 0.0f;

	bool bFirstFire = false;
};
