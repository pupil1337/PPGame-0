// Fill out your copyright notice in the Description page of Project Settings.


#include "PPGLaDOS.h"
#include "PPGame/Framework/AI/PPMonsterBase.h"
#include "PPGame/Framework/PPCharacter.h"

APPGLaDOS::APPGLaDOS()
{

}

void APPGLaDOS::BeginPlay()
{
	Super::BeginPlay();
	
}

void APPGLaDOS::OnMonsterSpawn(TSubclassOf<APPMonsterBase> MonsterClass, FVector Location, APPCharacter* TriggerPlayer)
{
	if (IsValid(MonsterClass))
	{
		UClass* tClass = MonsterClass.Get();
		if (IsValid(tClass))
		{
			FActorSpawnParameters tParam;
			tParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			APPMonsterBase* tMonster = GetWorld()->SpawnActor<APPMonsterBase>(tClass, Location, FRotator(ForceInit), tParam);
			tMonster->SetEnemy(TriggerPlayer);
			tMonster->SetGLaDOS(this);
			AllMonsters.Add(tMonster);
		}
	}
}

void APPGLaDOS::OnMonsterDeath(APPMonsterBase* Monster)
{
	if (AllMonsters.Contains(Monster))
	{
		AllMonsters.Remove(Monster);
	}
}
