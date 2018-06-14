// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"

class UTankAimingComponent;
class ATank;

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float AcceptanceRadius = 3000;

	UTankAimingComponent* AimingComponent = nullptr;
	ATank* PlayerTank = nullptr;

	void BeginPlay() override;
	void Tick(float DeltaTime) override;

};
