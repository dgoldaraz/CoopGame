// Fill out your copyright notice in the Description page of Project Settings.

#include "Grenade.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
AGrenade::AGrenade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->SetCollisionProfileName("Projectile");

	CollisionComp->SetupAttachment(MeshComponent);

	//Projectile Movement updater
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = MeshComponent;
	ProjectileMovement->InitialSpeed = MaxSpeed;
	ProjectileMovement->MaxSpeed = MaxSpeed;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	//Set Life time of grenade
	InitialLifeSpan = LifeTime;

	OnDestroyed.AddDynamic(this, &AGrenade::Explode);
}

// Called when the game starts or when spawned
void AGrenade::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGrenade::Explode(AActor* DestroyedActor)
{
	if (!DestroyedActor)
	{
		DestroyedActor = this;
	}

	//Damage in Radius
	const TArray<AActor*> IgnoreActors;
	UGameplayStatics::ApplyRadialDamage(GetWorld(), 50.0f, GetActorLocation(), ExplosionRadius, DamageType, IgnoreActors);
	DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius, 32, FColor::Yellow, false, 2.0f);
	//Play Explosion effect
	if (ExplosionEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
	}
}

