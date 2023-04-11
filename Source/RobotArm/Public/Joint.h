// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"
#include "Joint.generated.h"

UCLASS()
class ROBOTARM_API AJoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJoint();

protected:
	virtual void OnConstruction(const FTransform& Transform);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, Category = "RotationalAxis", meta = (DisplayName = "X"))
	bool bX;
	UPROPERTY(EditAnywhere, Category = "RotationalAxis", meta = (DisplayName = "Y"))
	bool bY;
	UPROPERTY(EditAnywhere, Category = "RotationalAxis", meta = (DisplayName = "Z"))
	bool bZ;
	UPROPERTY(EditAnywhere, Category = "RotationalAxis", meta = (DisplayName = "FirstJoint?"))
	bool bFirstJoint;
	UPROPERTY(EditAnywhere, Category = "StaticMesh", meta = (DisplayName = "StaticMesh"))
	UStaticMeshComponent* staticMesh;
	

	FVector xAx;
	FVector yAx;
	FVector zAx;

};
