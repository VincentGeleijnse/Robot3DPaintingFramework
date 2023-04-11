// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Math/UnrealMathUtility.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "Math/Quat.h"
#include "QuatRotation.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROBOTARM_API UQuatRotation : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UQuatRotation();

	UFUNCTION(BlueprintCallable)
		void RotateToDeg(float Deg);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	//
	UPROPERTY(EditAnywhere, Category = "General Properties", meta = (DisplayName = "Rotation Axis"))
		FVector rotAxis;
	UPROPERTY(EditAnywhere, Category = "General Properties", meta = (DisplayName = "Rotational SpeedD / s"))
	float rotSpeed;
	UPROPERTY(EditAnywhere, Category = "General Properties", meta = (DisplayName = "Rotation Degrees"))
		float dadegs;
	float degs;
	FQuat initialQuat;
	FQuat resultQuat;
	bool startRot;
	double tee = 0;
};
