// Fill out your copyright notice in the Description page of Project Settings.


#include "WriteCropData.h"

WriteCropData::WriteCropData()
{
}

void WriteCropData::CropDataWrite(double left, double right, double top, double bottom)
{
	
	FString FilePath = FPaths::ProjectContentDir();
	FilePath.Append(TEXT("/step1-2/Intermediate/CropData.txt"));
	IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();

	TArray<FString> StringsToWrite;
	StringsToWrite.Add(FString(TEXT("Left =%f"),left));
	StringsToWrite.Add(FString(TEXT("Right =%f"), right));
	StringsToWrite.Add(FString(TEXT("Top =%f"), top));
	StringsToWrite.Add(FString(TEXT("Bottom =%f"), bottom));


	if (FileManager.FileExists(*FilePath))
	{
		// We use the LoadFileToString to load the file into
		if (FFileHelper::SaveStringArrayToFile(StringsToWrite, *FilePath))
		{
			UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Sucsesfuly Written: \"%d\" FStrings to the text file"), StringsToWrite.Num());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Failed to write FString to file."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("FileManipulation: ERROR: Can not read file because it was not found."));
		UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Expected file location: %s"), *FilePath);
	}

	//FFileHelper::SaveStringArrayToFile(StringsToWrite, *FilePath);





}

WriteCropData::~WriteCropData()
{
}
