// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

class ATank;

/**
*
*/
UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	float CrosshairXLocation = 0.5f;

	UPROPERTY(EditDefaultsOnly)
	float CrosshairYLocation = 0.33333f;

	UPROPERTY(EditDefaultsOnly)
	int32 LineTraceRange = 1000000;

private:
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

	ATank * GetControlledTank() const;

	void AimTowardsCrosshair() const;

	bool GetSightRayHitLocation(FVector &OutHitLocation) const;

	bool GetLookDirection(FVector& CrosshairLocation, FVector& LookDirection) const;

	bool FindAimingLocation(FHitResult& HitResult, FVector CrosshairLocation, FVector LookDirection) const;

};
