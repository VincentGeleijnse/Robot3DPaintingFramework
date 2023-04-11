// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FollowGCodePathRobot.generated.h"

class AImportGCode;
class UProjectSurfaceInfo;
class ACyclicDescent;
class AIKSolver;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ROBOTARM_API UFollowGCodePathRobot : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties

	//AImportGCode* pathActor;
	UProjectSurfaceInfo* project;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed")
		float speed = 250;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enable Drawing")
		bool bEnable = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GCode")
		AImportGCode* pathActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CCD")
		ACyclicDescent* CCDOri;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CCD")
		ACyclicDescent* CCDPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CCD")
		AIKSolver* IKSolver;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CCD")
		float TargetAccuracy = 0.5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CCD")
		AActor* penTip;

	//section for the wrapping code.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DrawingType")
		bool Wrapping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite )
		TArray<FVector2D> UVs;
	UPROPERTY(EditAnywhere, BlueprintReadWrite )
		TArray<FVector> Vertices;
	UPROPERTY(EditAnywhere, BlueprintReadWrite )
		TArray<int32> Triangles;
	UPROPERTY(EditAnywhere, BlueprintReadWrite )
		TArray<FVector> Normals;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector canvasScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector canvasPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wrap Settings")
		float WrapScale; //make this small like 0.001

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wrap Settings")
		float xOffset; //0-1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wrap Settings")
		float yOffset; //0-1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Proj Settings")
		float yOffsetProj; //0-1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Proj Settings")
		float zOffsetProj; //0-1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Proj Settings")
		float scaleProj; //0-1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Params")
		int frameCounter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Params")
		float timer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Params")
		bool bDrawingCompleted;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Params")
		float maxTargetDist;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Params")
		float currPos = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Params")
		float splineLen = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Params")
		int currentSplinePoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Metrics")
		int OutOfRangeCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Metrics")
		int CoordinateNotMappedCount;

	FVector directionVector;
	bool targetReached = true;
	bool targetSet = false;
	int triIndexOld;

	float splineInputKeyOld;

	int i = 0;



	UFollowGCodePathRobot();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Canvas")
		AActor* canvas;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Canvas")
		int color = 0;

};
