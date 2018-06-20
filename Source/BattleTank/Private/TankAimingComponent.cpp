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


void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentAmmo = MaximumAmmo;
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

	UpdateFiringState(Direction);

	Barrel->Elevate(DeltaRotator.Pitch);

	RotateTurret(DeltaRotator.Yaw);
}

		void UTankAimingComponent::UpdateFiringState(FVector Direction) {
			if (FiringState == EFiringState::OutOfAmmo)
				return;
			else if ((FPlatformTime::Seconds() - LastFireTime) < ReloadTimeInSeconds)
				FiringState = EFiringState::Reloading;
			else if (IsTurretNotMoving(Direction))
				FiringState = EFiringState::Locked;
			else
				FiringState = EFiringState::Aiming;
		}

		bool UTankAimingComponent::IsTurretNotMoving(FVector Direction) {
			if (!ensure(Barrel)) { return false; }
			auto BarrelForward = Barrel->GetForwardVector();
			return BarrelForward.Equals(Direction, 0.01); // vectors are equal
		}

		void UTankAimingComponent::RotateTurret(float Yaw) const
		{
			if (FMath::Abs(Yaw) < 180)
				Turret->Rotate(Yaw);
			else
				Turret->Rotate(-Yaw);
		}



void UTankAimingComponent::Fire() {
	if (!ensure(ProjectileBlueprint)) return;

	if (FiringState != EFiringState::OutOfAmmo && FiringState != EFiringState::Reloading) {
		SpawnProjectile()->LaunchProjectile(LaunchSpeed);

		LastFireTime = FPlatformTime::Seconds();

		HandleAmmoNumber();
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

		void UTankAimingComponent::HandleAmmoNumber()
		{
			if (CurrentAmmo > 0)
				CurrentAmmo--;

			if (CurrentAmmo <= 0)
				FiringState = EFiringState::OutOfAmmo;
		}



// GETTERS AND SETTERS

EFiringState UTankAimingComponent::GetFiringState() const
{
	return FiringState;
}