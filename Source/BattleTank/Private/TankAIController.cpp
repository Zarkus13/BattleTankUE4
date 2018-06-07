// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Tank.h"

void ATankAIController::BeginPlay() {
	Super::BeginPlay();

	auto PlayerTank = GetPlayerTank();

	if (!PlayerTank)
		UE_LOG(LogTemp, Error, TEXT("Could not find player tank !"))
}

void ATankAIController::Tick(float DeltaTime)
{
	GetTankPossessed()->AimAt(
		GetPlayerTank()->GetActorLocation()
	);
}

ATank * ATankAIController::GetTankPossessed() const
{
	return Cast<ATank>(GetPawn());
}

ATank * ATankAIController::GetPlayerTank() const
{
	return Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
}
