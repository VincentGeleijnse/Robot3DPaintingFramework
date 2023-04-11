// Fill out your copyright notice in the Description page of Project Settings.


#include "InverseKinematics/CyclicDescent.h"
#include "Joint.h"
#include "Kismet/KismetMathLibrary.h"
#include "InverseKinematics/FK.h"


// Sets default values
ACyclicDescent::ACyclicDescent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACyclicDescent::BeginPlay()
{
	Super::BeginPlay();
	iterations = 0;
	curjoint = Njoints-1;

	//curjoint = 0;

	// get all the pos vectors
	if (bEnable) {
		TArray<FVector> P;
		P.Init(FVector(0, 0, 0), Njoints);
		for (int i = 0; i < Njoints; i++) {
			P[i] = joint[i]->GetRootComponent()->GetComponentLocation();
		}
		E = endEff->GetRootComponent()->GetComponentLocation();
		FVector T = target;
		FQuat rotation;
		FVector TR = targetOri;

		Dtheta.Init(0, Njoints);
		Robot.Njoints = Njoints;
		Robot.theta.Init(0, Njoints+1);
		Robot.jPos.Init(FVector(0, 0, 0), Njoints+1);
		Robot.DH.Init({ 0 }, Njoints + 1);


		for (int i = 0; i < Njoints; i++) {
			Robot.jPos[i] = P[i];
			Robot.DH[i].theta = FMath::DegreesToRadians(theta[i]);
			Robot.DH[i].alpha = FMath::DegreesToRadians(alpha[i]);
			Robot.DH[i].d = d[i];
			Robot.DH[i].r = r[i];
			Robot.theta[i] = joint[i]->GetRootComponent()->GetRelativeRotation().Quaternion().GetTwistAngle(locAxisRot[i]);
		}
		Robot.ePos = E;
		Robot.FKTransforms.Init(FMatrix::Identity, Njoints + 1);
		FK::forwardKinematics(Robot, joint[0]->GetActorLocation());
	}
	

}

// Called every frame
void ACyclicDescent::Tick(float DeltaTime)
{

	if (bEnable && iterations % maxIter == 0) {
		DrawDebugSphere(
			GetWorld(),
			target,
			1,
			10,
			FColor(255, 255, 0),
			false,
			0.2f,
			2);



		//UE_LOG(LogTemp, Warning, TEXT("Old dist %f new dist  %f"), (EOld - target).Length(), (E - target).Length());

		EOld = E;

		Super::Tick(DeltaTime);

		
		// get all the pos vectors

		while (curjoint >= 0) {

			double dTheta = CCDStep(curjoint, Robot.FKTransforms, target);
			Robot.DH[curjoint].theta = Robot.DH[curjoint].theta + dTheta;
			Dtheta[curjoint] = FMath::Fmod(Dtheta[curjoint] + FMath::RadiansToDegrees(dTheta),360);
			float dist = FVector::Dist(Robot.FKTransforms[Robot.FKTransforms.Num() - 1].GetColumn(3), target);
			FK::forwardKinematics(Robot, joint[0]->GetActorLocation());
			float dist2 = FVector::Dist(Robot.FKTransforms[Robot.FKTransforms.Num() - 1].GetColumn(3), target);
			UE_LOG(LogTemp, Warning, TEXT("curjoint %d angle %f "),curjoint, FMath::RadiansToDegrees(dTheta));
			if (dist2 > dist) {
				Robot.DH[curjoint].theta = Robot.DH[curjoint].theta - 2*dTheta;
				FK::forwardKinematics(Robot, joint[0]->GetActorLocation());
				Dtheta[curjoint] = FMath::Fmod(  Dtheta[curjoint] - FMath::RadiansToDegrees(dTheta*2) ,360) ;
			}
			//UE_LOG(LogTemp, Warning, TEXT("DHThetas %f %f %f %f %f %f %f"), Robot.DH[0].theta, Robot.DH[1].theta, Robot.DH[2].theta, Robot.DH[3].theta, Robot.DH[4].theta, Robot.DH[5].theta, Robot.DH[6].theta);
			curjoint--;
		}

		for (int i = 0; i < Njoints; i++) {
			FVector angles = locAxisRot[i] * Dtheta[i];
			joint[i]->GetRootComponent()->SetRelativeRotation(FRotator(angles.Y, angles.Z, angles.X));
				
		}
		curjoint = Njoints - 1;

	}
	iterations++;
	

}



//Get pos from Q instead of FK, we are missing the 0 0 0 pos for the first joint eh.
double ACyclicDescent::CCDStep(int i, TArray<FMatrix> FK, FVector tar) {
	FVector Eff = FK[FK.Num() - 1].GetColumn(3); // end effector pos
	FVector P_i;
	FMatrix mat;
	FMatrix Ax = FMatrix::Identity;
	if (i == 0){
		P_i = joint[0]->GetActorLocation();
		mat = FK[i];
		mat.SetColumn(3, FVector(0, 0, 0));
	}
	else {
		P_i = FK[i - 1].GetColumn(3); //joint pos;
		mat = FK[i];
		mat.SetColumn(3, FVector(0, 0, 0));
	}
	
	FVector e_i = Eff - P_i; //vector from Joint to End Effector
	FVector t_i = tar - P_i; //vector from Joint to Target
	
	Ax.SetColumn(3, locAxisRot[i]); //axis of joint in local space
	FVector jAx = (mat * Ax).GetColumn(3); //Axis of joint (in world space)
	FVector proje_iAx = (e_i.Dot(jAx) / (FMath::Square(jAx.Length()))) * jAx; // projection of e_i onto normal vector to joint plane (this being the joint axis)
	FVector projt_iAx = (t_i.Dot(jAx) / (FMath::Square(jAx.Length()))) * jAx; // projection of t_i onto normal vector to joint plane (this being the joint axis)
	FVector proje_i = (e_i - proje_iAx); //projection of e_i onto joint plane
	FVector projt_i = (t_i - projt_iAx); //projection of t_i onto joint plane
	proje_i.Normalize();
	projt_i.Normalize();
	
	double angle = FMath::Acos(FVector::DotProduct(proje_i, projt_i)/(proje_i.Length()*projt_i.Length())); //angle between projected vectors gives joint rotation.
	UE_LOG(LogTemp, Warning, TEXT("E %s P_i %s mat%s axvec %s ax %s e_i %s, t_i %s, proje_i %s, projt_i %s"),*Eff.ToString(),*P_i.ToString(),*mat.ToString(), *jAx.ToString(),*Ax.ToString(), *e_i.ToString(), *t_i.ToString(), *proje_i.ToString(), *projt_i.ToString());
	return angle;
}
