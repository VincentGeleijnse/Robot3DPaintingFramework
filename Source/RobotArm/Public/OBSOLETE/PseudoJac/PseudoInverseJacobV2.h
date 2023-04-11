// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomUEMatrix.h"
#include "PseudoInverseJacobV2.generated.h"


//forward declarations
class AJoint;


UCLASS()
class ROBOTARM_API APseudoInverseJacobV2 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APseudoInverseJacobV2();


	// let the user specify Goal position
	UPROPERTY(EditAnywhere, Category = "RobotGoal", meta = (DisplayName = "Goal Location"))
		FVector goalP;
	UPROPERTY(EditAnywhere, Category = "RobotGoal", meta = (DisplayName = "Goal Rotation"))
		FRotator goalR;
	UPROPERTY()
		FMatrix44d goalT;
	UPROPERTY(EditAnywhere, Category = "RobotGoal", meta = (DisplayName = "Max iterations"))
		int maxIterations;
	UPROPERTY(EditAnywhere, Category = "RobotGoal", meta = (DisplayName = "v step size"))
		double vStepSize;
	UPROPERTY(EditAnywhere, Category = "RobotGoal", meta = (DisplayName = "r step size"))
		double rStepSize;
	UPROPERTY(EditAnywhere, Category = "RobotGoal", meta = (DisplayName = "theta max step"))
		double thetaMaxStep;

	// Robot Arm configuration specifics
	UPROPERTY(EditAnywhere, Category = "RobotConfig", meta = (DisplayName = "Number of Joints"))
		int Njoints; // number of joints
	UPROPERTY(EditAnywhere, Category = "RobotConfig", meta = (DisplayName = "Axis of rotations"))
		TArray<FVector> locAxisRot; // Local Axis of Rotation per joint
	UPROPERTY(EditAnywhere, Category = "RobotConfig", meta = (DisplayName = "Joints"))
		TArray<AJoint*> joint;  //joint actor reference
	UPROPERTY(EditAnywhere, Category = "RobotConfig", meta = (DisplayName = "End Effector"))
		AActor* endEff;  //end Effector actor reference

	//iterating parameters
	UPROPERTY()
		TArray<FMatrix44d> TjointG; // global joint transforms;
	UPROPERTY()
		TArray<FMatrix44d> TjointR; //joint transforms relative to previous joint (or base frame in case of first);
	UPROPERTY()
		FMatrix44d TendEffG; //global End Effector transform
	UPROPERTY()
		FMatrix44d TendEffR; //End Effector transform relative to last joint;
	UPROPERTY()
		TArray<float> thetas;
	UPROPERTY()
		int iterationCount;

	

	

	



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// jacobian takes joint angles and (relative) end effector transform 
	UFUNCTION()
		TArray<FFloatMatrix> jacobian(TArray<float> _thetas, FMatrix44d _endEffT, int _Njoints);

	UFUNCTION()
	void ApplyRotationsAnalytic( TArray<float> _thetas, int _Njoints);

	UFUNCTION()
	void ApplyRotationsSim(TArray<float> _thetas, int _Njoints);
};
