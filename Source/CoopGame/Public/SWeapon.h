// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWeapon.generated.h"


class USkeletalMeshComponent;
class UDamageType;
class UParticleSystem;

UCLASS()
class COOPGAME_API ASWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASWeapon();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void Fire();

protected:


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
	UParticleSystem* ImpactEffect = nullptr;

	//Tracer effect particle from Muzzle point to hit point
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* TracerEffect = nullptr;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly, Category = "Effects")
	FName MuzzleSocketName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	FName TracerStartName;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TSubclassOf<UCameraShake> FireCamShake;
};
