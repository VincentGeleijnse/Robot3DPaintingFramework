// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Math/UnrealMathUtility.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "Math/Quat.h"
#include "GcodeConversion.generated.h"

UCLASS()
class ROBOTARM_API AGcodeConversion : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGcodeConversion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
		void G2ArcToCubicSpline(float X, float Y, float I, float J, TArray<FVector2D> &P, TArray<FVector2D> &Ptp, TArray<FVector2D> &Ptm);

	UFUNCTION(BlueprintCallable)
		float RadianAngleBetweenVectors2D(FVector2D A, FVector2D B);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	//
	UPROPERTY(EditAnywhere)
		float currentX;
	UPROPERTY(EditAnywhere)
		float currentY;

};
