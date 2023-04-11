// Fill out your copyright notice in the Description page of Project Settings.


#include "Jacobian_IK.h"

// Sets default values
AJacobian_IK::AJacobian_IK()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	UE_LOG(LogTemp, Warning, TEXT("Jacobian_IK Constructor"));
}

// Called when the game starts or when spawned
void AJacobian_IK::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Jacobian_IK BeginPlay"));
}

// Called every frame
void AJacobian_IK::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

TArray<float> AJacobian_IK::GetDeltaOrientation(int numJoints, TArray<FVector2D> jointPos, FVector2D targetPos, FVector2D endEffectorPos, TArray<float> Orientation)
{

	TArray<FVector2D> jacobianTranspose;
	jacobianTranspose.Init({ 0, 0 }, numJoints);
	FVector2D v;
	TArray<float> deltaOrient;
	deltaOrient.Init(0, numJoints);

	


	for (int32 Index = 0; Index != numJoints; ++Index)
	{
		jacobianTranspose[Index] = Orientation[Index] * (endEffectorPos - jointPos[Index]);
	}

	v = targetPos - endEffectorPos;

	for (int32 Index = 0; Index != numJoints; ++Index)
	{
		deltaOrient[Index] = (jacobianTranspose[Index].X * v.X) + (jacobianTranspose[Index].Y * v.Y);
	}

	

	return TArray<float>(deltaOrient);
}







