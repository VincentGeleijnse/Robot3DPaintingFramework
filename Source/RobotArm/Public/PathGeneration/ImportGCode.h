// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ImportGCode.generated.h"

class USplineComponent;

USTRUCT(BlueprintType)
struct FGcommandStruct
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int G = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float X = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Y = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Z = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float I = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float J = 0;


};

UCLASS()
class ROBOTARM_API AImportGCode : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties




	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path")
	TArray<USplineComponent*> pathSpline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path")
		bool importPaths;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path")
		TArray<FLinearColor> colorArr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path")
		int ImageWidth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path")
		int PixelWidth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path")
		int ImageHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path")
		int PixelHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform Properties")
		float scale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform Properties")
		FVector pathOrigin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "G-CodePath")
		TArray<FGcommandStruct> Gcommands;

	AImportGCode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
		void GetNewFile();

	UFUNCTION(BlueprintCallable)
		void ImportPathSplines();

	UFUNCTION(BlueprintCallable)
		void ImportStartData();
	UPROPERTY()
		bool pathsImported;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "G-CodePath")
		bool startDataImported;
};
