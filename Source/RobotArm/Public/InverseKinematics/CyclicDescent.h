// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InverseKinematics/FK.h"
#include "CyclicDescent.generated.h"

//forward declarations
class AJoint;


UCLASS()
class ROBOTARM_API ACyclicDescent : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACyclicDescent();

	UPROPERTY(EditAnywhere, Category = "RobotConfig", meta = (DisplayName = "Joints"))
		TArray<AJoint*> joint;  //joint actor reference
	UPROPERTY(EditAnywhere, Category = "RobotConfig", meta = (DisplayName = "End Effector"))
		AActor* endEff;  //end Effector actor reference
	UPROPERTY(EditAnywhere, Category = "RobotConfig", meta = (DisplayName = "Target"))
		FVector target;  
	UPROPERTY(EditAnywhere, Category = "RobotConfig", meta = (DisplayName = "TargetOrientationVector"))
		FVector targetOri;
	UPROPERTY(EditAnywhere, Category = "RobotConfig", meta = (DisplayName = "Enable"))
		bool bEnable;
	UPROPERTY(EditAnywhere, Category = "RobotConfig", meta = (DisplayName = "Number of Joints"))
		int Njoints;  
	UPROPERTY(EditAnywhere, Category = "RobotConfig", meta = (DisplayName = "Max iterations"))
		int maxIter; 
	UPROPERTY(EditAnywhere, Category = "RobotConfig", meta = (DisplayName = "Max Steps"))
		float maxStep;

	UPROPERTY(EditAnywhere, Category = "RobotConfig", meta = (DisplayName = "Axis of rotations"))
		TArray<FVector> locAxisRot; // Local Axis of Rotation per joint

	UPROPERTY(EditAnywhere, Category = "RobotConfig", meta = (DisplayName = "Max angle joint"))
		TArray<float> maxAngle;

	UPROPERTY(EditAnywhere, Category = "RobotConfig", meta = (DisplayName = "alpha"))
		TArray<double> alpha;

	UPROPERTY(EditAnywhere, Category = "RobotConfig", meta = (DisplayName = "thetaoffset"))
		TArray<double> theta;

	UPROPERTY()
		TArray<double> Dtheta;

	UPROPERTY(EditAnywhere, Category = "RobotConfig", meta = (DisplayName = "d"))
		TArray<double> d;

	UPROPERTY(EditAnywhere, Category = "RobotConfig", meta = (DisplayName = "r"))
		TArray<double> r;

	UPROPERTY()
		FRobot Robot;

	UPROPERTY()
		FQuat TargetQuat;

	UPROPERTY()
		FVector E;

	UPROPERTY()
		FVector EOld;

	UPROPERTY()
		int iterations;  
	UPROPERTY()
		int curjoint;  //end Effector actor reference

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
		double CCDStep(int i, TArray<FMatrix> FK, FVector tar);
};
