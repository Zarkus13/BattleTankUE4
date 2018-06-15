// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"

#define OUT


// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTankAimingComponent::Initialize(UTankBarrel * Barrel, UTankTurret * Turret)
{
	this->Barrel = Barrel;
	this->Turret = Turret;
}

void UTankAimingComponent::AimAt(FVector HitLocation)
{
	if (!ensure(Barrel && Turret)) return;

	FVector LaunchVelocity;
	FVector StartLocation = Barrel->GetSocketLocation("EndCannon");

	if (CalculateLaunchVelocity(OUT LaunchVelocity, StartLocation, HitLocation))
		MoveBarrelTowards(
			LaunchVelocity.GetSafeNormal()
		);
}

		bool UTankAimingComponent::CalculateLaunchVelocity(FVector & LaunchVelocity, FVector StartLocation, FVector HitLocation)
		{
			return UGameplayStatics::SuggestProjectileVelocity(
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
		}



void UTankAimingComponent::MoveBarrelTowards(FVector Direction)
{
	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	auto AimRotator = Direction.Rotation();
	auto DeltaRotator = AimRotator - BarrelRotator;

	UpdateFiringState(DeltaRotator);

	Barrel->Elevate(DeltaRotator.Pitch);
	Turret->Rotate(DeltaRotator.Yaw);
}

		void UTankAimingComponent::UpdateFiringState(FRotator DeltaRotator) {
			if ((FPlatformTime::Seconds() - LastFireTime) < ReloadTimeInSeconds)
				FiringState = EFiringState::Reloading;
			else if (IsTurretMoving(DeltaRotator))
				FiringState = EFiringState::Locked;
			else
				FiringState = EFiringState::Aiming;
		}

		bool UTankAimingComponent::IsTurretMoving(FRotator DeltaRotator) {
			return DeltaRotator.Pitch <= MaximumDeltaForLockedState && DeltaRotator.Yaw <= MaximumDeltaForLockedState;
		}



void UTankAimingComponent::Fire() {
	if (!ensure(ProjectileBlueprint)) return;

	if ((FPlatformTime::Seconds() - LastFireTime) > ReloadTimeInSeconds) {
		SpawnProjectile()->LaunchProjectile(LaunchSpeed);

		LastFireTime = FPlatformTime::Seconds();
	}
}

		AProjectile* UTankAimingComponent::SpawnProjectile() const
		{
			return GetWorld()->SpawnActor<AProjectile>(
				ProjectileBlueprint,
				Barrel->GetSocketLocation("Projectile"),
				Barrel->GetSocketRotation("Projectile")
			);
		}

