// Fill out your copyright notice in the Description page of Project Settings.


#include "Movement/FollowGCodePathRobot.h"
#include "PathGeneration/ImportGCode.h"
#include "Components/SplineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ProjectSurfaceInfo.h"
#include "InverseKinematics/CyclicDescent.h"
#include "InverseKinematics/IKSolver.h"

THIRD_PARTY_INCLUDES_START
#include <Eigen/QR>
#include <Eigen/Dense>
#include <Eigen/SVD>
THIRD_PARTY_INCLUDES_END

using namespace Eigen;

// Sets default values for this component's properties
UFollowGCodePathRobot::UFollowGCodePathRobot()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UFollowGCodePathRobot::BeginPlay()
{
	Super::BeginPlay();
	//initialize variables if component is enabled
	if (bEnable) {
		color = 0;
		project = Cast<UProjectSurfaceInfo>(canvas->GetComponentByClass(UProjectSurfaceInfo::StaticClass()));
		splineInputKeyOld = 0;
		triIndexOld = 0;
		frameCounter = 0;
		OutOfRangeCount = 0;
		currentSplinePoint = 0;
		timer = 0;
		bDrawingCompleted = false;
	}

}

//function to convert eigen library matrixes to to FMatrix
FMatrix44d EigenMat2FMatrixs(Matrix4d ME) {
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

//function to take position and normal to create 4x4 homogeneous transform matrix
Matrix4d create_homogeneous_transform(Vector3d position, Vector3d normal)
{
	// Create rotation matrix
	Matrix3d R = AngleAxisd(normal.norm(), normal.normalized()).toRotationMatrix();

	// Convert rotation matrix to a 4x4 matrix
	Matrix4d R_4x4 = Matrix4d::Identity();
	R_4x4.block<3, 3>(0, 0) = R;

	// Create translation matrix
	Matrix4d T = Matrix4d::Identity();
	T.block<3, 1>(0, 3) = position;

	// Concatenate rotation and translation matrices
	Matrix4d H = T * R_4x4;
	return H;
}

// Called every frame
void UFollowGCodePathRobot::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bEnable) {
		if (pathActor->pathsImported && !bDrawingCompleted) { //only follow path if gcode has been imported and parsed
			frameCounter++;
			timer += DeltaTime;
			
			float timestep = DeltaTime;
			if(IKSolver->waitForNextTarget == true){ //only execute if previous solution has been found/skipped
				targetSet = false;
				splineLen = pathActor->pathSpline[color]->GetSplineLength();
				
			}
			if (!targetSet) {
				bool targetInRange = false;	
				currPos += DeltaTime * speed;
				if (currPos > pathActor->pathSpline[color]->GetDistanceAlongSplineAtSplinePoint(currentSplinePoint + 1)) {
					currPos = pathActor->pathSpline[color]->GetDistanceAlongSplineAtSplinePoint(currentSplinePoint + 1);
					currentSplinePoint++;
				}
						
						
				FVector splinePos = pathActor->pathSpline[color]->GetLocationAtDistanceAlongSpline(currPos, ESplineCoordinateSpace::World);
				FVector surfacePos;
				FVector surfaceNormal;
				//Wrapping
				if (Wrapping) {
					float dist1 = FLT_MAX; //closest point (of the 3)
					float dist2 = FLT_MAX; //intermediate
					float dist3 = FLT_MAX; //furthest point (of the 3)
					int UVindex0 = 0;
					int UVindex1 = 0;
					int UVindex2 = 0;
					FVector2D splinePos2D = FVector2D(splinePos.Y - pathActor->pathOrigin.Y, (pathActor->ImageHeight * pathActor->scale) - (splinePos.Z - pathActor->pathOrigin.Z)) * WrapScale;
					splinePos2D = FVector2D(splinePos2D.X + xOffset, splinePos2D.Y + yOffset);
					//UE_LOG(LogTemp, Warning, TEXT("SplinePos UV %f %f"), splinePos2D.X, splinePos2D.Y);
					// we must find the overlapping triangles;
					FVector bary;

					bool triFound = false;
					int triIndex = triIndexOld;
					UVindex0 = Triangles[triIndex];
					UVindex1 = Triangles[triIndex + 1];
					UVindex2 = Triangles[triIndex + 2];
					bary = FMath::GetBaryCentric2D(splinePos2D, UVs[UVindex0], UVs[UVindex1], UVs[UVindex2]);

					if (bary.GetMax() <= 1 && bary.GetMin() >= 0) {
						//overlapping triangle found.
						triIndexOld = triIndex;
						triFound = true;
					}

					triIndex = 0;
					while (triIndex < Triangles.Num() && triFound == false)
					{
						UVindex0 = Triangles[triIndex];
						UVindex1 = Triangles[triIndex + 1];
						UVindex2 = Triangles[triIndex + 2];
						triIndex = triIndex + 3;
						bary = FMath::GetBaryCentric2D(splinePos2D, UVs[UVindex0], UVs[UVindex1], UVs[UVindex2]);

						if (bary.GetMax() <= 1 && bary.GetMin() >= 0) {
							//overlapping triangle found.
							triIndexOld = triIndex - 3;
							triFound = true;
						}

					}
					if (triIndex >= Triangles.Num()) {
						CoordinateNotMappedCount++;
						//IKSolver->nextTarget = true;
						return;
					}
					//find the world space equivalent to the UV coord 
					//Find FVector P using Barycentric coordinates
					FVector A = Vertices[UVindex0];
					FVector B = Vertices[UVindex1];
					FVector C = Vertices[UVindex2];
					FVector P = bary.X * A + bary.Y * B + bary.Z * C;
					//UE_LOG(LogTemp, Warning, TEXT("bary %s"), *bary.ToString());
					FRotator rot = canvas->GetActorRotation();
					surfacePos = rot.RotateVector((P * canvasScale)) + canvasPos;
					surfaceNormal = rot.RotateVector(Normals[UVindex1]);
					//UE_LOG(LogTemp, Warning, TEXT("surfacePos %s"), *surfacePos.ToString());

					//uncomment to show where triangle is found (SLOW)
					/*DrawDebugSphere(
						GetWorld(),
						(Vertices[UVindex0] * canvasScale) + canvasPos,
						0.5,
						10,
						FColor(0, 0, 255),
						false,
						0.03f,
						2);
					DrawDebugSphere(
						GetWorld(),
						(Vertices[UVindex1] * canvasScale) + canvasPos,
						0.5,
						10,
						FColor(0, 0, 255),
						false,
						0.03f,
						2);
					DrawDebugSphere(
						GetWorld(),
						(Vertices[UVindex2] * canvasScale) + canvasPos,
						0.5,
						10,
						FColor(0, 0, 255),
						false,
						0.03f,
						2);
					DrawDebugSphere(
						GetWorld(),
						surfacePos,
						0.5,
						10,
						FColor(255, 0, 255),
						false,
						0.03f,
						2);
						*/

				}
				//Projection 
				else {
					surfacePos = FVector(project->GetSurfaceHeight(splinePos.Y, splinePos.Z), splinePos.Y, splinePos.Z);
					surfaceNormal = project->GetSurfaceNormal(splinePos.Y, splinePos.Z);
				}
				//UE_LOG(LogTemp, Warning, TEXT("spline input key = %f"), pathActor->pathSpline[color]->FindInputKeyClosestToWorldLocation(splinePos));

				float splineHeight = pathActor->pathSpline[color]->GetLocationAtDistanceAlongSpline(currPos, ESplineCoordinateSpace::World).X;
				//UE_LOG(LogTemp, Warning, TEXT("actorRot %f %f %f"));

				if (FMath::IsNearlyEqual(splineHeight, pathActor->pathOrigin.X, 0.1)) {
					FVector MarkerPos = surfaceNormal * 11.55 + surfacePos;
					IKSolver->target1 = MarkerPos;

					if ((FVector::Dist(MarkerPos, IKSolver->joint[1]->GetActorLocation()) >= maxTargetDist)) {
						OutOfRangeCount++;
						targetInRange = false;
								
					}
					else if (FMath::IsNaN(MarkerPos.X)){
						OutOfRangeCount++;
						targetInRange = false;
					}
					else {
						targetInRange = true;
						targetSet = true;
					}
					IKSolver->target2 = surfacePos;

					Vector3d position;
					position << MarkerPos.X, MarkerPos.Y, MarkerPos.Z;
					Vector3d normal;
					normal << surfaceNormal.X  , surfaceNormal.Y  , surfaceNormal.Z  ;

					IKSolver->targetPose = EigenMat2FMatrixs(create_homogeneous_transform(position, normal));
				}
				else {
					FVector MarkerPos = surfaceNormal * 15.55 + surfacePos;
					IKSolver->target1 = MarkerPos;
					if ((FVector::Dist(MarkerPos, IKSolver->joint[1]->GetActorLocation()) >= maxTargetDist)) {
						OutOfRangeCount++;
						targetInRange = false;
				
					}
					else if (FMath::IsNaN(MarkerPos.X)) {
						OutOfRangeCount++;
						targetInRange = false;
					}
					else {
						targetInRange = true;
						targetSet = true;
					}
					IKSolver->target2 = surfaceNormal * 4 + surfacePos;
					Vector3d position;
					position << MarkerPos.X, MarkerPos.Y, MarkerPos.Z;
					Vector3d normal;
					normal << surfaceNormal.X  , surfaceNormal.Y  , surfaceNormal.Z  ;
					
					IKSolver->targetPose = EigenMat2FMatrixs(create_homogeneous_transform(position, normal));

				}
				if (currentSplinePoint >= pathActor->pathSpline[color]->GetNumberOfSplinePoints() - 2.9 && color < pathActor->colorArr.Num() - 1) {
					targetInRange = true;
					currentSplinePoint = 0;
					currPos = 0;
					color++;

					targetSet = true;
				}
				else if (currentSplinePoint >= pathActor->pathSpline[color]->GetNumberOfSplinePoints() - 2.9 && color == pathActor->colorArr.Num() - 1) {
					bDrawingCompleted = true;
					targetInRange = true;
					targetSet = true;
				}

						
			//}
				if (targetSet) {
					IKSolver->nextTarget = true;
				}
			}
		}
	}
	// ...
}

