// Fill out your copyright notice in the Description page of Project Settings.


#include "CropData.h"

// Sets default values
ACropData::ACropData()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACropData::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACropData::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACropData::CropDataWrite(double left, double right, double top, double bottom)
{
	FString FilePath = FPaths::ProjectContentDir();
	FilePath.Append(TEXT("/step1-2/Intermediate/CropData.txt"));
	TArray<FString> StringsToWrite;
	FString leftString = TEXT("left=");
	leftString.Append(FString::SanitizeFloat(left));
	FString rightString = TEXT("right=");
	rightString.Append(FString::SanitizeFloat(right));
	FString topString = TEXT("top=");
	topString.Append(FString::SanitizeFloat(top));
	FString bottomString = TEXT("bottom=");
	bottomString.Append(FString::SanitizeFloat(bottom));
	StringsToWrite.Add(leftString);
	StringsToWrite.Add(rightString);
	StringsToWrite.Add(topString);
	StringsToWrite.Add(bottomString);
	FFileHelper::SaveStringArrayToFile(StringsToWrite, *FilePath);
}