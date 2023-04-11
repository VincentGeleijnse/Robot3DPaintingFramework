// Fill out your copyright notice in the Description page of Project Settings.


#include "OBSOLETE/PathGeneration/GcodeConversion.h"

// Sets default values
AGcodeConversion::AGcodeConversion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGcodeConversion::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGcodeConversion::G2ArcToCubicSpline(float X, float Y, float I, float J, TArray<FVector2D> &P, TArray<FVector2D> &Ptp, TArray<FVector2D> &Ptm)
{
	FVector2D XY = { X-currentX,Y-currentY };
	FVector2D IJ = { I - currentX,J - currentY };
	float Phi =  RadianAngleBetweenVectors2D(XY, IJ);
	Phi = 1.5708;
	int m = 1;
	P.Init({0, 0}, m + 1);
	Ptm.Init({ 0, 0 }, m + 1);
	Ptp.Init({ 0, 0 }, m + 1);

	TArray<FVector2D> Ps;
	TArray<FVector2D> Pstm;
	TArray<FVector2D> Pstp;
	Ps.Init({ 0, 0 }, m + 1);
	Pstm.Init({ 0, 0 }, m + 1);
	Pstp.Init({ 0, 0 }, m + 1);

	float deltap = (1 - cos(Phi) / (3 * sin(Phi) * (3 + 2 * cos(Phi)))) * (2 * (4 + 3 * cos(Phi)) + sqrt(2 * (5 + 3 * cos(Phi))));
	float deltam = (1 - cos(Phi) / (3 * sin(Phi) * (3 + 2 * cos(Phi)))) * (2 * (4 + 3 * cos(Phi)) - sqrt(2 * (5 + 3 * cos(Phi))));
	for (int k = 0; k <= m; k++)
	{
		Ps[k] = { cos(k * Phi),sin(k * Phi)};
		FVector2D intP = { -sin(k * Phi), cos(k * Phi) };
		Pstp[k] =  /*Ps[k] + */deltap * intP;
		Pstm[k] =  /*Ps[k] - */ -deltam * intP;
		UE_LOG(LogTemp, Warning, TEXT("for k = %d P (%f,%f), Ptp (%f,%f), Ptm (%f,%f), Phi %f ,deltap %f, deltam %f" ), k,Ps[k].X, Ps[k].Y, Pstp[k].X, Pstp[k].Y, Pstm[k].X, Pstm[k].Y,Phi,deltap,deltam);
	}
	P = Ps;
	Ptm = Pstm;
	Ptp = Pstp;


	return;
}

float AGcodeConversion::RadianAngleBetweenVectors2D(FVector2D A, FVector2D B)
{
	return acos(FVector2D::DotProduct(A,B) /  (sqrt(pow(A.X,2)+pow(A.Y,2))*(sqrt(pow(B.X, 2) + pow(B.Y, 2)))));
}

// Called every frame
void AGcodeConversion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

