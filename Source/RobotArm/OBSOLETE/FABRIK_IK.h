// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <cmath>
#include "Kismet/KismetMathLibrary.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "GameFramework/Actor.h"
#include "FABRIK_IK.generated.h"

UCLASS(Blueprintable)
class ROBOTARM_API AFABRIK_IK : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFABRIK_IK();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		TArray<float> GetOrientation(int numJoints, TArray<FVector2D> LimbAxisPos, TArray<FVector2D> LimbEndPos, FVector2D GoalPos, FVector2D EndEffectorPos) ;
};
