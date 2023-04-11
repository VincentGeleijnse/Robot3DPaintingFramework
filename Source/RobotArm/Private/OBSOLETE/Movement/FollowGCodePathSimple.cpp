// Fill out your copyright notice in the Description page of Project Settings.


#include "OBSOLETE/Movement/FollowGCodePathSimple.h"
#include "PathGeneration/ImportGCode.h"
#include "Components/SplineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ProjectSurfaceInfo.h"

// Sets default values for this component's properties
UFollowGCodePathSimple::UFollowGCodePathSimple()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
}


// Called when the game starts
void UFollowGCodePathSimple::BeginPlay()
{
	Super::BeginPlay();
	if (bEnable) {

		color = 0;
		// ...
		pathActor = Cast<AImportGCode>(UGameplayStatics::GetActorOfClass(GetWorld(), AImportGCode::StaticClass()));
		project = Cast<UProjectSurfaceInfo>(canvas->GetComponentByClass(UProjectSurfaceInfo::StaticClass()));
	}
	
}


// Called every frame
void UFollowGCodePathSimple::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bEnable) {
		if (pathActor->importPaths) {
			currPos += DeltaTime * speed;
			FVector splinePos = pathActor->pathSpline[color]->GetLocationAtDistanceAlongSpline(currPos, ESplineCoordinateSpace::World);
			FVector surfacePos = FVector(project->GetSurfaceHeight(splinePos.Y, splinePos.Z), splinePos.Y, splinePos.Z);
			FVector surfaceNormal = project->GetSurfaceNormal(splinePos.Y, splinePos.Z);


			float splineHeight = pathActor->pathSpline[color]->GetLocationAtDistanceAlongSpline(currPos, ESplineCoordinateSpace::World).X;
			//UE_LOG(LogTemp, Warning, TEXT("actorRot %f %f %f"));

			if (splineHeight == pathActor->pathOrigin.X) {
				FVector MarkerPos = surfaceNormal * 40 + surfacePos;
				FRotator actorRot = UKismetMathLibrary::FindLookAtRotation(MarkerPos, surfacePos);
				GetOwner()->SetActorLocationAndRotation(MarkerPos, FRotator(actorRot.Pitch - 90, actorRot.Yaw, actorRot.Roll), false);
			}
			else {
				FVector MarkerPos = surfaceNormal * 42 + surfacePos;
				FRotator actorRot = UKismetMathLibrary::FindLookAtRotation(MarkerPos, surfacePos);
				GetOwner()->SetActorLocationAndRotation(MarkerPos, FRotator(actorRot.Pitch - 90, actorRot.Yaw, actorRot.Roll), false);
			}
			if (currPos >= pathActor->pathSpline[color]->GetSplineLength() && color < pathActor->colorArr.Num() - 1) {
				currPos = 0;
				color++;
			}
		}
	}
	// ...
}

