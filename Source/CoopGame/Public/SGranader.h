// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SWeapon.h"
#include "SGranader.generated.h"

/**
 * Weapon Class for a Granader weapon
 */

class AGrenade;

UCLASS()
class COOPGAME_API ASGranader : public ASWeapon
{
	GENERATED_BODY()

protected:

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void Fire() override;

	//Grenade Class
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AGrenade> ProjectileClass;
	
};
