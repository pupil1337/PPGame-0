// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "PPMonsterTrigger.generated.h"

class APPGLaDOS;
class APPCharacter;
class APPMonsterBase;

/**
 * 
 */
UCLASS()
class PPGAME_API APPMonsterTrigger : public ATriggerBox
{
	GENERATED_BODY()

public:
	APPMonsterTrigger();

protected:
	virtual void Tick(float DeltaSeconds) override;
	UFUNCTION(BlueprintCallable)
	void OnPlayerIn(APPCharacter* InPlayer);
	UFUNCTION(BlueprintCallable)
	void OnPlayerOut(APPCharacter* OutPlayer);

	void OnSpawnMonster(APPCharacter* TriggerPlayer);

protected:
	UPROPERTY(EditAnywhere)
	APPGLaDOS* GLaDOS;
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<APPMonsterBase> > MonsterClass;
	UPROPERTY(EditAnywhere)
	float Interval = 1.0f;
	
	UPROPERTY()
	TArray<APPCharacter*> AllInPlayer;
	float SpawnTimer;

};
