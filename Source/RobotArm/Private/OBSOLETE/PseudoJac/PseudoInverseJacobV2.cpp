// Fill out your copyright notice in the Description page of Project Settings.


#include "OBSOLETE/PseudoJac/PseudoInverseJacobV2.h"
#include "Joint.h"
#include "CustomUEMatrix.h"

THIRD_PARTY_INCLUDES_START
#include <Eigen/QR>
THIRD_PARTY_INCLUDES_END

// Sets default values
APseudoInverseJacobV2::APseudoInverseJacobV2()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}



// Called when the game starts or when spawned
void APseudoInverseJacobV2::BeginPlay()
{
	Super::BeginPlay();
	/*
	 Homogeneous Transform of goal position in the form
	 [[	r00		r01		r02		pX]
	  [	rr10	r11		r12		pY]
	  [	rr20	r21		r22		pZ]
	  [	0		0		0		1 ]
	 */
	// unreal gives us only XYZ angles and position, so we make homT like this.
	goalT = FRotationTranslationMatrix::Make(goalR,goalP).GetTransposed();
	
	//initialize the size of some matrices based on joint size;
	locAxisRot.Init(FVector(0, 0, 0), Njoints);
	TjointG.Init(FMatrix44d(), Njoints);
	TjointR.Init(FMatrix44d(), Njoints);
	thetas.Init(0.0, Njoints);
	// populate matrices with initial values.
	for (int i = 0; i < Njoints; i++) {
		//get the local axes of rotation
		if (joint[i]->bX) {
			locAxisRot[i] = FVector(1, 0, 0);
			thetas[i] = -FMath::DegreesToRadians(joint[i]->GetRootComponent()->GetRelativeRotation().Roll);
		}
		else if(joint[i]->bY)
		{
			locAxisRot[i] = FVector(0, 1, 0);
			thetas[i] = -FMath::DegreesToRadians(joint[i]->GetRootComponent()->GetRelativeRotation().Pitch);
		}
		else {
			locAxisRot[i] = FVector(0, 0, 1);
			thetas[i] = -FMath::DegreesToRadians(joint[i]->GetRootComponent()->GetRelativeRotation().Yaw);
		}
		// get the global and relative starting poses of each of the joints;
		TjointG[i] = FRotationMatrix::Make(joint[i]->GetRootComponent()->GetComponentQuat());
		TjointG[i].SetColumn(3, joint[i]->GetRootComponent()->GetComponentLocation());
		if (i == 0) {
			TjointR[i] = TjointG[i];
		}
		else {
			TjointR[i] = TjointR[i - 1] * TjointG[i];
		}
	}

	DrawDebugSphere(
		GetWorld(),
		FVector(goalP) ,
		1,
		10,
		FColor(255, 255, 0),
		true,
		100000.0f,
		2);
	

	//get global transform end effector and relative (to last joint) transform end effector
	TendEffG = FRotationMatrix::Make(endEff->GetRootComponent()->GetComponentQuat());
	TendEffG.SetColumn(3, endEff->GetRootComponent()->GetComponentLocation());
	TendEffR = TjointR[Njoints - 1] * TendEffG;

	FMatrix44d rGoal = FRotationMatrix::Make(goalR);
	FQuat rGoalQuat = rGoal.ToQuat();
	iterationCount = 0;
	UE_LOG(LogTemp, Warning, TEXT("'Last row of matrix? %s"), *TendEffG.GetColumn(3).ToString());
}

// Called every frame
void APseudoInverseJacobV2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (int i = 0; i < Njoints; i++) {
		// get the global and relative starting poses of each of the joints;
		TjointG[i] = FRotationMatrix::Make(joint[i]->GetRootComponent()->GetComponentQuat());
		TjointG[i].SetColumn(3, joint[i]->GetRootComponent()->GetComponentLocation());
		if (i == 0) {
			TjointR[i] = TjointG[i];
		}
		else {
			TjointR[i] = TjointR[i - 1] * TjointG[i];
		}
	}
	//get global transform end effector and relative (to last joint) transform end effector
	TendEffG = FRotationMatrix::Make(endEff->GetRootComponent()->GetComponentQuat());
	TendEffG.SetColumn(3, endEff->GetRootComponent()->GetComponentLocation());
	TendEffR = TjointR[Njoints - 1] * TendEffG;
	
	UE_LOG(LogTemp, Warning, TEXT("'TendEffG, %s TJointG1 %s JTjointG2 %s"), *TendEffG.ToString(), *TjointG[0].ToString(), *TjointG[1].ToString() );

	// Get Global rotation matrix end effector
	FMatrix44d RendEffG = TendEffG;
	RendEffG.SetColumn(3, FVector(0, 0, 0));
	// Get global rotation matrix Goal position
	FMatrix44d rGoal = FRotationMatrix::Make(goalR);
	
	// Get rotation matrix difference

	FMatrix44d diffRot = RendEffG * rGoal;
	double traceR = diffRot.M[0][0] + diffRot.M[1][1] + diffRot.M[2][2];
	double thetaError = acos((traceR - 1) / 2);
	

	FQuat endeffQuat = RendEffG.ToQuat();
	FQuat rGoalQuat = rGoal.ToQuat();
		double thetaDif = endeffQuat.AngularDistance(rGoalQuat);  
		//FQuat diffQuat = endeffQuat.Inverse() * rGoalQuat; //try opposite
		FQuat diffQuat = rGoalQuat.Inverse() * endeffQuat;
	//get Position error
	FVector posEndEff = TendEffG.GetColumn(3);
	FVector posGoal = goalT.GetColumn(3);

	FVector deltaP = posEndEff - posGoal;
	FVector degDIF = diffQuat.Euler();

	//Iterate until within range
	// if within range apply the joint angles for real or just every frame because easier for now
	if ((deltaP.Length() > 0.1  /* || thetaDif >0.05*/) && iterationCount < maxIterations) {
		TArray<FFloatMatrix> v_p;
		v_p = CustomUEMatrix::InitMatrix(3, 1);
		FVector vpv = deltaP *vStepSize / deltaP.Length();
		//FVector vrv = diffQuat.Rotator().Vector() * rStepSize / thetaDif; // no clue if this step will work

			v_p[0].C[0] = vpv.X;
			v_p[1].C[0] = vpv.Y;
			v_p[2].C[0] = vpv.Z;
/*
	
			v_p[3].C[0] = degDIF.X * rStepSize / degDIF.Length();
			v_p[4].C[0] = degDIF.Y * rStepSize / degDIF.Length();
			v_p[5].C[0] = degDIF.Z * rStepSize / degDIF.Length();
		
	*/
		
		
/*

		*/

 		//acquire the jacobian
		TArray<FFloatMatrix> J = jacobian(thetas, TendEffR, Njoints);
		//pseudo inverse the jacobian
		Eigen::MatrixXd JEig = Eigen::MatrixXd::Random(J.Num(), J[0].C.Num());
		for (int r = 0; r < J.Num(); r++) {
			{
				for (int c = 0; c < J[0].C.Num(); c++) {
					JEig(r, c) = J[r].C[c];
				}
			}
		}
		Eigen::MatrixXd pinv = JEig.completeOrthogonalDecomposition().pseudoInverse();
		TArray<FFloatMatrix> PINV = CustomUEMatrix::InitMatrix(J[0].C.Num(), J.Num());
		for (int r = 0; r < J[0].C.Num(); r++) {
			{
				for (int c = 0; c < J.Num(); c++) {
					PINV[r].C[c] = pinv(r, c);
				}
			}
		}
		//PINV = CustomUEMatrix::TransposeMatrix(PINV);

		TArray<FFloatMatrix> v_Q = CustomUEMatrix::MatrixProduct(PINV, v_p);
		/*
		We use clip method here so that the joint doesn't move too much. We
		*/
		TArray<FFloatMatrix> thetaM;
		thetaM = CustomUEMatrix::InitMatrix(1, Njoints);
		thetaM[0].C = thetas;
		TArray<FFloatMatrix> v_Q_clip = CustomUEMatrix::TransposeMatrix(CustomUEMatrix::MatrixClip(v_Q, -1 * thetaMaxStep, thetaMaxStep));
		TArray<FFloatMatrix> deltaTheta;
		deltaTheta = CustomUEMatrix::MatrixAdd(thetaM, v_Q_clip);
		thetas = deltaTheta[0].C;

		//this method is not good (we are stuck doing iterations only 60 times a second in Tick(), we need to compute the new rotations automatically 
		
		//ApplyRotationsAnalytic(thetas, Njoints);
		ApplyRotationsSim(thetas, Njoints); //if the whole pseudo inverse function is in tick we can use this one to render out each change every frame (good for testing)

	
		
		iterationCount++;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("'errors %d thetaErr %f, posErr %f "), iterationCount, thetaError, deltaP.Length());
	}
	UE_LOG(LogTemp, Warning, TEXT("targetQuat %s, PosQuat %s"), *rGoalQuat.ToString(), *TendEffG.ToQuat().ToString());



}

TArray<FFloatMatrix> APseudoInverseJacobV2::jacobian(TArray<float> _thetas, FMatrix44d _endEffT, int _Njoints)
{
	//Initialize the Jacobian and two sub matrices of Jacobian
	TArray<FFloatMatrix> J = CustomUEMatrix::InitMatrix(3, _Njoints); 

	// Position Part
	//Position of the end effector in global frame
	FVector p_eff = TendEffG.GetColumn(3); //EndEff->GetRootComponent()->GetComponentLocation();
	FVector p_eff_minus_this_p;
	FVector k;
	
	
	for (int i = 0; i < _Njoints; i++) {

		// Difference in the position of the end effector in the global frame
		//and this joint in the global frame
		p_eff_minus_this_p = p_eff - FVector(TjointG[i].GetColumn(3)) ;

		// Axes

		k = locAxisRot[i];
		FVector jacVector = FVector::CrossProduct(k, p_eff_minus_this_p);
		J[0].C[i] = jacVector.X; // jacobian in form      | x1 x2 x3 x4 x5 .. |
		J[1].C[i] = jacVector.Y;  //                      | y1 y2 y3 y4 y5 .. |
		J[2].C[i] = jacVector.Z; //                       | z1 z2 z3 z4 z5 .. |
	}
	
	//Orientation Part
	/*
	for (int i = 0; i < _Njoints; i++) {
		if (locAxisRot[i] == FVector(1, 0, 0)) {
			J[3].C[i] = TjointG[i].M[0][0];
			J[4].C[i] = TjointG[i].M[1][0];
			J[5].C[i] = TjointG[i].M[2][0];
		}
		else if (locAxisRot[i] == FVector(0, 1, 0)) {
			J[3].C[i] = TjointG[i].M[0][1];
			J[4].C[i] = TjointG[i].M[1][1];
			J[5].C[i] = TjointG[i].M[2][1];
		}
		else if (locAxisRot[i] == FVector(0, 0, 1)) {
			J[3].C[i] = TjointG[i].M[0][2];
			J[4].C[i] = TjointG[i].M[1][2];
			J[5].C[i] = TjointG[i].M[2][2];
		}
	}
	*/
	return J;
}

void APseudoInverseJacobV2::ApplyRotationsAnalytic(TArray<float> _thetas ,int _Njoints)
{ //this might not work since we are changing world rotations instead of relative rotations? 
	for (int i= 0; i < _Njoints; i++) {
		FQuat newQuat = FQuat(locAxisRot[i],_thetas[i]); // quat method
		FVector oldOrg;
		if (i == 0) {
			oldOrg = TjointG[i].GetColumn(3);
			TjointG[i] = FRotationMatrix::Make(newQuat ); //Quat Method
			TjointG[i].SetColumn(3, oldOrg);
			TjointR[i] = TjointG[i];
		}
		else {
			oldOrg = TjointR[i].GetColumn(3);
			TjointR[i] = FRotationMatrix::Make(newQuat); //Quat Method
			TjointR[i].SetColumn(3, oldOrg);
			FMatrix44d intermediateM = TjointR[0];
			for (int j = 1 ; j < i + 1; j++) {
				intermediateM = intermediateM * TjointR[j];
			}
			TjointG[i] = intermediateM;
		}
	}
	TendEffG = TjointG[_Njoints-1] * TendEffR;
	TendEffR = TjointR[_Njoints - 1] * TendEffG;
}

void APseudoInverseJacobV2::ApplyRotationsSim(TArray<float> _thetas, int _Njoints)
{
	for (int i = 0; i < _Njoints; i++) {
		joint[i]->SetActorRelativeRotation(FQuat(locAxisRot[i], _thetas[i])); // Quat method;
	}
}
