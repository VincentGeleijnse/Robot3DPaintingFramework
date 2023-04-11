// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InverseKinematics/FK.h"
#include "CustomUEMatrix.h"


#include "IKSolver.generated.h"


class UBoxComponent;

USTRUCT(BlueprintType)
struct FJointConfig
{
	GENERATED_BODY()
public:
	TArray<float> jointAngles;
};


UCLASS()
class ROBOTARM_API AIKSolver : public AActor
{
	GENERATED_BODY()
		

	
public:	
	// Sets default values for this actor's properties
	AIKSolver();

	UPROPERTY(EditAnywhere, Category = "Components", meta = (DisplayName = "BoxComp"))
		UBoxComponent* targetCheckActor; 

	UPROPERTY(EditAnywhere, Category = "RobotConfig", meta = (DisplayName = "Workspace"))
		AActor* Workspace;

	UPROPERTY(EditAnywhere, Category = "RobotConfig", meta = (DisplayName = "Joints"))
		TArray<AJoint*> joint;  //joint actor reference
	UPROPERTY(EditAnywhere, Category = "RobotConfig", meta = (DisplayName = "End Effector"))
		AActor* endEff;  //end Effector actor reference
	UPROPERTY(EditAnywhere, Category = "RobotConfig", meta = (DisplayName = "Target (pos)"))
		FVector target1;
	UPROPERTY(EditAnywhere, Category = "RobotConfig", meta = (DisplayName = "Target (orientation)"))
		FVector target2;

	UPROPERTY(EditAnywhere, Category = "RobotConfig", meta = (DisplayName = "Target (pose)"))
		FMatrix targetPose;
	UPROPERTY(EditAnywhere, Category = "RobotConfig", meta = (DisplayName = "Number of Joints"))
		int Njoints;
	UPROPERTY(EditAnywhere, Category = "RobotConfig", meta = (DisplayName = "Axis of rotations"))
		TArray<FVector> locAxisRot; // Local Axis of Rotation per joint
	UPROPERTY(EditAnywhere, Category = "RobotConfig", meta = (DisplayName = "Max angle joint"))
		TArray<float> maxAngle;

	UPROPERTY(EditAnywhere, Category = "RobotConfig", meta = (DisplayName = "alpha"))
		TArray<double> alpha;

	UPROPERTY(EditAnywhere, Category = "RobotConfig", meta = (DisplayName = "thetaoffset"))
		TArray<double> theta;


	UPROPERTY(EditAnywhere, Category = "RobotConfig", meta = (DisplayName = "d"))
		TArray<double> d;

	UPROPERTY(EditAnywhere, Category = "RobotConfig", meta = (DisplayName = "r"))
		TArray<double> r;


	UPROPERTY(EditAnywhere, Category = "Solver Settings", meta = (DisplayName = "Enable"))
		bool bEnable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Solver Settings", meta = (DisplayName = "primary Solver (1 = CCD, 2 = PINV)"))
		int IKsolver;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Solver Settings", meta = (DisplayName = "Solver max iterations"))
		TArray<int> IKsolverIterations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Solver Settings", meta = (DisplayName = "Solver max angle Randomizations"))
		TArray<int> IKsolverRandomizeAttempts;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Solver Metrics", meta = (DisplayName = "PenTargets Failed"))
		TArray<FVector> penTargetsF;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Solver Metrics", meta = (DisplayName = "ArmTargets Failed"))
		TArray<FVector> armTargetsF;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Solver Metrics", meta = (DisplayName = "PenTargets Succ"))
		TArray<FVector> penTargetsS;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Solver Metrics", meta = (DisplayName = "ArmTargets Succ"))
		TArray<FVector> armTargetsS;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Solver Metrics", meta = (DisplayName = "Iterations to Succ"))
		TArray<int> itersSucc;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Solver Metrics", meta = (DisplayName = "ArmConfigurations"))
		TArray<FJointConfig> armConfigs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Solver Settings", meta = (DisplayName = "Use Randomization"))
		bool bRandomize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Solver Settings", meta = (DisplayName = "Use JointLimits"))
		bool bJointLimits;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Solver Settings", meta = (DisplayName = "Max Target Error"))
		float maxTargetError;
	
	//UPROPERTY(EditAnywhere, Category = "Solver Settings", meta = (DisplayName = "Max iterations"))
		//int maxIter;


	UPROPERTY(EditAnywhere)
		bool solFound;

	UPROPERTY(EditAnywhere)
		bool nextTarget;

	UPROPERTY(EditAnywhere)
		bool waitForNextTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Solver Settings", meta = (DisplayName = "Exceeded Iterations Count"))
		int targetsFailed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Solver Settings", meta = (DisplayName = "IK Solution Found Count"))
		int targetsSuccess;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Solver Settings", meta = (DisplayName = "Randomizations Count"))
		int randomizations;


	UPROPERTY()
		TArray<double> Dtheta;

	UPROPERTY()
		FRobot Robot;





protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
		void Solve(int solv, FRobot DH);

	UFUNCTION()
		bool CheckWorkSpace(FVector tar);
	UFUNCTION()
		double CCD(FRobot Robo);
	UFUNCTION()
		double CCDStep(FRobot Robo, int i); //do a CCD Step
	UFUNCTION()
		TArray<double> UpdateDHCCD(FRobot Robo, double dTh, int i, TArray<double> deltaP ); //update DH, FK and maybe update DH and FK again (if move in other direction)
	UFUNCTION()
		double UpdateDHPINV(FRobot Robo, TArray<double> dTh); //update DH, FK and maybe update DH and FK again (if move in other direction)

	UFUNCTION()
		FMatrix44d logm(FRobot Robo);

	UFUNCTION()
		double PINV(FRobot Robo);

	UFUNCTION()
	TArray<FFloatMatrix> Jacobian(FRobot Robo);
};
