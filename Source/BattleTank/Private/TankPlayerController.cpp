// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "Tank.h"

#define OUT


void ATankPlayerController::BeginPlay() {
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("ATankPlayerController BeginPlay !"))

	auto Tank = GetControlledTank();

	if (!Tank)
		UE_LOG(LogTemp, Error, TEXT("Player controller not possessing a tank !"))
	else
		UE_LOG(LogTemp, Warning, TEXT("Tank : %s"), *GetControlledTank()->GetName())
}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AimTowardsCrosshair();
}

ATank* ATankPlayerController::GetControlledTank() const {
	return Cast<ATank>(GetPawn());
}

void ATankPlayerController::AimTowardsCrosshair() const
{
	if (!GetControlledTank())
		return;

	FVector HitLocation;
	if (GetSightRayHitLocation(OUT HitLocation)) {
		GetControlledTank()->AimAt(HitLocation);
	}
}

bool ATankPlayerController::GetSightRayHitLocation(FVector & OutHitLocation) const
{
	FVector CrosshairLocation, LookDirection;

	if (GetLookDirection(CrosshairLocation, LookDirection)) {
		FHitResult HitResult;
		
		if (FindAimingLocation(OUT HitResult, CrosshairLocation, LookDirection)) {
			OutHitLocation = HitResult.Location;

			return true;
		}
	}

	return false;
}

bool ATankPlayerController::GetLookDirection(FVector & CrosshairLocation, FVector & LookDirection) const
{
	int32 ViewportWidth, ViewportHeight;
	GetViewportSize(ViewportWidth, ViewportHeight);

	FVector2D CrosshairScreenLocation =
		FVector2D(
			ViewportWidth * CrosshairXLocation,
			ViewportHeight * CrosshairYLocation
		);

	return DeprojectScreenPositionToWorld(CrosshairScreenLocation.X, CrosshairScreenLocation.Y, OUT CrosshairLocation, OUT LookDirection);
}

bool ATankPlayerController::FindAimingLocation(FHitResult& HitResult, FVector CrosshairLocation, FVector LookDirection) const
{
	FVector End = CrosshairLocation + (LookDirection * LineTraceRange);

	return GetWorld()->LineTraceSingleByChannel(
		OUT HitResult,
		CrosshairLocation,
		End,
		ECollisionChannel::ECC_Visibility
	);
}
