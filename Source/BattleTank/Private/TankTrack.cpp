// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"

UTankTrack::UTankTrack() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UTankTrack::BeginPlay()
{
	Super::BeginPlay();
	OnComponentHit.AddDynamic(this, &UTankTrack::OnHit);
}

void UTankTrack::SetThrottle(float Throttle) {
	CurrentThrottle = FMath::Clamp<float>(CurrentThrottle + Throttle, -1, 1);
}

void UTankTrack::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult & Hit)
{
	ApplySidewaysForce();
	DriveTrack();
	
	CurrentThrottle = 0.f;
}

		void UTankTrack::ApplySidewaysForce()
		{
			auto SlippageSpeed = FVector::DotProduct(GetRightVector(), GetComponentVelocity());
			auto DeltaTime = GetWorld()->GetDeltaSeconds();

			auto CorrectionAcceleration = -SlippageSpeed / DeltaTime * GetRightVector();
			auto Root = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());

			auto CorrectionForce = Root->GetMass() * CorrectionAcceleration / 2;

			Root->AddForce(CorrectionForce);
		}

		void UTankTrack::DriveTrack()
		{
			auto ForceApplied = GetForwardVector() * TrackMaxDrivingForce * CurrentThrottle;
			auto ForceLocation = GetSocketLocation("ThrottleFront");
			auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());

			TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);
		}
