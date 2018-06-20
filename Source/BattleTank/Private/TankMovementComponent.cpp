// Fill out your copyright notice in the Description page of Project Settings.

#include "TankMovementComponent.h"
#include "TankTrack.h"

void UTankMovementComponent::Initialize(UTankTrack* LeftTrack, UTankTrack* RightTrack) {
	if (!ensure(LeftTrack && RightTrack))
		return;

	this->LeftTrack = LeftTrack;
	this->RightTrack = RightTrack;
}

void UTankMovementComponent::IntendMoveForward(float Throw) {
	if (!ensure(LeftTrack && RightTrack))
		return;

	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(Throw);
}

void UTankMovementComponent::IntendRotate(float Force)
{
	if (!ensure(LeftTrack && RightTrack))
		return;

	LeftTrack->SetThrottle(Force);
	RightTrack->SetThrottle(-Force);
}

void UTankMovementComponent::RequestDirectMove(const FVector & MoveVelocity, bool bForceMaxSpeed)
{
	auto TankForward = GetOwner()->GetActorForwardVector().GetSafeNormal();
	auto AIForwardIntention = MoveVelocity.GetSafeNormal();

	IntendRotate(
		FVector::CrossProduct(TankForward, AIForwardIntention).Z
	);

	IntendMoveForward(
		FVector::DotProduct(TankForward, AIForwardIntention)
	);
}
