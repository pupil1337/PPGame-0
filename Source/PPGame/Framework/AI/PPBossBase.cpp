// Fill out your copyright notice in the Description page of Project Settings.


#include "PPBossBase.h"
#include "PPGame/Framework/PPCharacter.h"

APPBossBase::APPBossBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APPBossBase::Tick(float DeltaSeconds)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		APPCharacter* tPlayer = GetEnemy();
		if (IsValid(tPlayer) && MonsterIsAlive())
		{
			static float tTimer = 0.0f;
			tTimer += DeltaSeconds;
			if (tTimer > 1.0f)
			{
				FVector tV = tPlayer->GetActorLocation() - GetActorLocation();
				FRotator tR = tV.Rotation();
				tR.Pitch = 0.0f;
				tR.Normalize();

				SetActorRotation(tR);
				tTimer = 0.0f;
			}
		}
	}
}

void APPBossBase::SetEnemy(APPCharacter* InEnemy)
{
	Super::SetEnemy(InEnemy);

	bUseControllerRotationYaw = false;
}

void APPBossBase::Dead(AActor* tInstigator)
{
	Super::Dead(tInstigator);

	if (GetLocalRole() == ROLE_Authority)
	{
		FTimerHandle tHandle;
		GetWorldTimerManager().SetTimer(tHandle, FTimerDelegate::CreateLambda([this]() {
			GetWorld()->DestroyActor(this, true);
			}), 5.0f, false);
	}
}
