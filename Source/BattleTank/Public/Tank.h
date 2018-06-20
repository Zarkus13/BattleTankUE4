// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

class UParticleSystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTankDeath);

UCLASS()
class BATTLETANK_API ATank : public APawn
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "Setup")
	int32 CurrentHealth = StartingHealth;

	FTankDeath OnTankDeath;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UParticleSystemComponent* TankExplosion = nullptr;

private:
	ATank();

	void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	int32 StartingHealth = 100;

	float TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


public:
	UFUNCTION(BlueprintPure, Category = "Setup")
	int32 GetCurrentHealth() const;

	UFUNCTION(BlueprintPure, Category = "Setup")
	float GetHealthPercent() const;
};
