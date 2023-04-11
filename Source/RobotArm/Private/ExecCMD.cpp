// Fill out your copyright notice in the Description page of Project Settings.


#include "ExecCMD.h"

// Sets default values
AExecCMD::AExecCMD()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AExecCMD::BeginPlay()
{
	Super::BeginPlay();
	//FString FilePath = "cd ";
	//FilePath.Append(FPaths::ProjectContentDir());
	//FilePath.Append(TEXT("\\step1-2 & WrapComparePrep.bat && ImgComp.bat"));
	//char* result = TCHAR_TO_ANSI(*FilePath);
	//system(result);
	//FString pap = "OutputProj0";
	//FString peep = " ";
	//ProjCmds(peep, pap);
}

// Called every frame
void AExecCMD::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExecCMD::WrapCmds(FString OutputPath, FString FileName, FString Canvas)
{
	FString Cmds = "cd ";
	Cmds.Append(FPaths::ProjectContentDir());
	Cmds.Append(TEXT("step1-2 & wrap.bat "));
	Cmds.Append(FileName);
	Cmds.Append(TEXT(" "));
	Cmds.Append(Canvas);
		
	char* result = TCHAR_TO_ANSI(*Cmds);
	system(result);
}

void AExecCMD::ProjCmds(FString OutputPath, FString FileName)
{
	FString Cmds = "cd ";
	Cmds.Append(FPaths::ProjectContentDir());
	Cmds.Append(TEXT("step1-2 & proj.bat "));
	Cmds.Append(FileName);
	//FString Cmds = "cmd /k cd ";
	//Cmds.Append(FPaths::ProjectContentDir());
	char* result = TCHAR_TO_ANSI(*Cmds);
	system(result);
}

void AExecCMD::genPathCMds(FString Canvas)
{
	FString Cmds = "cd ";
	Cmds.Append(FPaths::ProjectContentDir());
	Cmds.Append(TEXT("step1-2 & step1-2-v2.bat"));
	char* result = TCHAR_TO_ANSI(*Cmds);
	system(result);
}

