// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PPCompBase.h"
#include "PPGame/Framework/Library/PPCharacterEnumLibrary.h"
#include "PPWeaponMgr.generated.h"

class APPWeaponBase;
class UAnimMontage;

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PPGAME_API UPPWeaponMgr : public UPPCompBase
{
	GENERATED_BODY()
public:
	UPPWeaponMgr();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	virtual void PPInitComponent() override;
	virtual void ChangeControllerRole() override;
	virtual void OnUnregister() override;

protected:
	UPROPERTY(Replicated, EditDefaultsOnly)
	TArray<TSubclassOf<APPWeaponBase> > WeaponClassList;
	
	UPROPERTY(Replicated)
	TArray<APPWeaponBase*> WeaponList;

	UPROPERTY(EditDefaultsOnly)
	TMap<EPPOverlayState, UAnimMontage*> ChangeWeaponMontages;
	
	UPROPERTY()
	FTimerHandle InitEquipHandle;
	UFUNCTION()
	void OnRep_CurrWeapon(APPWeaponBase* PreWeapon);
	UPROPERTY(ReplicatedUsing=OnRep_CurrWeapon)
	APPWeaponBase* CurrWeapon;
	int CurrIndex;
	bool bFiring;
	bool bChangeClip;
	bool Active = true;
	
	void Equip(APPWeaponBase* NewWeapon, bool Force);
	void OnWeaponChanged(APPWeaponBase* PreWeapon);
	UFUNCTION(Server, Reliable)
	void Server_Equip(APPWeaponBase* NewWeapon, bool Force);
	
public:
	UFUNCTION()
	void OnFireState(bool Op);
	UFUNCTION()
	void OnAimState(bool Op);
	UFUNCTION()
	void OnFire(bool Op);
	UFUNCTION()
	void OnReload(bool Start);
	UFUNCTION()
	void OnReloadEnd();

	UFUNCTION()
	void OnSwitchWeapon(bool Up);

	UFUNCTION()
	void OnPickup();

	UFUNCTION()
	void OnDead();
	UFUNCTION(Server, Reliable)
	void Server_OnDead();

	UFUNCTION()
	void OnReSpawn();
	UFUNCTION(Server, Reliable)
	void Server_ReSpawn();

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<AActor> PickItem;
	UPROPERTY(BlueprintReadWrite)
	AActor* PickBase;
	void EquipAndDele(TSubclassOf<AActor> InPickItem, AActor* InPickBase);
	UFUNCTION(Server, Reliable)
	void Server_EquipAndDele(TSubclassOf<AActor> InPickItem, AActor* InPickBase);
	
};
