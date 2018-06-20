// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Engine/World.h"
#include "Public/TimerManager.h"


UStaticMeshComponent* InitializeCollisionMesh(AProjectile* Projectile);
UParticleSystemComponent* InitializeLaunchBlast(AProjectile* Projectile);
UParticleSystemComponent* InitializeImpactBlast(AProjectile* Projectile);
URadialForceComponent* InitializeExplosionForce(AProjectile* Projectile);

AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile movement");
	ProjectileMovement->SetAutoActivate(false);

	CollisionMesh = InitializeCollisionMesh(this);
	LaunchBlast = InitializeLaunchBlast(this);
	ImpactBlast = InitializeImpactBlast(this);
	ExplosionForce = InitializeExplosionForce(this);
}

		UStaticMeshComponent* InitializeCollisionMesh(AProjectile* Projectile) {
			auto CollisionMesh = Projectile->CreateDefaultSubobject<UStaticMeshComponent>("Collision Mesh");
			CollisionMesh->SetNotifyRigidBodyCollision(true);
			CollisionMesh->SetVisibility(false);

			Projectile->SetRootComponent(CollisionMesh);

			return CollisionMesh;
		}

		UParticleSystemComponent* InitializeLaunchBlast(AProjectile* Projectile) {
			auto LaunchBlast = Projectile->CreateDefaultSubobject<UParticleSystemComponent>("Launch blast");
			LaunchBlast->AttachToComponent(Projectile->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

			return LaunchBlast;
		}

		UParticleSystemComponent* InitializeImpactBlast(AProjectile* Projectile) {
			auto ImpactBlast = Projectile->CreateDefaultSubobject<UParticleSystemComponent>("Impact blast");
			ImpactBlast->AttachToComponent(Projectile->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
			ImpactBlast->SetAutoActivate(false);

			return ImpactBlast;
		}

		URadialForceComponent* InitializeExplosionForce(AProjectile* Projectile) {
			auto ExplosionForce = Projectile->CreateDefaultSubobject<URadialForceComponent>("Explosion force");
			ExplosionForce->AttachToComponent(Projectile->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

			return ExplosionForce;
		}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	CollisionMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}


void AProjectile::LaunchProjectile(float LaunchSpeed)
{
	ProjectileMovement->SetVelocityInLocalSpace(FVector::ForwardVector * LaunchSpeed);
	ProjectileMovement->Activate();
}

void AProjectile::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult & Hit)
{
	LaunchBlast->Deactivate();
	ImpactBlast->Activate();
	ExplosionForce->FireImpulse();

	SetRootComponent(ImpactBlast);
	CollisionMesh->DestroyComponent();

	auto TimerHandler = FTimerHandle();

	GetWorld()->GetTimerManager().SetTimer(TimerHandler, this, &AProjectile::OnDestroy, DestroyDelay, false);
}

void AProjectile::OnDestroy() {
	Destroy();
}
