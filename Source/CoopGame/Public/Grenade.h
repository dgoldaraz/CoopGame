// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grenade.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UParticleSystem;

UCLASS()
class COOPGAME_API AGrenade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrenade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Sphere collision component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	USphereComponent* CollisionComp;

	//Static Mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	UStaticMeshComponent* MeshComponent;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovement;

	//Time of Grenade before explode
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	float LifeTime = 1.0f;

	//Time of Grenade before explode
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	float MaxSpeed = 2000.0f;


	//Explosion Radius
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	float ExplosionRadius = 100.0f;

	//Explosion Effect for Grenade
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	UParticleSystem* ExplosionEffect = nullptr;

	//Function to make grenade explode
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void Explode(AActor* DestroyedActor);

	//Damage type
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<UDamageType> DamageType;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
