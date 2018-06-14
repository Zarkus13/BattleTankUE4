// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Tank.h"
#include "TankAimingComponent.h"

void ATankAIController::BeginPlay() {
	Super::BeginPlay();

	AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	PlayerTank = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
}

void ATankAIController::Tick(float DeltaTime)
{
	if (!ensure(AimingComponent && PlayerTank)) return;

	MoveToActor(PlayerTank, AcceptanceRadius);

	AimingComponent->AimAt(
		PlayerTank->GetActorLocation()
	);

	//ControlledTank->Fire();
}
