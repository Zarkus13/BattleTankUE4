// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

class UTankBarrel;
class UTankTurret;
class AProjectile;

UENUM()
enum class EFiringState : uint8 {
	Reloading,
	Aiming,
	Locked,
	OutOfAmmo
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTankAimingComponent();

	void AimAt(FVector HitLocation);

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Initialize(UTankBarrel* Barrel, UTankTurret* Turret);

	UFUNCTION(BlueprintCallable, Category = "Firing")
	void Fire();

	EFiringState GetFiringState() const;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Firing")
	EFiringState FiringState = EFiringState::Locked;

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	int32 MaximumAmmo = 10;

	UPROPERTY(BlueprintReadOnly, Category = "Firing")
	int32 CurrentAmmo = MaximumAmmo;

private:
	UPROPERTY(EditAnywhere, Category = "Setup")
	TSubclassOf<AProjectile> ProjectileBlueprint;

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float LaunchSpeed = 5000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float ReloadTimeInSeconds = 3.f;

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float MaximumDeltaForLockedState = 0.01f;

	double LastFireTime = 0;
	
	UTankBarrel* Barrel = nullptr;
	UTankTurret* Turret = nullptr;

	bool CalculateLaunchVelocity(FVector& LaunchVelocity, FVector StartLocation, FVector HitLocation);
	
	void MoveBarrelTowards(FVector Direction);
	void UpdateFiringState(FRotator DeltaRotator);
	bool IsTurretNotMoving(FRotator DeltaRotator);
	
	AProjectile* SpawnProjectile() const;
};
