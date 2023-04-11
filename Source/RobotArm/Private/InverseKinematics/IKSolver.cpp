// Fill out your copyright notice in the Description page of Project Settings.


#include "InverseKinematics/IKSolver.h"
#include "Joint.h"
#include "Kismet/KismetMathLibrary.h"
#include "InverseKinematics/FK.h"
#include "Components/BoxComponent.h"

THIRD_PARTY_INCLUDES_START
#include <Eigen/QR>
#include <Eigen/Dense>
#include <Eigen/SVD>
THIRD_PARTY_INCLUDES_END

using namespace Eigen;

// Sets default values
AIKSolver::AIKSolver()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	targetCheckActor = CreateDefaultSubobject<UBoxComponent>(TEXT("Target Check Actor"));
}

// Called when the game starts or when spawned
void AIKSolver::BeginPlay()
{
	Super::BeginPlay();

	if (bEnable) {
		targetsFailed = 0;
		targetsSuccess = 0;
		targetCheckActor->SetWorldLocation(FVector(0, 0, 10000), true);
		Dtheta.Init(0, Njoints);
		Robot.Njoints = Njoints;
		Robot.theta.Init(0, Njoints + 1);
		Robot.jPos.Init(FVector(0, 0, 0), Njoints + 1);
		Robot.DH.Init({ 0 }, Njoints + 1);


		for (int i = 0; i < Njoints; i++) {
			//Robot.jPos[i] = P[i];
			Robot.DH[i].theta = FMath::DegreesToRadians(theta[i]);
			Robot.DH[i].alpha = FMath::DegreesToRadians(alpha[i]);
			Robot.DH[i].d = d[i];
			Robot.DH[i].r = r[i];
			Robot.theta[i] = joint[i]->GetRootComponent()->GetRelativeRotation().Quaternion().GetTwistAngle(locAxisRot[i]);
		}
		//Robot.ePos = E;
		Robot.FKTransforms.Init(FMatrix::Identity, Njoints + 1);
		Solve(IKsolver, Robot);
		//UE_LOG(LogTemp, Warning, TEXT("solving "));
	}

}

MatrixXd TArrayDouble2EigenMat(TArray<double> d) {
	int rows = d.Num();
	int cols = 1;
	MatrixXd ME(rows, cols);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			ME(i, j) = d[i + j];
		}
	}
	return ME;
}


TArray<double> EigenMat2TArrayDouble(MatrixXd m) {
	int rows = m.rows();
	int cols = m.cols();
	TArray<double> d;
	d.Init(0, rows * cols);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			d[i + j] = m(i, j);
		}
	}
	return d;
}

MatrixXd FFloatMatrix2EigenMat(TArray<FFloatMatrix>M) {
	int rows = M.Num();
	int cols = M[0].C.Num();
	MatrixXd ME(rows, cols);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			ME(i, j) = M[i].C[j];
		}
	}
	return ME;
}


Matrix4d FMatrix2EigenMat(FMatrix M) {
	int rows = 4;
	int cols = 4;
	Matrix4d ME(rows, cols);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			ME(i, j) = M.M[i][j];
		}
	}
	return ME;
}

FMatrix44d EigenMat2FMatrix(Matrix4d ME) {
	int rows = 4;
	int cols = 4;
	FMatrix44d M;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			M.M[i][j] = ME(i, j);
		}
	}
	return M;
}

// Called every frame
void AIKSolver::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bEnable) {
		if (solFound) {
			//UE_LOG(LogTemp, Warning, TEXT("solution found"));
			FJointConfig jConfig;
			for (int i = 0; i < Njoints; i++) {
				jConfig.jointAngles.Add(FMath::RadiansToDegrees(Robot.DH[i].theta - FMath::DegreesToRadians(theta[i])));
				FVector angles = locAxisRot[i] * FMath::RadiansToDegrees(Robot.DH[i].theta - FMath::DegreesToRadians(theta[i]));
				joint[i]->GetRootComponent()->SetRelativeRotation(FRotator(angles.Y, angles.Z, angles.X));
			}
			armConfigs.Add(jConfig);
			waitForNextTarget = true;
			solFound = false;

		}
		if (nextTarget) {
			
			nextTarget = false;
			waitForNextTarget = false;

			//shows the targets in 3D scene(SLOW!)
			/*
			DrawDebugSphere(
				GetWorld(),
				target1,
				1,
				10,
				FColor(255, 255, 0),
				false,
				0.03f,
				2);
			
			DrawDebugSphere(
				GetWorld(),
				target2,
				1,
				10,
				FColor(0, 255, 0),
				false,
				0.03f,
				2);
				*/


			

			Solve(IKsolver, Robot);
		}
	}
	
}

void AIKSolver::Solve(int solv, FRobot Arm)
{
	FK::forwardKinematics(Robot, joint[0]->GetActorLocation());
	int maxIters;
	int maxRandoms;
	double deltaP;
	double deltaPose;
	switch (solv) {
	case 1: //CCD
		maxIters = IKsolverIterations[0];
		maxRandoms = IKsolverRandomizeAttempts[0];
		break;
	case 2: //PINV
		maxIters = IKsolverIterations[1];
		maxRandoms = IKsolverRandomizeAttempts[1];
		break;
	default:
		maxIters = IKsolverIterations[0];
		maxRandoms = IKsolverRandomizeAttempts[0];
		break;
	}

	for (int j = 0; j < maxRandoms; j++) {
		for (int i = 0; i < maxIters; i++) {
			switch (solv) {
			case 1: //CCD
				deltaP = CCD(Arm);
				break;
			case 2: //PINV
				deltaPose = PINV(Arm);
				break;
			default:
				deltaP = CCD(Arm);
				break;
			}

			if (solv == 1 && deltaP < maxTargetError) { //0.05
				targetsSuccess++;
				solFound = true; //allows tick to run in order to render the solution.
				armTargetsS.Add(target1);
				penTargetsS.Add(target2);
				itersSucc.Add((maxIters * j) + i);
				return;
			}
			else if (solv == 2) {
				solFound = true; //allows tick to run in order to render the solution.
				return;
			}
			
			
		}
		if (bRandomize) {
			for (int i = 0; i < Njoints; i++) {
				Robot.DH[i].theta = FMath::DegreesToRadians(FMath::RandRange(theta[i] - maxAngle[i], theta[i] + maxAngle[i]));
			}
			FK::forwardKinematics(Robot, joint[0]->GetActorLocation());
			randomizations++;
		}
	}
	targetsFailed++;
	UE_LOG(LogTemp, Warning, TEXT("solution Not found %d"), targetsFailed);
	armTargetsF.Add(target1);
	penTargetsF.Add(target2);
	waitForNextTarget = true;
	//skip target go to next one;
}

bool AIKSolver::CheckWorkSpace(FVector tar)
{
	return(targetCheckActor->IsOverlappingActor(Workspace));
}

double AIKSolver::CCD(FRobot Arm)
{
	TArray<double> deltaP;
	deltaP.Init(0, 2);
	for (int i = Njoints - 1; i >= 0; i--) { //iterate through all joints
		double dth = CCDStep(Arm, i);
		deltaP = UpdateDHCCD(Arm, dth, i, deltaP);
	}
	

	return deltaP[0] + deltaP[1];
}

double AIKSolver::CCDStep(FRobot Arm, int i)
{
	
	FVector E, T, P_i;
	FMatrix Ax = FMatrix::Identity;
	FMatrix	JRmat = Robot.FKTransforms[i];
	JRmat.SetColumn(3, FVector(0, 0, 0));
	if (i == 0) {
		P_i = joint[0]->GetActorLocation();
	}
	else {
		P_i = Robot.FKTransforms[i - 1].GetColumn(3);
	}
	if (i < 4) {
		E = Robot.FKTransforms[4].GetColumn(3);
		T = target1;
	}
	else {
		E = Robot.FKTransforms[Robot.FKTransforms.Num()-1].GetColumn(3);
		T = target2;
	}

	FVector e_i = E - P_i; //vector from Joint to End Effector
	FVector t_i = T - P_i; //vector from Joint to Target

	Ax.SetColumn(3, locAxisRot[i]); //axis of joint in local space
	FVector jAx = (JRmat * Ax).GetColumn(3); //Axis of joint (in world space)
	FVector proje_iAx = (e_i.Dot(jAx) / (FMath::Square(jAx.Length()))) * jAx; // projection of e_i onto normal vector to joint plane (this being the joint axis)
	FVector projt_iAx = (t_i.Dot(jAx) / (FMath::Square(jAx.Length()))) * jAx; // projection of t_i onto normal vector to joint plane (this being the joint axis)
	FVector proje_i = (e_i - proje_iAx); //projection of e_i onto joint plane
	FVector projt_i = (t_i - projt_iAx); //projection of t_i onto joint plane
	proje_i.Normalize();
	projt_i.Normalize();
	double angle = FMath::Acos(FVector::DotProduct(proje_i, projt_i) / (proje_i.Length() * projt_i.Length())); //angle between projected vectors gives joint rotation.
	//UE_LOG(LogTemp, Warning, TEXT("E %s P_i %s mat%s axvec %s ax %s e_i %s, t_i %s, proje_i %s, projt_i %s"), *E.ToString(), *P_i.ToString(), *JRmat.ToString(), *jAx.ToString(), *Ax.ToString(), *e_i.ToString(), *t_i.ToString(), *proje_i.ToString(), *projt_i.ToString());
	return angle;

}

TArray<double> AIKSolver::UpdateDHCCD(FRobot Arm, double dTh, int i, TArray<double> deltaP)
{
	int split;
	FVector E, T;
	double deltaNew;
	if (i < 4) {
		T = target1;
		split = 0;
		double deltaOld = FVector::Dist(Robot.FKTransforms[4].GetColumn(3), T);
		if (bJointLimits) {
			Robot.DH[i].theta = FMath::Clamp(Robot.DH[i].theta + dTh, FMath::DegreesToRadians(theta[i] - maxAngle[i]), FMath::DegreesToRadians(theta[i] + maxAngle[i]));
			// maybe we need Dtheta[i]?
		}
		else {
			Robot.DH[i].theta = Robot.DH[i].theta + dTh;
			Dtheta[i] = FMath::Fmod(Dtheta[i] + FMath::RadiansToDegrees(dTh), 360);
		}
		
		FK::forwardKinematics(Robot, joint[0]->GetActorLocation());
		deltaNew = FVector::Dist(Robot.FKTransforms[4].GetColumn(3), T);
		if (deltaOld < deltaNew) {
			if (bJointLimits) {
				Robot.DH[i].theta = FMath::Clamp(Robot.DH[i].theta - 2 * dTh,FMath::DegreesToRadians(theta[i]-maxAngle[i]), FMath::DegreesToRadians(theta[i] + maxAngle[i]));
				// maybe we need Dtheta[i]?
			}
			else {
				Robot.DH[i].theta = Robot.DH[i].theta - 2 * dTh;
				Dtheta[i] = FMath::Fmod(Dtheta[i] - 2 * FMath::RadiansToDegrees(dTh), 360);
			}
			FK::forwardKinematics(Robot, joint[0]->GetActorLocation());
			deltaNew = FVector::Dist(Robot.FKTransforms[4].GetColumn(3), T);
		}
	}
	else {
		T = target2;
		split = 1;
		double deltaOld = FVector::Dist(Robot.FKTransforms[Robot.FKTransforms.Num()-1].GetColumn(3), T);
		if (bJointLimits) {
			Robot.DH[i].theta = FMath::Clamp(Robot.DH[i].theta + dTh, FMath::DegreesToRadians(theta[i] - maxAngle[i]), FMath::DegreesToRadians(theta[i] + maxAngle[i]));
			// maybe we need Dtheta[i]?
		}
		else {
			Robot.DH[i].theta = Robot.DH[i].theta + dTh;
			Dtheta[i] = FMath::Fmod(Dtheta[i] + FMath::RadiansToDegrees(dTh), 360);
		}
		FK::forwardKinematics(Robot, joint[0]->GetActorLocation());
		deltaNew = FVector::Dist(Robot.FKTransforms[Robot.FKTransforms.Num() - 1].GetColumn(3), T);
		if (deltaOld < deltaNew) {
			if (bJointLimits) {
				Robot.DH[i].theta = FMath::Clamp(Robot.DH[i].theta - 2 * dTh, FMath::DegreesToRadians(theta[i] - maxAngle[i]), FMath::DegreesToRadians(theta[i] + maxAngle[i]));
				// maybe we need Dtheta[i]?
			}
			else {
				Robot.DH[i].theta = Robot.DH[i].theta - 2 * dTh;
				Dtheta[i] = FMath::Fmod(Dtheta[i] - 2 * FMath::RadiansToDegrees(dTh), 360);
			}
			FK::forwardKinematics(Robot, joint[0]->GetActorLocation());
			deltaNew = FVector::Dist(Robot.FKTransforms[Robot.FKTransforms.Num() - 1].GetColumn(3), T);
		}
	}
	
	TArray<double> deltas;
	deltas = deltaP;
	deltas[split] = deltaNew;
	//UE_LOG(LogTemp, Warning, TEXT("Delta %f  %f "), deltas[0], deltas[1]);
	return deltas;
}

FMatrix44d AIKSolver::logm(FRobot Arm) {
	Matrix4d T;
	FMatrix44d E = Robot.FKTransforms[Robot.Njoints];
	T = FMatrix2EigenMat(E);
	JacobiSVD<Matrix4d> svd(T, ComputeFullU | ComputeFullV);
	Matrix4d Sigma = Matrix4d::Zero();
	Sigma.diagonal() = svd.singularValues().array().log();

	Matrix4d Res = svd.matrixU() * Sigma * svd.matrixV().transpose();
	FMatrix44d Result = EigenMat2FMatrix(Res);

	return Result;
}

double AIKSolver::PINV(FRobot Arm) {
	MatrixXd deltaThetaM(1, Robot.Njoints);
	
	MatrixXd J = FFloatMatrix2EigenMat(Jacobian(Robot));
	MatrixXd J_pinv = J.completeOrthogonalDecomposition().pseudoInverse();

	deltaThetaM = J_pinv * FMatrix2EigenMat(logm(Robot));
	


	
	TArray<double> deltaTheta = EigenMat2TArrayDouble(deltaThetaM);
	for (int i = 0; i < Robot.Njoints; i++) {
		deltaTheta[i] = deltaTheta[i] *0.01;
	}
	UE_LOG(LogTemp, Warning, TEXT("deltaThetas %f  %f  %f  %f"), deltaTheta[0], deltaTheta[1], deltaTheta[2], deltaTheta[3]);
	double error = UpdateDHPINV(Robot, deltaTheta);

	return error;
}



TArray<FFloatMatrix> AIKSolver::Jacobian(FRobot Arm) {

	TArray<FFloatMatrix> J = CustomUEMatrix::InitMatrix(6, Robot.Njoints);
	FVector p_eff = Robot.ePos;
	FVector p_eff_min_jointP;
	FVector k;
	for (int i = 0; i < Robot.Njoints; i++) {
		p_eff_min_jointP = p_eff - Robot.jPos[i];

		k = locAxisRot[i];

		FVector jacVector = FVector::CrossProduct(k, p_eff_min_jointP);
		J[0].C[i] = jacVector.X; // jacobian in form      | x1 x2 x3 x4 x5 .. |
		J[1].C[i] = jacVector.Y;  //                      | y1 y2 y3 y4 y5 .. |
		J[2].C[i] = jacVector.Z; //                       | z1 z2 z3 z4 z5 .. |
	}
	//Orientation Part
	
	for (int i = 0; i < Robot.Njoints; i++) {
		if (locAxisRot[i] == FVector(1, 0, 0)) {
			J[3].C[i] = Robot.FKTransforms[i].M[0][0];
			J[4].C[i] = Robot.FKTransforms[i].M[1][0];
			J[5].C[i] = Robot.FKTransforms[i].M[2][0];
		}
		else if (locAxisRot[i] == FVector(0, 1, 0)) {
			J[3].C[i] = Robot.FKTransforms[i].M[0][1];
			J[4].C[i] = Robot.FKTransforms[i].M[1][1];
			J[5].C[i] = Robot.FKTransforms[i].M[2][1];
		}
		else if (locAxisRot[i] == FVector(0, 0, 1)) {
			J[3].C[i] = Robot.FKTransforms[i].M[0][2];
			J[4].C[i] = Robot.FKTransforms[i].M[1][2];
			J[5].C[i] = Robot.FKTransforms[i].M[2][2];
		}
	}
	return J;
}

double AIKSolver::UpdateDHPINV(FRobot Arm, TArray<double> dTh){
	Matrix4d T_desired = FMatrix2EigenMat(targetPose);
	
	for (int i = 0; i < Robot.Njoints; i++) {
		if (bJointLimits) {
			Robot.DH[i].theta = FMath::Clamp(Robot.DH[i].theta + dTh[i], FMath::DegreesToRadians(theta[i] - maxAngle[i]), FMath::DegreesToRadians(theta[i] + maxAngle[i]));
		}
		else {
			Robot.DH[i].theta = Robot.DH[i].theta + dTh[i];
			Dtheta[i] = FMath::Fmod(Dtheta[i] + FMath::RadiansToDegrees(dTh[i]), 360);
		}
	}
	FK::forwardKinematics(Robot, joint[0]->GetActorLocation());

	Matrix4d T_error = FMatrix2EigenMat(Robot.FKTransforms[Robot.Njoints]).inverse() * T_desired;
	
	return T_error.norm();
	}

