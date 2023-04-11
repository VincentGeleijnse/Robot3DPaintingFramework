// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomUEMatrix.h"

CustomUEMatrix::CustomUEMatrix()
{
}

CustomUEMatrix::~CustomUEMatrix()
{
}

TArray<FFloatMatrix> CustomUEMatrix::InitMatrix(int Rows, int Cols)
{
    TArray<FFloatMatrix> M;
    M.Init({}, Rows);
    for (int i = 0; i < Rows; i++) {
        M[i].C.Init(0.0, Cols);
    }
    return M;
}

TArray<FFloatMatrix> CustomUEMatrix::TransposeMatrix(TArray<FFloatMatrix> Mat)
{
    int rows = Mat.Num();
    int cols = Mat[0].C.Num();

    TArray<FFloatMatrix> TMat;
    TMat = InitMatrix(cols, rows);
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            TMat[c].C[r] = Mat[r].C[c];
        }
    }
    return TMat;
}

TArray<FFloatMatrix> CustomUEMatrix::MatrixProduct(TArray<FFloatMatrix> A, TArray<FFloatMatrix> B)
{
    int rowsA = A.Num();
    int rowsB = B.Num();
    int colsA = A[0].C.Num();
    int colsB = B[0].C.Num();
    TArray<FFloatMatrix> M;
    M = InitMatrix(rowsA, colsB);
    for (int i = 0; i < rowsA; i++) {
        for (int j = 0; j < colsB; j++) {
            for (int r = 0; r < rowsB; r++) {
                M[i].C[j] += A[i].C[r] * B[r].C[j];
            }
        }
    }
    return M;
}

TArray<FFloatMatrix> CustomUEMatrix::MatrixAdd(TArray<FFloatMatrix> A, TArray<FFloatMatrix> B)
{
    int rA = A.Num();
    int cA = A[0].C.Num();
    int rB = B.Num();
    int cB = B[0].C.Num();
    TArray<FFloatMatrix> C;
    C = InitMatrix(rA, cA);
    if (rA != rB || cA != cB) {
        UE_LOG(LogTemp, Warning, TEXT("'Matrix Addition failed, matrices not same size"));
        return C;
    }
    for (int r = 0; r < rA; r++) {
        for (int c = 0; c < cA; c++) {
            C[r].C[c] = A[r].C[c] + B[r].C[c];
        }
    }
    return C;
}

TArray<FFloatMatrix> CustomUEMatrix::MatrixClip(TArray<FFloatMatrix> M, float min, float max)
{
    int rows = M.Num();
    int cols = M[0].C.Num();
    TArray<FFloatMatrix> MC;
    MC = InitMatrix(rows, cols);

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            MC[r].C[c] = FMath::Clamp(M[r].C[c], min, max);
        }
    }

    return MC;
}