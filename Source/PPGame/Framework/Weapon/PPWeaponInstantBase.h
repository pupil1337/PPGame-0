// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PPWeaponBase.h"
#include "PPWeaponInstantBase.generated.h"

USTRUCT()
struct FInstantInfo
{
	GENERATED_BODY()

	FVector StartLocation;
	FRotator StartRotation;

	FVector HitLocation;
	AActor* HitActor;
};

class UPPCrosshairWidget;
class UParticleSystem;

/**
 * 
 */
UCLASS()
class PPGAME_API APPWeaponInstantBase : public APPWeaponBase
{
	GENERATED_BODY()
	
public:
	APPWeaponInstantBase();
	virtual void BeginPlay() override;
	virtual bool Fire() override;
	virtual void Aim(bool Op) override;

protected:
	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* TrailPS;

	UFUNCTION()
	void PlayTrailPS(FVector Start, FRotator Rotation);
	UFUNCTION(Server, Unreliable)
	void Server_PlayTrailPS(FVector Start, FRotator Rotation);
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayTrailPS(FVector Start, FRotator Rotation);
};