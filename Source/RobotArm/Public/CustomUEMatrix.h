// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomUEMatrix.generated.h"

USTRUCT(BlueprintType)
struct FFloatMatrix
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<float> C; //columns
};


class ROBOTARM_API CustomUEMatrix
{
public:
	CustomUEMatrix();
	~CustomUEMatrix();

	UFUNCTION()
		static TArray<FFloatMatrix> InitMatrix(int Rows, int Cols);

	UFUNCTION()
		static TArray<FFloatMatrix> TransposeMatrix(TArray<FFloatMatrix> Mat);

	UFUNCTION()
		static TArray<FFloatMatrix> MatrixProduct(TArray<FFloatMatrix> A, TArray<FFloatMatrix> B);

	UFUNCTION()
		static TArray<FFloatMatrix> MatrixAdd(TArray<FFloatMatrix> A, TArray<FFloatMatrix> B);

	UFUNCTION()
		static TArray<FFloatMatrix> MatrixClip(TArray<FFloatMatrix>M, float min, float max);
};
