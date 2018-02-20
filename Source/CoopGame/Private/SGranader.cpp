// Fill out your copyright notice in the Description page of Project Settings.

#include "SGranader.h"
#include "Grenade.h"

void ASGranader::Fire()
{
	if (AActor* Owner = GetOwner())
	{
		//Create a projectile that explodes after one second
		//Create Sphere and make it explode after one second
		FVector EyesLocation;
		FRotator EyesRotation;
		Owner->GetActorEyesViewPoint(EyesLocation, EyesRotation);

		FVector MuzzleLocation = MeshComponent->GetSocketLocation(MuzzleSocketName);

		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		if (APawn* PawnOwner = Cast<APawn>(Owner))
		{
			ActorSpawnParams.Instigator = PawnOwner;
		}

		// spawn the projectile at the muzzle
		if (ProjectileClass)
		{
			GetWorld()->SpawnActor<AGrenade>(ProjectileClass, MuzzleLocation, EyesRotation, ActorSpawnParams);
		}
	}
}

