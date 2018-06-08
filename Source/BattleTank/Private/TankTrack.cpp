// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"
#include "GameFramework/Actor.h"


void UTankTrack::SetThrottle(float Throttle) {
	auto ForceApplied = GetForwardVector() * TrackMaxDrivingForce * FMath::Clamp<float>(Throttle, 0, 1);
	auto ForceLocation = GetComponentLocation();
	auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());

	TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);
}