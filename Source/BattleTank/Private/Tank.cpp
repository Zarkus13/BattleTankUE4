// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"

// Sets default values
ATank::ATank()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void ATank::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = StartingHealth;
}

float ATank::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	int32 DamagePoints = FPlatformMath::RoundToInt(DamageAmount);
	auto DamageTaken = FMath::Clamp<int32>(DamagePoints, 0, CurrentHealth);
	
	CurrentHealth -= DamageTaken;

	if (CurrentHealth <= 0)
		OnTankDeath.Broadcast();

	return DamageTaken;
}


// GETTERS AND SETTERS
int32 ATank::GetCurrentHealth() const {
	return CurrentHealth;
}

float ATank::GetHealthPercent() const
{
	return (float) CurrentHealth / (float) StartingHealth;
}
