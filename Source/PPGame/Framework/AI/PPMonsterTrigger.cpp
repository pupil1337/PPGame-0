// Fill out your copyright notice in the Description page of Project Settings.


#include "PPMonsterTrigger.h"
#include "PPGame/Framework/PPCharacter.h"
#include "PPGame/Framework/AI/PPGLaDOS.h"
#include "Kismet/KismetMathLibrary.h"
#include "NavigationSystem.h"


APPMonsterTrigger::APPMonsterTrigger()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APPMonsterTrigger::Tick(float DeltaSeconds)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		SpawnTimer += DeltaSeconds;
		if (SpawnTimer > 1.0f)
		{
			SpawnTimer = 0.0f;
			for (auto& it: AllInPlayer)
			{
				if (IsValid(it))
				{
					OnSpawnMonster(it);
				}
			}
		}
	}
}

void APPMonsterTrigger::OnPlayerIn(APPCharacter* InPlayer)
{
	AllInPlayer.Add(InPlayer);
	OnSpawnMonster(InPlayer);
}

void APPMonsterTrigger::OnPlayerOut(APPCharacter* OutPlayer)
{
	if (AllInPlayer.Contains(OutPlayer))
	{
		AllInPlayer.Remove(OutPlayer);
	}
}

void APPMonsterTrigger::OnSpawnMonster(APPCharacter* TriggerPlayer)
{
	if (IsValid(GLaDOS) && MonsterClass.Num() > 0)
	{
		// λ��
		FVector tV = GetActorLocation();
		UNavigationSystemV1* NavSys = UNavigationSystemV1::GetNavigationSystem(GetWorld());
		if (IsValid(NavSys))
		{
			FNavLocation tNavLocation;
			if (NavSys->GetRandomReachablePointInRadius(tV, 2000.0f, tNavLocation))
			{
				tV = tNavLocation.Location;
			}
		}
		TSubclassOf<APPMonsterBase> tMonsterSubclass = MonsterClass[UKismetMathLibrary::RandomInteger(MonsterClass.Num())];
		GLaDOS->OnMonsterSpawn(tMonsterSubclass, tV, TriggerPlayer);
	}
}
