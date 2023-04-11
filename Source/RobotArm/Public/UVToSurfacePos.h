// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UVToSurfacePos.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROBOTARM_API UUVToSurfacePos : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUVToSurfacePos();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	FVector UVToWorld(FVector UV);
		
};
