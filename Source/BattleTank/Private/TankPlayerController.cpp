// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "TankAimingComponent.h"

#define OUT


void ATankPlayerController::BeginPlay() {
	Super::BeginPlay();

	AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();

	if (ensure(AimingComponent))
		FoundAimingComponent(AimingComponent);
	else
		UE_LOG(LogTemp, Error, TEXT("No AimingComponent !"))
}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AimTowardsCrosshair();
}

void ATankPlayerController::AimTowardsCrosshair() const
{
	if (!ensure(AimingComponent))
		return;

	FVector HitLocation;
	if (GetSightRayHitLocation(OUT HitLocation)) {
		AimingComponent->AimAt(HitLocation);
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
