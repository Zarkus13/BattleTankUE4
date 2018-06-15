// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"

UTankTrack::UTankTrack() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UTankTrack::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	auto SlippageSpeed = FVector::DotProduct(GetRightVector(), GetComponentVelocity());

	auto CorrectionAcceleration = - SlippageSpeed / DeltaTime * GetRightVector();
	auto Root = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());

	auto CorrectionForce = Root->GetMass() * CorrectionAcceleration / 2;

	Root->AddForce(CorrectionForce);
}

void UTankTrack::SetThrottle(float Throttle) {
	if (Throttle != 0) {
		auto ForceApplied = GetForwardVector() * TrackMaxDrivingForce * FMath::Clamp<float>(Throttle, -1, 1);
		auto ForceLocation = GetSocketLocation("ThrottleFront");
		auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());

		TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);
	}
}