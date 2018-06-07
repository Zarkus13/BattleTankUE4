// Fill out your copyright notice in the Description page of Project Settings.

#include "TankBarrel.h"
#include "Engine/World.h"

void UTankBarrel::Elevate(float RelativeSpeed) {
	auto ElevationChange = FMath::Clamp<float>(RelativeSpeed, -1.f, 1.f) * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	auto RawNewElevation = RelativeRotation.Pitch + ElevationChange;

	SetRelativeRotation(
		FRotator(
			FMath::Clamp<float>(RawNewElevation, MinElevationDegrees, MaxElevationDegrees), 
			0,
			0
		)
	);
}


