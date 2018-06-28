// Fill out your copyright notice in the Description page of Project Settings.

#include "SWeapon.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"

static int32 DebugWeaponDrawing = 0;
FAutoConsoleVariableRef CVARDebugWeaponDrawing(TEXT("Coop.DebugWeapons"), DebugWeaponDrawing, TEXT("Draw Debug Lines for Weapons"), ECVF_Cheat);

// Sets default values
ASWeapon::ASWeapon()
{
	//Create mesh fro weapon
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	//Set socket name on code so it can change dynamically
	MuzzleSocketName = "MuzzleSocket";
	TracerStartName = "BeamEnd";
}

void ASWeapon::PlayFireEffects(const FVector& TraceDirection, const FHitResult& Hit)
{
	//Muzzle particle system
	if (MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComponent, MuzzleSocketName);
	}

	//Get Muzzle socket location
	FVector MuzzleLocation = MeshComponent->GetSocketLocation(MuzzleSocketName);

	if (TracerEffect)
	{
		UParticleSystemComponent* TracerComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, MuzzleLocation);
		if (TracerComponent)
		{
			TracerComponent->SetVectorParameter(TracerStartName, Hit.bBlockingHit ? Hit.ImpactPoint : TraceDirection);
		}
	}

	APawn* MyPawn = Cast<APawn>(GetOwner());
	if (MyPawn)
	{
		APlayerController* MyController = Cast<APlayerController>(MyPawn->GetController());
		MyController->ClientPlayCameraShake(FireCamShake);
	}
}

void ASWeapon::Fire()
{
	//Trace the world from Pawn eyes to crosshair location (center of screen)
	if (AActor* Owner = GetOwner())
	{
		//Get eye transform
		FVector EyesLocation;
		FRotator EyesRotation;
		Owner->GetActorEyesViewPoint(EyesLocation, EyesRotation);


		//Calculate direction (and trace end) multipliying by big number
		FVector ShotDirection = EyesRotation.Vector();
		FVector TraceDirection = EyesLocation + (ShotDirection * 10000);

		//Collision query params

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(Owner);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;
		
		FHitResult Hit;
		if (GetWorld()->LineTraceSingleByChannel(Hit, EyesLocation, TraceDirection, ECC_Visibility, QueryParams))
		{
			//If there is a hit
			//Process Damage
			AActor* HitActor = Hit.GetActor();
			UGameplayStatics::ApplyPointDamage(HitActor, 20.0f, ShotDirection, Hit, Owner->GetInstigatorController(), this, DamageType);

			//Spawn Particle effects on hit
			if (ImpactEffect)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
			}
		}

		if (DebugWeaponDrawing > 0)
		{
			DrawDebugLine(GetWorld(), EyesLocation, TraceDirection, FColor::White, false, 1.0f, 0, 1.0f);
		}

		PlayFireEffects(TraceDirection, Hit);
	}
}
