// Fill out your copyright notice in the Description page of Project Settings.

#include "SWeapon.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "CoopGame.h"
#include "TimerManager.h"

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

void ASWeapon::BeginPlay()
{
	Super::BeginPlay();

	TimeBetweenShots = 60.0f / RateOfFire;

	TimeToIncreaseDispersion = 60.0f / RateOfDispersion;

	CurrentNumBullets = BulletsLoader;
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
	//Check if we have bullets
	AActor* Owner = GetOwner();
	if (Owner && CurrentNumBullets > 0)
	{
		//Get eye transform
		FVector EyesLocation;
		FRotator EyesRotation;
		Owner->GetActorEyesViewPoint(EyesLocation, EyesRotation);

		//Apply some Recoil
		if (APawn* OwnerPawn = Cast<APawn>(Owner))
		{
			OwnerPawn->AddControllerPitchInput(FMath::RandRange(MinRecoil, MaxRecoil));
		}

		//Calculate direction (and trace end) multipliying by big number
		FVector ShotDirection = EyesRotation.Vector();
		ApplyDispersion(ShotDirection);
		FVector TraceDirection = EyesLocation + (ShotDirection * 10000);

		//Collision query params

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(Owner);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bReturnPhysicalMaterial = true;
		QueryParams.bTraceComplex = true;
		
		FHitResult Hit;
		if (GetWorld()->LineTraceSingleByChannel(Hit, EyesLocation, TraceDirection, COLLISION_WEAPON, QueryParams))
		{
			//If there is a hit
			//Process Damage
			AActor* HitActor = Hit.GetActor();

			// Get Surface type
			EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

			float ActualDamage = BaseDamage;
			if (SurfaceType == SURFACE_FLESHVULNERABLE)
			{
				ActualDamage *= 4.0f;
			}

			UGameplayStatics::ApplyPointDamage(HitActor, ActualDamage, ShotDirection, Hit, Owner->GetInstigatorController(), this, DamageType);

			UParticleSystem* SelectedEffect = nullptr;

			switch (SurfaceType)
			{
				case SURFACE_FLESHDEFAULT:
				case SURFACE_FLESHVULNERABLE:
				{
					SelectedEffect = FleshImpactEffect;
					break;
				}
				default:
				{
					SelectedEffect = DefaultImpactEffect;
					break;
				}
			}

			//Spawn Particle effects on hit
			if (SelectedEffect)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
			}

		}

		if (DebugWeaponDrawing > 0)
		{
			DrawDebugLine(GetWorld(), EyesLocation, TraceDirection, FColor::White, false, 1.0f, 0, 1.0f);
		}

		PlayFireEffects(TraceDirection, Hit);

		LastFireTime = GetWorld()->TimeSeconds;
		CurrentNumBullets--;
	}
}

void ASWeapon::StartFire()
{
	if (!bFirstFire)
	{
		bFirstFire = true;
		GetWorldTimerManager().SetTimer(TimerHandle_TimeIncreaseDispersion, this, &ASWeapon::IncreaseDispersion, TimeToIncreaseDispersion, true, 0.0f);
	}
	float FirstDelay = FMath::Max(LastFireTime + TimeBetweenShots - GetWorld()->TimeSeconds, 0.0f);
	GetWorldTimerManager().SetTimer(TimerHandle_TimeBetweenShots, this, &ASWeapon::Fire, TimeBetweenShots, true, FirstDelay);
}

void ASWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_TimeBetweenShots);
	GetWorldTimerManager().ClearTimer(TimerHandle_TimeIncreaseDispersion);
	bFirstFire = false;
	CurrentDispersionRadius = 0.0f;
}

void ASWeapon::Reload()
{
	CurrentNumBullets = BulletsLoader;
}

void ASWeapon::ApplyDispersion(FVector& ShotDirection)
{
	//Apply a dispersion to the shotDirection based on time and radius
	FVector Offset(FMath::RandRange(-CurrentDispersionRadius, CurrentDispersionRadius), 0.0f, FMath::RandRange(-CurrentDispersionRadius, CurrentDispersionRadius));
	ShotDirection += Offset;
}

void ASWeapon::IncreaseDispersion()
{
	if (CurrentDispersionRadius < MaxDispersionRadius)
	{
		CurrentDispersionRadius += IncrementDispersion;
		UE_LOG(LogTemp, Warning, TEXT("Dispersion %f "), CurrentDispersionRadius);
		GetWorldTimerManager().SetTimer(TimerHandle_TimeIncreaseDispersion, this, &ASWeapon::IncreaseDispersion, TimeToIncreaseDispersion, true, 0.0f);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_TimeIncreaseDispersion);
	}
	
}
