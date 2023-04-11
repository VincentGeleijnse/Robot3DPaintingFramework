// Fill out your copyright notice in the Description page of Project Settings.


#include "InverseKinematics/DenavitHartmanConstructor.h"

// Sets default values
ADenavitHartmanConstructor::ADenavitHartmanConstructor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADenavitHartmanConstructor::BeginPlay()
{
	Super::BeginPlay();
	DHparams.Init({ 0 }, 6);
	TArray<AActor*> childActors;
	TArray<AActor*> childActors2;
	AJoint* currentJoint = firstJoint;
	DHparams[0].origin = firstJoint->GetActorLocation();

	if (currentJoint->bX) {
		currentJoint->xAx = -currentJoint->GetActorRightVector();
		
		currentJoint->yAx = currentJoint->GetActorUpVector();
		DHparams[0].theta = FMath::DegreesToRadians(currentJoint->GetActorRotation().Roll);
	}
	else if (currentJoint->bY) {
		currentJoint->xAx = -currentJoint->GetActorForwardVector();
		currentJoint->yAx = currentJoint->GetActorUpVector();
		DHparams[0].theta = FMath::DegreesToRadians(currentJoint->GetActorRotation().Pitch);
	}
	else if (currentJoint->bZ) {
		currentJoint->xAx = -currentJoint->GetActorForwardVector();
		currentJoint->yAx = currentJoint->GetActorRightVector();
		DHparams[0].theta = FMath::DegreesToRadians(currentJoint->GetActorRotation().Yaw);
	}
	DHparams[0].xDir = currentJoint->xAx;
	DHparams[0].yDir = currentJoint->yAx;
	

	//UE_LOG(LogTemp, Warning, TEXT(" joint %d, xdir %f %f %f ydir %f %f %f zdir %f %f %f"), 0, DHparams[0].xDir.X, DHparams[0].xDir.Y, DHparams[0].xDir.Z, DHparams[0].yDir.X, DHparams[0].yDir.Y, DHparams[0].yDir.Z, DHparams[0].zDir.X, DHparams[0].zDir.Y, DHparams[0].zDir.Z);
	int jointNum = 1;
	while (jointNum < 6) {
		
		currentJoint->GetAttachedActors(childActors,true);
		
		if (Cast<AJoint>(childActors[0])->IsValidLowLevel()) {
			GetX(currentJoint, Cast<AJoint>(childActors[0]), jointNum);
			//currentJoint->yAx = FVector::CrossProduct(currentJoint->xAx, currentJoint->zAx);
			//DHparams[jointNum].yDir = currentJoint->yAx;
			
			//UE_LOG(LogTemp, Warning, TEXT(" joint %d, xdir %f %f %f ydir %f %f %f zdir %f %f %f"), jointNum, currentJoint->xAx.X, currentJoint->xAx.Y, currentJoint->xAx.Z, currentJoint->yAx.X, currentJoint->yAx.Y, currentJoint->yAx.Z, currentJoint->zAx.X, currentJoint->zAx.Y, currentJoint->zAx.Z);
			
			
			currentJoint = Cast<AJoint>(childActors[0]);
			jointNum++;
		}
		else {
			childActors[0]->GetAttachedActors(childActors2,true);
			if (Cast<AJoint>(childActors2[0])->IsValidLowLevel()) {
				GetX(currentJoint, Cast<AJoint>(childActors2[0]), jointNum);
				//currentJoint->yAx = FVector::CrossProduct(currentJoint->xAx, currentJoint->zAx);
				//UE_LOG(LogTemp, Warning, TEXT(" joint %d, xdir %f %f %f ydir %f %f %f zdir %f %f %f"), jointNum, currentJoint->xAx.X, currentJoint->xAx.Y, currentJoint->xAx.Z, currentJoint->yAx.X, currentJoint->yAx.Y, currentJoint->yAx.Z, currentJoint->zAx.X, currentJoint->zAx.Y, currentJoint->zAx.Z );
				//DHparams[jointNum].yDir = currentJoint->yAx;
				


				currentJoint = Cast<AJoint>(childActors2[0]);
				jointNum++;
				
			}
		}
	
		
	}

	for (int i = 0; i < 6; i++) {
		UE_LOG(LogTemp, Warning, TEXT("DH param joint %d: d %f, theta %f PI , r %f, alpha %f PI, origin %f %f %f"), i, DHparams[i].d, DHparams[i].theta/PI, DHparams[i].r, DHparams[i].alpha/PI, DHparams[i].origin.X, DHparams[i].origin.Y, DHparams[i].origin.Z);
		
		DrawDebugSphere(
			GetWorld(),
			DHparams[i].origin,
			i + 2,
			10,
			FColor(40 * i, 0, 0),
			true,
			1000,
			2
		);
		DrawDebugLine(
			GetWorld(),
			DHparams[i].origin,
			DHparams[i].origin + DHparams[i].xDir * 10,
			FColor(255, 0, 0),
			false,
			100.0f,
			2,
			1.0f);
		DrawDebugLine(
			GetWorld(),
			DHparams[i].origin,
			DHparams[i].origin + DHparams[i].yDir * 10,
			FColor(0, 255, 0),
			false,
			100.0f,
			2,
			1.0f);
		DrawDebugLine(
			GetWorld(),
			DHparams[i].origin,
			DHparams[i].origin + DHparams[i].zDir * 10,
			FColor(0, 0, 255),
			false,
			100.0f,
			2,
			1.0f);
	}


}

// Called every frame
void ADenavitHartmanConstructor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//GetX(firstJoint, TestJoint, 1);

}

void ADenavitHartmanConstructor::GetX(AJoint* a, AJoint* b, int jointNum)
{
	if (a->bX){
		a->zAx = a->GetActorForwardVector();}
	else if (a->bY){
		a->zAx = a->GetActorRightVector();}
	else if (a->bZ) {
		a->zAx = a->GetActorUpVector();}
	DHparams[jointNum-1].zDir = a->zAx;

	if (b->bX){
		b->zAx = b->GetActorForwardVector();}
	else if (b->bY){
		b->zAx = b->GetActorRightVector();}
	else if (b->bZ){
		b->zAx = b->GetActorUpVector();}
	DHparams[jointNum].zDir = b->zAx;

	FVector V = a->zAx;
	FVector W = b->zAx;
	FVector Pp = a->GetActorLocation();
	FVector Qp = b->GetActorLocation();
	float lambdaA = ((V - V.Dot(W)*W).Dot(Qp-Pp))/(1-FMath::Square(V.Dot(W)));
	float lambdaB = ((-W + V.Dot(W) * V).Dot(Qp - Pp)) / (1 - FMath::Square(V.Dot(W)));
	FVector Posa = Pp + lambdaA * V;
	FVector Posb = Qp + lambdaB * W;
	FVector cNDir = FVector::CrossProduct(V, W);
	
	
	//Zi and Zi-1 are parallel
	if (FVector::PointsAreNear(a->zAx, b->zAx,0.01) || FVector::PointsAreNear(a->zAx, -b->zAx, 0.01)) {
		
		
		
		//find equation for vector perpendicular to Z axis line going through oi-1 (this should gives b->xAx as direction vector)
		//line Qp+W
		//Point DHparams[jointNum-1].origin

		FVector R = DHparams[jointNum - 1].origin;

		FVector L = Qp + W;

		//FVector X = QP+ t*W for some t in Real  

		//FVector RX = QP-R+t*W   //points along the line K which we want to find

		//since K is perpendicular to L
		//RX is then perpendicular to W)
		//Therefore RX.Dot(W) == 0
		// QP-r.Dot(W) + t*W.Dot(W) = 0
		// t =  -(QP-r.Dot(W))/W.Dot(W)
		// therefore the vector we are looking for is t*W

		float t = -((Qp - R).Dot(W)) / W.Dot(W);

		b->xAx = (Qp-R+t * W);
		b->xAx.Normalize();

		DHparams[jointNum].origin = Qp + t * W;
		//b->xAx = a->xAx; // find a way to make b->xax such that d = 0? 

		DHparams[jointNum].xDir = b->xAx;
		DHparams[jointNum].alpha = 0;
		DHparams[jointNum].d = 0;
		DHparams[jointNum].r = FVector::Dist(DHparams[jointNum].origin, DHparams[jointNum - 1].origin);
		

		UE_LOG(LogTemp, Warning, TEXT("joint %d's Zi-1 parallel with joint %d's Zi"),  jointNum - 1, jointNum);
	}
	//Zi-1 intersects Zi
	else if (FVector::PointsAreNear(Posa, Posb, 0.01)) {
		DHparams[jointNum].d = FVector::Distance(Posb, DHparams[jointNum - 1].origin);
		b->xAx = cNDir;
		b->xAx.Normalize();
		DHparams[jointNum].xDir = b->xAx;
		DHparams[jointNum].origin = Posb;

		DHparams[jointNum].alpha = FMath::Acos(a->zAx.Dot(b->zAx) /
			((FMath::Sqrt(FMath::Square(a->zAx.X) + FMath::Square(a->zAx.Y) + FMath::Square(a->zAx.Z))) +
				(FMath::Sqrt(FMath::Square(b->zAx.X) + FMath::Square(b->zAx.Y) + FMath::Square(b->zAx.Z)))));

		UE_LOG(LogTemp, Warning, TEXT("joint %d's  Zi-1 intersects joint %d's Zi  "),jointNum-1, jointNum);
		//UE_LOG(LogTemp, Warning, TEXT("Zi intersect azax  %f %f %f bzax %f %f %f  posA %f %f %f posb %f %f %f"), a->zAx.X, a->zAx.Y, a->zAx.Z, b->zAx.X, b->zAx.Y, b->zAx.Z, Posa.X, Posa.Y, Posa.Z, Posb.X, Posb.Y, Posb.Z);



		DHparams[jointNum].r = 0;
	}
	//zi and zi-1 are not coplanar
	else {
		
		b->xAx = cNDir;
		b->xAx.Normalize();
		DHparams[jointNum].xDir = b->xAx;
		DHparams[jointNum].alpha = a->zAx.Dot(b->zAx) /
			((FMath::Sqrt(FMath::Square(a->zAx.X) + FMath::Square(a->zAx.Y) + FMath::Square(a->zAx.Z))) +
			(FMath::Sqrt(FMath::Square(b->zAx.X) + FMath::Square(b->zAx.Y) + FMath::Square(b->zAx.Z))));
		DHparams[jointNum].d = FVector::Dist(DHparams[jointNum-1].origin, Posa);
		DHparams[jointNum].r = FVector::Dist(Posb, Posa);
		DHparams[jointNum].origin = Posb;

		//UE_LOG(LogTemp, Warning, TEXT("Zi not coplanar azax  %f %f %f bzax %f %f %f  posA %f %f %f posb %f %f %f"), a->zAx.X, a->zAx.Y, a->zAx.Z, b->zAx.X, b->zAx.Y, b->zAx.Z, Posa.X, Posa.Y, Posa.Z, Posb.X, Posb.Y, Posb.Z);

		UE_LOG(LogTemp, Warning, TEXT("joint %d's Zi-1 not coplanar with joint %d's Zi "), jointNum - 1, jointNum);
	}

	/*DHparams[jointNum - 1].theta = FMath::Acos(a->xAx.Dot(b->xAx) /
		((FMath::Sqrt(FMath::Square(a->xAx.X) + FMath::Square(a->xAx.Y) + FMath::Square(a->xAx.Z))) +
		(FMath::Sqrt(FMath::Square(b->xAx.X) + FMath::Square(b->xAx.Y) + FMath::Square(b->xAx.Z)))));*/


	FVector c = a->xAx - a->zAx * (a->xAx.Dot(a->zAx));
	FVector d = b->xAx - a->zAx * (b->xAx.Dot(a->zAx));
	DHparams[jointNum-1].theta = FMath::Acos((c.Dot(d)) / (c.Size() * d.Size()));
	//DHparams[jointNum -1].theta = 
	
	b->yAx = FVector::CrossProduct(b->xAx, b->zAx);
	DHparams[jointNum].yDir = b->yAx;
	
		
	DrawDebugSphere(
		GetWorld(),
		Posa,
		50,
		10,
		FColor(0, 255, 255),
		false,
		0.0f,
		2,
		30.0f);
	DrawDebugLine(
		GetWorld(),
		Posa,
		Posb,
		FColor(0, 255, 255),
		false,
		0.0f,
		2,
		30.0f);



	
}


bool ADenavitHartmanConstructor::GetIntersection(FVector fromA, FVector fromB, FVector toA, FVector toB) {


	FVector da = fromB - fromA;
	FVector db = toB - toA;
	FVector dc = toA - fromA;

	FVector crossDaDb = FVector::CrossProduct(da, db);
	float prod = crossDaDb.X * crossDaDb.X + crossDaDb.Y * crossDaDb.Y + crossDaDb.Z * crossDaDb.Z;
	if (prod == 0 || FVector::DotProduct(dc, crossDaDb) != 0)
	{

		return false;
	}
	float res = FVector::DotProduct(FVector::CrossProduct(dc, db), FVector::CrossProduct(da, db)) / prod;

	OutIntersection = fromA + da * FVector(res, res, res);

	FVector fromAToIntersectPoint = OutIntersection - fromA;
	FVector fromBToIntersectPoint = OutIntersection - fromB;
	FVector toAToIntersectPoint = OutIntersection - toA;
	FVector toBToIntersectPoint = OutIntersection - toB;
	if (FVector::DotProduct(fromAToIntersectPoint, fromBToIntersectPoint) <= 0 && FVector::DotProduct(toAToIntersectPoint, toBToIntersectPoint) <= 0)
	{

		return true;
	}

	return false;
}