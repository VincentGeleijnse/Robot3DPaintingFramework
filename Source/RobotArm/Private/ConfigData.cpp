// Fill out your copyright notice in the Description page of Project Settings.


#include "ConfigData.h"

// Sets default values
AConfigData::AConfigData()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AConfigData::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AConfigData::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AConfigData::ConfigDataWrite(FString CanvasName, FVector RobotOrigin, FVector CanvasOrigin, FVector CanvasBounds, bool bWrapping, float OffsetYProj, float OffsetZProj,
	float OffsetXWrap, float OffsetYWrap, float wrapScale, float SplineScale, int maxIterCCD, int maxIterPINV, int maxRandomCCD, int maxRandomPINV, int outRangeErrs, int iterationsExceeded, int IKsucCount,
	float TargetErr, bool bJointConstr, bool bCCD, bool bPINV, float size, float speed, int origWidth, int origHeight, int colorAmt, int finFrames, FString startTime, FString stopTime, FString OutputPath)
{

	FString FilePath = FPaths::ProjectContentDir();
	FilePath.Append(TEXT("/step1-2/Result/ConfigData.csv"));
	TArray<FString> StringsToWrite;
	FString StringToWrite = "\r\n";


	//StringsToWrite.Init("", 14);
	//StringsToWrite[0] = TEXT("bWrapping=");
	StringToWrite.Append(CanvasName);
	StringToWrite.Append(TEXT(","));
	StringToWrite.Append(FString::Format(TEXT("{0},{1},{2},"), {RobotOrigin.X, RobotOrigin.Y, RobotOrigin.Z}));
	StringToWrite.Append(FString::Format(TEXT("{0},{1},{2},"), { CanvasOrigin.X, CanvasOrigin.Y, CanvasOrigin.Z }));
	StringToWrite.Append(FString::Format(TEXT("{0},{1},{2},"), { CanvasBounds.X, CanvasBounds.Y, CanvasBounds.Z }));
	StringToWrite.Append(bWrapping ? TEXT("True") : TEXT("False"));
	StringToWrite.Append(TEXT(","));
	//StringsToWrite[1] = TEXT("OffsetYProj=");
	StringToWrite.Append(FString::SanitizeFloat(OffsetYProj));
	StringToWrite.Append(TEXT(","));
	//StringsToWrite[2] = TEXT("OffsetXProj=");
	StringToWrite.Append(FString::SanitizeFloat(OffsetZProj));
	StringToWrite.Append(TEXT(","));
	//StringsToWrite[3] = TEXT("OffsetXWrap=");
	StringToWrite.Append(FString::SanitizeFloat(OffsetXWrap));
	StringToWrite.Append(TEXT(","));
	//StringsToWrite[4] = TEXT("OffsetYWrap=");
	StringToWrite.Append(FString::SanitizeFloat(OffsetYWrap));
	StringToWrite.Append(TEXT(","));
	//StringsToWrite[5] = TEXT("WrapScale=");
	StringToWrite.Append(FString::SanitizeFloat(wrapScale));
	StringToWrite.Append(TEXT(","));
	//StringsToWrite[6] = TEXT("SplineScale=");
	StringToWrite.Append(FString::SanitizeFloat(SplineScale));
	StringToWrite.Append(TEXT(","));
	//StringsToWrite[7] = TEXT("MaxIter=");
	StringToWrite.Append(FString::FromInt(maxIterCCD));
	StringToWrite.Append(TEXT(","));
	StringToWrite.Append(FString::FromInt(maxIterPINV));
	StringToWrite.Append(TEXT(","));
	StringToWrite.Append(FString::FromInt(maxRandomCCD));
	StringToWrite.Append(TEXT(","));
	StringToWrite.Append(FString::FromInt(maxRandomPINV));
	StringToWrite.Append(TEXT(","));
	StringToWrite.Append(FString::FromInt(outRangeErrs));
	StringToWrite.Append(TEXT(","));
	StringToWrite.Append(FString::FromInt(iterationsExceeded));
	StringToWrite.Append(TEXT(","));
	StringToWrite.Append(FString::FromInt(IKsucCount));
	StringToWrite.Append(TEXT(","));
	//StringsToWrite[8] = TEXT("TargetError=");
	StringToWrite.Append(FString::SanitizeFloat(TargetErr));
	StringToWrite.Append(TEXT(","));
	StringToWrite.Append(bJointConstr ? "True" : "False");
	StringToWrite.Append(TEXT(","));
	//StringsToWrite[9] = TEXT("bCCD=");
	StringToWrite.Append(bCCD ? "True" : "False");
	StringToWrite.Append(TEXT(","));
	//StringsToWrite[10] = TEXT("bPINV=");
	StringToWrite.Append(bPINV ? "True" : "False");
	StringToWrite.Append(TEXT(","));
	//StringsToWrite[11] = TEXT("Size=");
	StringToWrite.Append(FString::SanitizeFloat(size));
	StringToWrite.Append(TEXT(","));
	//StringsToWrite[12] = TEXT("Speed=");
	StringToWrite.Append(FString::SanitizeFloat(speed));
	StringToWrite.Append(TEXT(","));
	StringToWrite.Append(FString::FromInt(origWidth));
	StringToWrite.Append(TEXT(","));
	StringToWrite.Append(FString::FromInt(origHeight));
	StringToWrite.Append(TEXT(","));
	StringToWrite.Append(FString::FromInt(colorAmt));
	StringToWrite.Append(TEXT(","));
	//StringsToWrite[13] = TEXT("FramesToFin=");
	StringToWrite.Append(FString::FromInt(finFrames));
	StringToWrite.Append(TEXT(","));
	StringToWrite.Append(startTime);
	StringToWrite.Append(TEXT(","));
	StringToWrite.Append(stopTime);
	StringToWrite.Append(TEXT(","));
	StringToWrite.Append(OutputPath);
	StringToWrite.Append(TEXT(","));
	StringToWrite.Append(TEXT("DIFFCOUNTA"));
	StringToWrite.Append(TEXT(","));
	StringToWrite.Append(TEXT("DIFFPCTA_ALL"));
	StringToWrite.Append(TEXT(","));
	StringToWrite.Append(TEXT("DIFFPCTA_NOWHITE"));
	StringToWrite.Append(TEXT(","));
	StringToWrite.Append(TEXT("DIFFCOUNTW"));
	StringToWrite.Append(TEXT(","));
	StringToWrite.Append(TEXT("DIFFPCTW_ALL"));
	StringToWrite.Append(TEXT(","));
	StringToWrite.Append(TEXT("DIFFPCTW_NOWHITE"));
	StringToWrite.Append(TEXT(","));
	StringToWrite.Append(FDateTime::Now().ToString(TEXT("%Y-%m-%d_%H-%M")));
	//FFileHelper::SaveStringArrayToFile(StringsToWrite, *FilePath );
	FFileHelper::SaveStringToFile(StringToWrite, *FilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
}

void AConfigData::WriteTargets(TArray<FVector> armTF, TArray<FVector> penTF, TArray<FVector> armTS, TArray<FVector> penTS, TArray<int>itersTS)
{
	FString FilePath = FPaths::ProjectContentDir();
	FilePath.Append(TEXT("/step1-2/Result/TargetDataF_"));
	FilePath.Append(FDateTime::Now().ToString(TEXT("%Y-%m-%d_%H-%M")));
	FilePath.Append(TEXT(".csv"));

	FString StringToWrite;
	StringToWrite.Append(TEXT("X Arm,Y Arm,Z Arm,X Pen,Y Pen,Z Pen\r\n"));

	for (int i = 0; i < armTF.Num(); i++) {
		StringToWrite.Append(FString::Format(TEXT("{0},{1},{2},{3},{4},{5}"), { armTF[i].X,armTF[i].Y,armTF[i].Z,penTF[i].X,penTF[i].Y,penTF[i].Z }));
		StringToWrite.Append(TEXT("\r\n"));
	}
	FFileHelper::SaveStringToFile(StringToWrite, *FilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get());

	FString FilePath2 = FPaths::ProjectContentDir();
	FilePath2.Append(TEXT("/step1-2/Result/TargetDataS_"));
	FilePath2.Append(FDateTime::Now().ToString(TEXT("%Y-%m-%d_%H-%M")));
	FilePath2.Append(TEXT(".csv"));

	FString StringToWrite2;
	StringToWrite2.Append(TEXT("X Arm,Y Arm,Z Arm,X Pen,Y Pen,Z Pen, Iterations\r\n"));

	for (int i = 0; i < armTS.Num(); i++) {
		StringToWrite2.Append(FString::Format(TEXT("{0},{1},{2},{3},{4},{5},{6}"), { armTS[i].X,armTS[i].Y,armTS[i].Z,penTS[i].X,penTS[i].Y,penTS[i].Z,itersTS[i] }));
		StringToWrite2.Append(TEXT("\r\n"));
	}
	FFileHelper::SaveStringToFile(StringToWrite2, *FilePath2, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get());
	
}

void AConfigData::WriteJointData(TArray<FJointConfig> jConf, TArray<FVector> armTS, TArray<FVector> penTS )
{
	FString FilePath = FPaths::ProjectContentDir();
	FilePath.Append(TEXT("/step1-2/Result/JointData_"));
	FilePath.Append(FDateTime::Now().ToString(TEXT("%Y-%m-%d_%H-%M")));
	FilePath.Append(TEXT(".csv"));
	FString StringToWrite;
	StringToWrite.Append(TEXT("Joint 1,Joint 2, Joint 3, Joint 4, Joint 5, Joint 6, TargetXArm, TargetYArm, TargetZArm, TargetXPen, TargetYPen, TargetZPen \r\n"));
	for (int i = 0; i < jConf.Num(); i++) {
		for (int j = 0; j < jConf[i].jointAngles.Num()-1; j++) {
			StringToWrite.Append(FString::SanitizeFloat(jConf[i].jointAngles[j]));
			StringToWrite.Append(TEXT(","));
		}
		StringToWrite.Append(FString::SanitizeFloat(jConf[i].jointAngles[jConf[i].jointAngles.Num() - 1]));
		StringToWrite.Append(FString::Format(TEXT(",{0},{1},{2},{3},{4},{5}"), { armTS[i].X,armTS[i].Y,armTS[i].Z,penTS[i].X,penTS[i].Y,penTS[i].Z }));
		StringToWrite.Append(TEXT("\r\n"));
	}
	FFileHelper::SaveStringToFile(StringToWrite, *FilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get());
}

