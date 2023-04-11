// Fill out your copyright notice in the Description page of Project Settings.


#include "InverseKinematics/FK.h"
//#include "FK.h"

FK::FK()
{
}

FK::~FK()
{
}

void FK::forwardKinematics(FRobot& Robot, FVector BasePos)
{

	TArray<FMatrix44d> DHTrans;
	
	DHTrans.Init(FMatrix::Identity, Robot.Njoints+1);
	//construct tranform matrices
	for (int i = 0; i < Robot.Njoints+1; i++) {
		//first row
		DHTrans[i].M[0][0] = cos(Robot.DH[i].theta);
		DHTrans[i].M[0][1] = -sin(Robot.DH[i].theta)*cos(Robot.DH[i].alpha);
		DHTrans[i].M[0][2] = sin(Robot.DH[i].theta) * sin(Robot.DH[i].alpha);
		DHTrans[i].M[0][3] = Robot.DH[i].r * cos(Robot.DH[i].theta);
		//second row
		DHTrans[i].M[1][0] = sin(Robot.DH[i].theta);
		DHTrans[i].M[1][1] = cos(Robot.DH[i].theta) * cos(Robot.DH[i].alpha);
		DHTrans[i].M[1][2] = -cos(Robot.DH[i].theta) * sin(Robot.DH[i].alpha);
		DHTrans[i].M[1][3] = Robot.DH[i].r * sin(Robot.DH[i].theta);
		//third row
		DHTrans[i].M[2][0] = 0;
		DHTrans[i].M[2][1] = sin(Robot.DH[i].alpha);
		DHTrans[i].M[2][2] = cos(Robot.DH[i].alpha);
		DHTrans[i].M[2][3] = Robot.DH[i].d;
		//fourth row
		DHTrans[i].M[3][0] = 0;
		DHTrans[i].M[3][1] = 0;
		DHTrans[i].M[3][2] = 0;
		DHTrans[i].M[3][3] = 1;
	}
	
	//multiply matrices together
	
	TArray<FMatrix44d> DHTransNew = DHTrans;
	DHTransNew[0].SetColumn(3,DHTransNew[0].GetColumn(3)+BasePos);
	FMatrix44d res = DHTransNew[0];
	Robot.FKTransforms[0] = DHTransNew[0];
	for (int i = 1; i < Robot.Njoints+1; i++) {
		res *= DHTrans[i]; //DHTrans[i] are relative joint transforms
		DHTransNew[i] = res; //DHTrans[i] are global joint transforms (nice)
		if (i != Robot.Njoints) {
			Robot.jPos[i] = DHTransNew[i].GetColumn(3);
		}
		else {
			Robot.ePos = DHTransNew[i].GetColumn(3);
		}
		Robot.FKTransforms[i] = DHTransNew[i];
	}



}
