// Fill out your copyright notice in the Description page of Project Settings.


#include "Simple_IK.h"

// Sets default values
ASimple_IK::ASimple_IK()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASimple_IK::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASimple_IK::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

