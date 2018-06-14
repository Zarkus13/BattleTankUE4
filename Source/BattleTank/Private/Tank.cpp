// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "TankAimingComponent.h"
#include "Projectile.h"
#include "Engine/World.h"


// Sets default values
ATank::ATank()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();

	TankAimingComponent = FindComponentByClass<UTankAimingComponent>();
}

void ATank::Fire()
{
	if (!ensure(ProjectileBlueprint  && TankAimingComponent)) return;

	TankAimingComponent->Fire(ProjectileBlueprint);
}

void ATank::AimAt(FVector HitLocation) {
	if (!ensure(TankAimingComponent)) return;

	TankAimingComponent->AimAt(HitLocation);
}