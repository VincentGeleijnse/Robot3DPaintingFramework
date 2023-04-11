// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class ROBOTARM_API WriteCropData
{
public:
	WriteCropData();

	UFUNCTION(Blueprintable)
		void CropDataWrite(double left, double right, double top, double bottom);

	~WriteCropData();
};
