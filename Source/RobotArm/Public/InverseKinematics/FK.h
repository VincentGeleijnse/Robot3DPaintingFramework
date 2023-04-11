// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DenavitHartmanConstructor.h"
#include "FK.generated.h"

/**
 * 
 */



USTRUCT(BlueprintType)
struct FRobot
{
	GENERATED_BODY()
public:
	UPROPERTY()
		int Njoints; // number of joints
	UPROPERTY()
		TArray<FVector> jPos; // joint positions (size should be Njoints)
	UPROPERTY()
		FVector ePos; // end effector position
	UPROPERTY()
		TArray<double> theta; // joint rotations; (size should be Njoints)
	UPROPERTY()
		TArray<F_DHstruct> DH; // DH parameters
	UPROPERTY()
		TArray<FMatrix> FKTransforms; // joint transforms
};

class ROBOTARM_API FK
{
public:
	FK();
	~FK();

	UFUNCTION()
		static void forwardKinematics(FRobot& Robot, FVector BasePos);
};
