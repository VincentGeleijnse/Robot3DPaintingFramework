// Fill out your copyright notice in the Description page of Project Settings.


#include "Joint.h"

// Sets default values
AJoint::AJoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
}

void AJoint::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	
}

// Called when the game starts or when spawned
void AJoint::BeginPlay()
{
	Super::BeginPlay();
	if (bX) {
		zAx = GetActorForwardVector();
		if (bFirstJoint) {
			xAx = -GetActorRightVector();
			yAx = GetActorUpVector();
		}
	}
	else if (bY) {
		zAx = GetActorRightVector();
		if (bFirstJoint) {
			xAx = GetActorForwardVector();
			yAx = GetActorUpVector();
		}
	}
	else if (bZ) {
		zAx = GetActorUpVector();
		if (bFirstJoint) {
			xAx = GetActorForwardVector();
			yAx = GetActorRightVector();
		}
	}
	
	
}

// Called every frame
void AJoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FHitResult a;

	TArray<AActor*> c;
	FLinearColor d = FLinearColor(0, 0, 255);
	/*if (bFirstJoint) {
		DrawDebugLine(
			GetWorld(),
			GetActorLocation() - (xAx * 100),
			GetActorLocation(),
			FColor(255, 0, 0),
			false,
			0.0f,
			0.0f,
			1.0f
		);
		DrawDebugLine(
			GetWorld(),
			GetActorLocation() - (yAx * 100),
			GetActorLocation(),
			FColor(0, 255, 0),
			false,
			0.0f,
			0.0f,
			1.0f
		);


	}*/
	
}

