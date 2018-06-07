// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TankBarrel.h"
#include "TankTurret.h"

#define OUT


// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTankAimingComponent::SetBarrelReference(UTankBarrel * Barrel)
{
	this->Barrel = Barrel;
}

void UTankAimingComponent::SetTurretReference(UTankTurret * Turret)
{
	this->Turret = Turret;
}

void UTankAimingComponent::AimAt(FVector HitLocation, float LaunchSpeed)
{
	if (!Barrel) return;

	UE_LOG(LogTemp, Warning, TEXT("Firing at %f"), LaunchSpeed)

	FVector LaunchVelocity;
	FVector StartLocation = Barrel->GetSocketLocation("EndCannon");

	bool SuccessCalculateLaunchVelocity = 
		UGameplayStatics::SuggestProjectileVelocity(
			this,
			OUT LaunchVelocity,
			StartLocation,
			HitLocation,
			LaunchSpeed,
			false,
			0,
			0,
			ESuggestProjVelocityTraceOption::DoNotTrace
		);

	if (SuccessCalculateLaunchVelocity)
		MoveBarrelTowards(
			LaunchVelocity.GetSafeNormal()
		);
}

void UTankAimingComponent::MoveBarrelTowards(FVector Direction) const
{
	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	auto AimRotator = Direction.Rotation();
	auto DeltaRotator = AimRotator - BarrelRotator;

	UE_LOG(LogTemp, Warning, TEXT("DeltaRotator : %s"), *DeltaRotator.ToString())

	Barrel->Elevate(DeltaRotator.Pitch);
	Turret->Rotate(DeltaRotator.Yaw);
}

