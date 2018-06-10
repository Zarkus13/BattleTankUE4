// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"
#include "GameFramework/Actor.h"


void UTankTrack::SetThrottle(float Throttle) {
	if (Throttle != 0) {
		auto ForceApplied = GetForwardVector() * TrackMaxDrivingForce * FMath::Clamp<float>(Throttle, -1, 1);
		auto ForceLocation = GetSocketLocation("ThrottleFront");
		auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());

		UE_LOG(LogTemp, Warning, TEXT("ForceApplied : %s"), *ForceApplied.ToString())

		TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);
	}
}