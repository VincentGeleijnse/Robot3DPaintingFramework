// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InverseKinematics/IKSolver.h"
#include "ConfigData.generated.h"

UCLASS()
class ROBOTARM_API AConfigData : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AConfigData();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
		void ConfigDataWrite(FString CanvasName, FVector RobotOrigin, FVector CanvasOrigin, FVector CanvasBounds, bool bWrapping, float OffsetYProj, float OffsetZProj, float OffsetXWrap, float OffsetYWrap, float wrapScale, float SplineScale, int maxIterCCD, int maxIterPINV, int maxRandomCCD, int maxRandomPINV,  int outRangeErrs, int iterationsExceeded, int IKsucCount, float TargetErr, bool jointConstr, bool bCCD, bool bPINV, float size, float speed, int origWidth,int origHeight, int colorAmt , int finFrames, FString startTime, FString stopTime, FString OutputPath);

	UFUNCTION(BlueprintCallable)
		void WriteTargets(TArray<FVector> armTF, TArray<FVector> penTF, TArray<FVector> armTS, TArray<FVector> penTS, TArray<int> itersTS);
	UFUNCTION(BlueprintCallable)
		void WriteJointData(TArray<FJointConfig> jConfg, TArray<FVector> armTS, TArray<FVector> penTS);
		

};
