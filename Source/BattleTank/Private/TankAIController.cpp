// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Tank.h"

void ATankAIController::BeginPlay() {
	Super::BeginPlay();

	ControlledTank = Cast<ATank>(GetPawn());
	PlayerTank = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (!PlayerTank || !ControlledTank)
		UE_LOG(LogTemp, Error, TEXT("Could not find player tank or possessed tank !"))
}

void ATankAIController::Tick(float DeltaTime)
{
	if (!ControlledTank || !PlayerTank) return;

	MoveToActor(PlayerTank, AcceptanceRadius);

	ControlledTank->AimAt(
		PlayerTank->GetActorLocation()
	);

	//ControlledTank->Fire();
}
