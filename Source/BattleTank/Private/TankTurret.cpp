// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTurret.h"
#include "Engine/World.h"

void UTankTurret::Rotate(float RelativeSpeed) {
	auto RotationChange = FMath::Clamp<float>(RelativeSpeed, -1.f, 1.f) * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;

	SetRelativeRotation(
		FRotator(
			0.f,
			RelativeRotation.Yaw + RotationChange,
			0.f
		)
	);
}


