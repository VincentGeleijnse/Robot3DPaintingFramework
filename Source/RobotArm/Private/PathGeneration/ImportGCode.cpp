// Fill out your copyright notice in the Description page of Project Settings.


#include "PathGeneration/ImportGCode.h"
#include "Components/SplineComponent.h"
#include "fstream"
#include "iostream"

// Sets default values
AImportGCode::AImportGCode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}



// Called when the game starts or when spawned
void AImportGCode::BeginPlay()
{
	Super::BeginPlay();
	pathsImported = false;
	startDataImported = false;

}

// Called every frame
void AImportGCode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AImportGCode::GetNewFile() {

	
}

void AImportGCode::ImportPathSplines() {
	FString Str;
	FString FilePath;
	// load in the g-code files (for each mask) as a string
	for (int o = 0; o < colorArr.Num(); o++) {
		FString splineName = TEXT("SplineComp");
		splineName.AppendInt(o);
		pathSpline.Add(NewObject<USplineComponent>(this, FName(*splineName)));

		FilePath = FPaths::ProjectContentDir();
		FilePath.Append(TEXT("/step1-2/Output/colorMask"));
		FilePath.AppendInt(o + 1); //for now we ignore the first color(which we always pick as white, until white can or should also be drawn)
		FilePath.Append(TEXT(".txt"));

		UE_LOG(LogTemp, Warning, TEXT("The Path to file is %s"), *FilePath);
		FFileHelper::LoadFileToString(Str, *FilePath);

		FString command;
		FString b;
		//TArray<FString> GCommands;
		int i = 0;
		bool prevG0 = false;
		FVector G0Vec;


		while (Str.Split(LINE_TERMINATOR, &command, &Str)) {

			if (prevG0) {
				if (command.Contains(TEXT("G1 X")) || command.Contains(TEXT("G2 X")) || command.Contains(TEXT("G3 X"))) {
					pathSpline[o]->AddSplinePointAtIndex(FVector(pathOrigin.X, pathOrigin.Y + (G0Vec.X) * scale, pathOrigin.Z + (G0Vec.Y) * scale), i, ESplineCoordinateSpace::World);
					pathSpline[o]->SetSplinePointType(i, ESplinePointType::Linear);
					i++;
					prevG0 = false;

				}
			}
			//straight line non drawing segment
			if (command.Contains(TEXT("G0 X"))) {
				FString result;
				FGcommandStruct  Gcommand;

				if (command.Contains(TEXT("G0 X"))) {
					Gcommand.Z = 1;
				}
				if (command.Split(TEXT(" Y"), &command, &result)) {
					Gcommand.Y = FCString::Atof(*result);
				}
				if (command.Split(TEXT(" X"), &command, &result)) {
					Gcommand.X = FCString::Atof(*result);
				}
				prevG0 = true;
				G0Vec = FVector(Gcommand.X, Gcommand.Y, Gcommand.Z);
				pathSpline[o]->AddSplinePointAtIndex(FVector(pathOrigin.X + Gcommand.Z, pathOrigin.Y + (Gcommand.X) * scale, pathOrigin.Z + (Gcommand.Y) * scale), i, ESplineCoordinateSpace::World);
				pathSpline[o]->SetSplinePointType(i, ESplinePointType::Linear);
				i++;
			}
			//straight line drawing segment
			else if (command.Contains(TEXT("G1 X"))) {
				FString result;
				FGcommandStruct  Gcommand;

				if (command.Split(TEXT(" Y"), &command, &result)) {
					Gcommand.Y = FCString::Atof(*result);
				}
				if (command.Split(TEXT(" X"), &command, &result)) {
					Gcommand.X = FCString::Atof(*result);
				}



				pathSpline[o]->AddSplinePointAtIndex(FVector(pathOrigin.X + Gcommand.Z, pathOrigin.Y + (Gcommand.X) * scale, pathOrigin.Z + (Gcommand.Y) * scale), i, ESplineCoordinateSpace::World);
				pathSpline[o]->SetSplinePointType(i, ESplinePointType::Linear);
				/*DrawDebugSphere(
					GetWorld(),
					pathSpline[o]->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World),
					1,
					10,
					FColor(40, 0, 0),
					true,
					10000
				);*/
				i++;
			}
			//straight line arc segment
			else if (command.Contains(TEXT("G2 X")) || command.Contains(TEXT("G3 X"))) {
				FString result;
				FGcommandStruct  Gcommand;
				if (command.Split(TEXT(" J"), &command, &result)) {
					Gcommand.J = FCString::Atof(*result);
				}
				if (command.Split(TEXT(" I"), &command, &result)) {
					Gcommand.I = FCString::Atof(*result);
				}
				if (command.Split(TEXT(" Y"), &command, &result)) {
					Gcommand.Y = FCString::Atof(*result);
				}
				if (command.Split(TEXT(" X"), &command, &result)) {
					Gcommand.X = FCString::Atof(*result);
				}

				pathSpline[o]->AddSplinePointAtIndex(FVector(pathOrigin.X + Gcommand.Z, pathOrigin.Y + (Gcommand.X) * scale, pathOrigin.Z + (Gcommand.Y) * scale), i, ESplineCoordinateSpace::World);
				pathSpline[o]->SetSplinePointType(i, ESplinePointType::Linear);

				i++;
			}
		}

	}
	pathsImported = true;
}

void AImportGCode::ImportStartData() {
	if (importPaths) {
		//load in all the colors 
		FString Str;
		FString FilePath = FPaths::ProjectContentDir();
		FilePath.Append(TEXT("/step1-2/Input/colors.txt"));
		UE_LOG(LogTemp, Warning, TEXT("The Path to file is %s"), *FilePath);
		FFileHelper::LoadFileToString(Str, *FilePath);

		FString colorDesc;

		while (Str.Split(LINE_TERMINATOR, &colorDesc, &Str)) {
			FString result;
			FLinearColor c;
			colorDesc.Split(TEXT(","), &result, &colorDesc);
			c.R = FCString::Atof(*result) / 255;
			colorDesc.Split(TEXT(","), &result, &colorDesc);
			c.G = FCString::Atof(*result) / 255;
			c.B = FCString::Atof(*colorDesc) / 255;
			c.A = 1;
			colorArr.Add(c);
			UE_LOG(LogTemp, Warning, TEXT("color %s"), *c.ToString());
		}
		//remove white (can be removed later when we actually want to paint white) 
		colorArr.Remove(FLinearColor(1, 1, 1, 1));
		UE_LOG(LogTemp, Warning, TEXT("found %d colors"), colorArr.Num());

		//load in the config for some parameters to use:
		FilePath = FPaths::ProjectContentDir();
		FilePath.Append(TEXT("/step1-2/Config/config.txt"));
		UE_LOG(LogTemp, Warning, TEXT("The Path to file is %s"), *FilePath);
		FFileHelper::LoadFileToString(Str, *FilePath);

		FString configDesc;
		while (Str.Split(TEXT("\n"), &configDesc, &Str)) {
			if (configDesc.StartsWith(TEXT("IP.TargetSize.Width"))) {
				FString imgWidth;
				configDesc.Split(TEXT("="), &configDesc, &imgWidth);
				ImageWidth = FCString::Atoi(*imgWidth);
			}
			//else if() other settings we might want to use
		}
		if (Str.StartsWith(TEXT("IP.TargetSize.Width"))) {
			FString imgWidth;
			Str.Split(TEXT("="), &Str, &imgWidth);
			ImageWidth = FCString::Atoi(*imgWidth);
		}

		//load in image properties to get width and height of image and gcode path
		FilePath = FPaths::ProjectContentDir();
		FilePath.Append(TEXT("/step1-2/Input/ImageProperties.txt"));
		UE_LOG(LogTemp, Warning, TEXT("The Path to file is %s"), *FilePath);
		FFileHelper::LoadFileToString(Str, *FilePath);

		FString imgDesc;


		while (Str.Split(LINE_TERMINATOR, &imgDesc, &Str)) {
			if (imgDesc.StartsWith(TEXT("width"))) {
				FString imgWidth;
				imgDesc.Split(TEXT("="), &imgDesc, &imgWidth);
				PixelWidth = FCString::Atoi(*imgWidth);
			}
			else if (imgDesc.StartsWith(TEXT("height"))) {
				FString imgHeight;
				imgDesc.Split(TEXT("="), &imgDesc, &imgHeight);
				PixelHeight = FCString::Atoi(*imgHeight);
			}
		}
		ImageHeight = int(float(ImageWidth) / float(PixelWidth) * float(PixelHeight));

		UE_LOG(LogTemp, Warning, TEXT("original image dimensions width = %d, height = %d "), PixelWidth, PixelHeight);
		UE_LOG(LogTemp, Warning, TEXT("path dimensions = %d, height = %d "), ImageWidth, ImageHeight);
		
	}
	startDataImported = true;
}