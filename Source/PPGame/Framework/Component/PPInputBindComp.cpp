// Fill out your copyright notice in the Description page of Project Settings.


#include "PPInputBindComp.h"

#include "PPWeaponMgr.h"
#include "PPGame/Framework/PPCharacter.h"
#include "Engine/InputDelegateBinding.h"
#include "PPGame/Framework/PPPlayerController.h"

UPPInputBindComp::UPPInputBindComp()
{
	CompSpawnCondition = EPPCompSpawnCondition::EPPCSC_OnlyOnAutonomousProxy;
}

void UPPInputBindComp::PPInitComponent()
{
	Super::PPInitComponent();
	
	if (IsValid(OwnerPawn))
	{
		if (OwnerPawn->IsLocallyControlled())
		{
			if (IsValid(OwnerPawn->InputComponent))
			{
				SetupPlayerInputComponent(OwnerPawn->InputComponent);
			}
			else
			{
				OwnerPawn->InputComponent = NewObject<UInputComponent>(OwnerPawn, TEXT("PawnInputComponent0"));
				if (OwnerPawn->InputComponent)
				{
					SetupPlayerInputComponent(OwnerPawn->InputComponent);
					OwnerPawn->InputComponent->RegisterComponent();
					if (UInputDelegateBinding::SupportsInputDelegate(GetClass()))
					{
						OwnerPawn->InputComponent->bBlockInput = OwnerPawn->bBlockInput;
						UInputDelegateBinding::BindInputDelegates(GetClass(), OwnerPawn->InputComponent);
					}
				}
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("This InputBindComp has Not OwnerPawn!"));
	}
}

void UPPInputBindComp::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (IsValid(PlayerInputComponent) && !bInputBind)
	{
		bInputBind = true;
		PlayerInputComponent->BindAxis("MoveForward/Backwards", this, &UPPInputBindComp::MoveForward);
		PlayerInputComponent->BindAxis("MoveRight/Left", this, &UPPInputBindComp::MoveRight);
		PlayerInputComponent->BindAxis("LookUp/Down", this, &UPPInputBindComp::TurnUp);
		PlayerInputComponent->BindAxis("LookLeft/Right", this, &UPPInputBindComp::TurnRight);

		PlayerInputComponent->BindAction("FireAction", IE_Pressed, this, &UPPInputBindComp::FirePressedAction);
		PlayerInputComponent->BindAction("FireAction", IE_Released, this, &UPPInputBindComp::FireReleasedAction);
		PlayerInputComponent->BindAction("JumpAction", IE_Pressed, this, &UPPInputBindComp::JumpAction);
		PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &UPPInputBindComp::ReloadAction);
		PlayerInputComponent->BindAction("AimAction", IE_Pressed, this, &UPPInputBindComp::AimPressedAction);
		PlayerInputComponent->BindAction("AimAction", IE_Released, this, &UPPInputBindComp::AimReleasedAction);
		PlayerInputComponent->BindAction("WeaponUp", IE_Pressed, this, &UPPInputBindComp::WeaponUp);
		PlayerInputComponent->BindAction("WeaponDown", IE_Pressed, this, &UPPInputBindComp::WeaponDown);
		PlayerInputComponent->BindAction("CameraAction", IE_Released, this, &UPPInputBindComp::CameraReleasedAction);
		PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &UPPInputBindComp::InteractAction);
		PlayerInputComponent->BindAction("Shift", IE_Pressed, this, &UPPInputBindComp::ShiftAction);
	}
}

void UPPInputBindComp::MoveForward(float Value)
{
	if (Value && IsValid(OwnerPawn))
	{
		if (OwnerPawn->bUseControllerRotationYaw)
		{
			OwnerPawn->AddMovementInput(OwnerPawn->GetActorForwardVector(), Value);
		}
		else
		{
			// find out which way is forward
			const FRotator Rotation = OwnerPawn->GetController()->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			OwnerPawn->AddMovementInput(Direction, Value);
			OwnerPawn->SetActorRotation(Direction.Rotation());
		}
	}
}

void UPPInputBindComp::MoveRight(float Value)
{
	if (Value && IsValid(OwnerPawn))
	{
		if (OwnerPawn->bUseControllerRotationYaw)
		{
			OwnerPawn->AddMovementInput(OwnerPawn->GetActorRightVector(), Value);
		}
		else
		{
			// find out which way is right
			const FRotator Rotation = OwnerPawn->GetController()->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);
			// get right vector 
			FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			// add movement in that direction
			OwnerPawn->AddMovementInput(Direction, Value);
			OwnerPawn->SetActorRotation(Direction.Rotation());
		}
	}
}

void UPPInputBindComp::TurnUp(float Value)
{
	if (IsValid(OwnerPawn))
	{
		OwnerPawn->AddControllerPitchInput(Value);
	}
}

void UPPInputBindComp::TurnRight(float Value)
{
	if (IsValid(OwnerPawn))
	{
		OwnerPawn->AddControllerYawInput(Value);
	}
}

void UPPInputBindComp::CameraReleasedAction()
{
	if (IsValid(OwnerPawn))
	{
		GetWorld()->GetTimerManager().SetTimer(CameraMoveHandle, this, &UPPInputBindComp::SwitchCameraMode, 0.2f, false);
	}
}

void UPPInputBindComp::AimPressedAction()
{
	if (IsValid(OwnerPawn))
	{
		OwnerPawn->SetRotationMode(EPPRotationMode::Aiming);
		OnAim.Broadcast(true);
	}
}

void UPPInputBindComp::AimReleasedAction()
{
	if (IsValid(OwnerPawn))
	{
		OwnerPawn->SetRotationMode(EPPRotationMode::LookingDirection);
		OnAim.Broadcast(false);
	}
}

void UPPInputBindComp::JumpAction()
{
	if (IsValid(OwnerPawn))
	{
		OwnerPawn->Jump();
	}
}

void UPPInputBindComp::ReloadAction()
{
	if (IsValid(OwnerPawn))
	{
		OnReload.Broadcast(true);
	}
}

void UPPInputBindComp::FirePressedAction()
{
	OnFire.Broadcast(true);
}

void UPPInputBindComp::FireReleasedAction()
{
	OnFire.Broadcast(false);
}

void UPPInputBindComp::WeaponUp()
{
	if (CheckOpenMirror(true))
	{
		return;
	}
	OnChangeWeapon.Broadcast(true);
}

void UPPInputBindComp::WeaponDown()
{
	if (CheckOpenMirror(false))
	{
		return;
	}
	OnChangeWeapon.Broadcast(false);
}

void UPPInputBindComp::InteractAction()
{
	OnInteract.Broadcast();
}

void UPPInputBindComp::ShiftAction()
{
	OnShift.Broadcast();
}

void UPPInputBindComp::SwitchCameraMode()
{
	if (IsValid(OwnerPawn))
	{
		EPPViewMode tViewMode = OwnerPawn->GetViewMode();
		if (tViewMode == EPPViewMode::FirstPerson)
		{
			OwnerPawn->SetViewMode(EPPViewMode::ThirdPerson);
		}
		else if (tViewMode == EPPViewMode::ThirdPerson)
		{
			OwnerPawn->SetViewMode(EPPViewMode::FirstPerson);
		}
	}
}

bool UPPInputBindComp::CheckOpenMirror(bool Up)
{
	if (IsValid(OwnerPawn))
	{
		EPPRotationMode tRotationMode = OwnerPawn->GetRotationMode();
		EPPOverlayState tOverlayState = OwnerPawn->GetOverlayState();
		if (tRotationMode == EPPRotationMode::Aiming && tOverlayState == EPPOverlayState::Sniper)
		{
			APPPlayerController* tController = Cast<APPPlayerController>(OwnerPawn->GetController());
			if (IsValid(tController))
			{
				tController->OnMouseWheel(Up);
			}
			return true;
		}
	}
	return false;
}
