// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Math/Vector2D.h"
#include "GameFramework/Actor.h"
#include "Jacobian_IK.generated.h"

UCLASS(Blueprintable)
class ROBOTARM_API AJacobian_IK : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJacobian_IK();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		TArray<float> GetDeltaOrientation(int numJoints, TArray<FVector2D> jointPos, FVector2D targetPos, FVector2D endEffectorPos, TArray<float> Orientation);
		
		


	

protected:
};
