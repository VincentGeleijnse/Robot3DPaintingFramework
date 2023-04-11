// Fill out your copyright notice in the Description page of Project Settings.


#include "QuatRotation.h"

// Sets default values for this component's properties
UQuatRotation::UQuatRotation()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UQuatRotation::RotateToDeg(float Deg)
{
	FTransform initialTransform = GetOwner()->GetTransform();
	initialQuat = GetOwner()->GetRootComponent()->GetRelativeTransform().GetRotation();
	FVector revoluteAxis;
	if (rotAxis.X)
		revoluteAxis = initialQuat.GetAxisX();
	else if (rotAxis.Y)
		revoluteAxis = initialQuat.GetAxisY();
	else if(rotAxis.Z)
		revoluteAxis = initialQuat.GetAxisZ();
	double radDeg = Deg * PI / 180;
	FQuat rotatedQuat = FQuat(revoluteAxis, radDeg);
	initialQuat.Normalize();
	rotatedQuat.Normalize();
	FQuat printQuat = rotatedQuat;
	resultQuat = initialQuat * rotatedQuat;
	resultQuat.Normalize();
	degs = Deg;
	startRot = true;
	tee = 0;
	return;
}

// Called when the game starts
void UQuatRotation::BeginPlay()
{
	Super::BeginPlay();

	//RotateToDeg(dadegs);
	
	// ...
	
}


// Called every frame
void UQuatRotation::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (startRot)
	{
		
		tee += DeltaTime;
		if (rotSpeed* tee / degs >= 1) {
			startRot = false;
			tee = 0;
		}
		GetOwner()->SetActorRelativeRotation(FQuat::Slerp(GetOwner()->GetRootComponent()->GetRelativeTransform().GetRotation() , resultQuat, rotSpeed * DeltaTime));
	}


	// ...
}

