// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Joint.h"
#include "CustomUEMatrix.h"

#include "PseudoJacobianv1.generated.h"








UCLASS()
class ROBOTARM_API APseudoJacobianv1 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APseudoJacobianv1();

	UPROPERTY(EditAnywhere, Category = "RobotArmConfig", meta = (DisplayName = "Joints"))
		TArray<AJoint*> Joint;

	UPROPERTY(EditAnywhere, Category = "RobotArmConfig", meta = (DisplayName = "EndEffWorld"))
		FVector EndEffWorld;

	UPROPERTY(EditAnywhere, Category = "RobotArmConfig", meta = (DisplayName = "EndEffRel"))
		FVector EndEffRel;

	UPROPERTY(EditAnywhere, Category = "RobotArmConfig", meta = (DisplayName = "EndEffAct"))
		AActor* EndEff;

	UPROPERTY(EditAnywhere, Category = "RobotArmConfig", meta = (DisplayName = "GoalPos"))
		FVector GoalPos;

	UPROPERTY(EditAnywhere, Category = "RobotArmConfig", meta = (DisplayName = "vstepsize"))
		float v_step_size;
	UPROPERTY(EditAnywhere, Category = "RobotArmConfig", meta = (DisplayName = "thetamaxstep"))
		float theta_max_step;

	UPROPERTY()
		TArray<float> Q_j;
	UPROPERTY()
		FVector p_j; // x, y, z coordinate of the position of the end effector in the global reference frame
	UPROPERTY()
		FVector delta_p;
	UPROPERTY()
		FVector p_end;

	UPROPERTY()
		FVector p_eff_Nah;

	UPROPERTY()
		int iterationCount;

	UPROPERTY()
		int max_steps;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		TArray<FFloatMatrix> axis_angle_rot_matrix(FVector k, float q);

	UFUNCTION()
		TArray<FFloatMatrix> hr_matrix(FVector k, FVector t, float q);

	UFUNCTION()
		FVector4 Matrix4x4Vector4Mult(FMatrix M, FVector4 V);
	UFUNCTION()
		FVector position(TArray<float> Q, int index, FVector p_i);  /*still have to make a robot arm class and add as arg */

	//UFUNCTION()
	//	TArray<float> pseudo_inverse(TArray<float> theta_start, FVector p_eff_N, FVector goal_position, int max_steps);

	UFUNCTION()
		TArray<FFloatMatrix> jacobian(TArray<float> Q, FVector p_eff_N);

	UFUNCTION()
		TArray<FFloatMatrix> InitMatrix(int Rows, int Cols);

	UFUNCTION()
		TArray<FFloatMatrix> TransposeMatrix(TArray<FFloatMatrix> Mat);

	UFUNCTION()
		TArray<FFloatMatrix> MatrixProduct(TArray<FFloatMatrix> A, TArray<FFloatMatrix> B);

	UFUNCTION()
		TArray<FFloatMatrix> MatrixInverse(TArray<FFloatMatrix> M);

	UFUNCTION()
		float MatrixDeterminant(TArray<FFloatMatrix> M, int n);

	UFUNCTION()
		TArray<FFloatMatrix> MatrixAdjoint(TArray<FFloatMatrix> M);

	UFUNCTION()
		TArray<FFloatMatrix> MatrixAdd(TArray<FFloatMatrix> A, TArray<FFloatMatrix> B);

	UFUNCTION()
		TArray<FFloatMatrix> MatrixCofactor(TArray<FFloatMatrix> M, int a, int b, int n);

	UFUNCTION()
		TArray<FFloatMatrix> MatrixClip(TArray<FFloatMatrix>M, float min, float max);

	UFUNCTION()
		TArray<FFloatMatrix> MatrixRowEchelon(TArray<FFloatMatrix>M);
	
	UFUNCTION()
		void ApplyTheta(TArray<AJoint*> Joints, TArray<float> Thetas);

	UFUNCTION()
		TArray<float> GetTheta(TArray<AJoint*> Joints);

	UFUNCTION()
		TArray<FVector> GetT(TArray<AJoint*> Joints);

	UFUNCTION()
		TArray<FVector> GetK(TArray<AJoint*> Joints);
		

	UPROPERTY()
		TArray<FVector> k; //A 3 element array containing the unit axis to rotate around(kx, ky, kz)
	UPROPERTY()
		TArray<FVector> t; //The translation from the current frame(e.g.Frame A) to the next frame(e.g.Frame B)



	

	


	//UPROPERTY()
		//int N_joints;
	//TArray<FVector> self. k = k_arm;
	//TArray<FVector> self.t = t_arm;

	

};
