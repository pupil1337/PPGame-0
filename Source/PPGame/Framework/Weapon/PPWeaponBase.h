﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PPGame/Framework/Library/PPCharacterEnumLibrary.h"
#include "PPWeaponBase.generated.h"

UCLASS()
class PPGAME_API APPWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	APPWeaponBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Equip();
	
	UPROPERTY(VisibleDefaultsOnly)
	EPPOverlayState WeaponType;
	
protected:
	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* Mesh;
	
};
