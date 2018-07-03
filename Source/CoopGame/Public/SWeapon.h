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
	float RateOfFire = 600;

	// Number of Bullets in each Loader
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float BulletsLoader = 60;


	FTimerHandle TimerHandle_TimeBetweenShots;

	float LastFireTime = 0.0f;

	//Derive from RateOfFire
	float TimeBetweenShots; 

	int CurrentNumBullets = 0;
};
