// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"

class UTankAimingComponent;
class ATank;
class APawn;

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
	UTankAimingComponent* AimingComponent = nullptr;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float AcceptanceRadius = 8000;

	ATank* PlayerTank = nullptr;

	void BeginPlay() override;
	void Tick(float DeltaTime) override;

	void SetPawn(APawn* InPawn) override;

	UFUNCTION()
	void OnTankDeath();

};
