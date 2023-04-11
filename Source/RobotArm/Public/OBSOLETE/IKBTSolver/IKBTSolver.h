// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IKBTSolver.generated.h"

UCLASS()
class ROBOTARM_API AIKBTSolver : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIKBTSolver();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "thetas")
		float th1deg;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "thetas")
		float th2deg;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "thetas")
		float th3deg;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "thetas")
		float th4deg;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "thetas")
		float th5deg;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "thetas")
		float th6deg;

	UFUNCTION()
	int ikin(FTransform EndEffPos /*, double solution_list[64][6]*/);


};
