// Fill out your copyright notice in the Description page of Project Settings.


#include "FABRIK_IK.h"

// Sets default values
AFABRIK_IK::AFABRIK_IK()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFABRIK_IK::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFABRIK_IK::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<float> AFABRIK_IK::GetOrientation(int numJoints, TArray<FVector2D> LimbAxisPos, TArray<FVector2D> LimbEndPos, FVector2D GoalPos, FVector2D EndEffectorPos)
{
	TArray<float> LimbAxisRot2;
	LimbAxisRot2.Init(0, numJoints);
	TArray<FVector2D> LimbAxisPos2 = LimbAxisPos;
	TArray<FVector2D> LimbEndPos2 = LimbEndPos;

	FVector2D EPS = { 1,1 };
	

		UE_LOG(LogTemp, Warning, TEXT("Fart"));
		FVector2D currGoal = GoalPos;

		for (int32 Index = numJoints - 1; Index >= 0; --Index)
		{
			FVector2D vecJoint = LimbAxisPos2[Index] - LimbEndPos2[Index];
			FVector2D vecGoal = LimbAxisPos2[Index] - currGoal;
			float dotProduct = FVector2D::DotProduct(vecJoint,vecGoal);
			float sqrtStuff = sqrt(pow(vecJoint.X, 2) + pow(vecJoint.Y, 2)) * sqrt(pow(vecGoal.X, 2) + pow(vecGoal.Y, 2));
			LimbAxisRot2[Index] = acos(dotProduct / sqrtStuff);
			LimbAxisPos2[Index] +=  LimbEndPos2[Index] - currGoal;

			LimbEndPos2[Index] = currGoal;
			currGoal = LimbAxisPos2[Index];

			UE_LOG(LogTemp, Warning, TEXT("%f"), LimbAxisRot2[Index]);
		}

		for (int32 Index = 0; Index != numJoints; ++Index)
		{
			if (Index - 1 > 0) {
				LimbEndPos2[Index] = LimbAxisPos2[Index] - LimbEndPos2[Index - 1]; 
				LimbAxisPos2[Index] = LimbEndPos2[Index - 1];
				UE_LOG(LogTemp, Warning, TEXT("%f, %f"), LimbEndPos2[Index].X, LimbEndPos2[Index].Y);
				UE_LOG(LogTemp, Warning, TEXT("%f, %f"), (LimbAxisPos2[Index] - LimbEndPos2[Index - 1]).X, (LimbAxisPos2[Index] - LimbEndPos2[Index - 1]).Y);
			}
		}
	
	UE_LOG(LogTemp, Warning, TEXT("%f, %f"), LimbEndPos2[numJoints - 1].X, LimbEndPos2[numJoints - 1].Y);
	return TArray<float>(LimbAxisRot2);
}

