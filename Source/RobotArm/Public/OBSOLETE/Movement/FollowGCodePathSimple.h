// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FollowGCodePathSimple.generated.h"

class AImportGCode;
class UProjectSurfaceInfo;



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROBOTARM_API UFollowGCodePathSimple : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties

	AImportGCode* pathActor;
	UProjectSurfaceInfo* project;

	float currPos = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed")
		float speed = 250;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enable Drawing")
		bool bEnable = false;

	FVector directionVector;
	bool targetReached = true;
	int i = 0;




	UFollowGCodePathSimple();

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
