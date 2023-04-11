// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectSurfaceInfo.h"

// Sets default values for this component's properties
UProjectSurfaceInfo::UProjectSurfaceInfo()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


float UProjectSurfaceInfo::GetSurfaceHeight(float Y, float Z)
{
	FHitResult hit;
	GetWorld()->LineTraceSingleByChannel(hit, FVector(rayOrigin, Y, Z), FVector(rayOrigin + (bounds.X * 2)+10, Y, Z), ECollisionChannel::ECC_Visibility);
	

	return hit.ImpactPoint.X;
}

FVector UProjectSurfaceInfo::GetSurfaceNormal(float Y, float Z)
{
	FHitResult hit;
	GetWorld()->LineTraceSingleByChannel(hit, FVector(rayOrigin, Y, Z), FVector(rayOrigin + (bounds.X * 2) +10, Y, Z), ECollisionChannel::ECC_Visibility);


	return hit.ImpactNormal;
}



// Called when the game starts
void UProjectSurfaceInfo::BeginPlay()
{
	Super::BeginPlay();
	
	FVector origin;
	GetOwner()->GetActorBounds(false, origin, bounds);
	//FRotator rot = GetOwner()->GetActorRotation();
	//bounds = rot.RotateVector(bounds);
	UE_LOG(LogTemp, Warning, TEXT("ActoBoundo %f %f %f"), bounds.X, bounds.Y, bounds.Z);

	rayOrigin = origin.X - bounds.X - 5;
	// ...
	
}


// Called every frame
void UProjectSurfaceInfo::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

