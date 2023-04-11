// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Joint.h"
#include "DenavitHartmanConstructor.generated.h"

USTRUCT(BlueprintType)
struct F_DHstruct
{
	GENERATED_BODY()

	UPROPERTY()
	float d; //disance origin previous axis to common normal 
	float theta; // angle about previous z axis to align x with new origin
	float r; // length of the common normal
	float alpha; // rotates about new x axis to rotate z to axis orientation
	FVector origin;
	FVector xDir;
	FVector yDir;
	FVector zDir;

};

UCLASS()
class ROBOTARM_API ADenavitHartmanConstructor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADenavitHartmanConstructor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void GetX(AJoint* a, AJoint* b,int jointNum);
	UFUNCTION()
		bool GetIntersection(FVector fromA, FVector fromB, FVector toA, FVector toB);

	UPROPERTY(EditAnywhere, Category = "RobotArmConfig", meta = (DisplayName = "FirstJoint"))
		AJoint* firstJoint;

	FVector OutIntersection;
	TArray<F_DHstruct> DHparams;
};


