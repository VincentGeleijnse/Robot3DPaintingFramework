// Fill out your copyright notice in the Description page of Project Settings.


#include "OBSOLETE/IKBTSolver/IKBTSolver.h"
#include "math.h"

// Sets default values
AIKBTSolver::AIKBTSolver()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AIKBTSolver::BeginPlay()
{
	Super::BeginPlay();
    FTransform homTrans = FTransform(FRotator(0,0,0),FVector(0,0,100),FVector(1,1,1));
    
    ikin(homTrans);
	
}

// Called every frame
void AIKBTSolver::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



int AIKBTSolver::ikin(FTransform EndEffPos /*, double solution_list[64][6]*/)
{

    //th23 = th2 + th3
    //th12 = th1 +th2;

    // declare constant parameters (note they will need values!) //later if this works we can input these if neccesary

    double l_1 = 18;
    double l_2 = 32;
    double l_3 = 30;
    double a_1 = 30;

    //forward Kinematics
    double T[4][4] = { {1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}, };
    double th1, th2, th3, th23, th4, th5, th6;
    double c1, c2, c3, c23, c4, c5, c6, s1, s2, s3, s23, s4, s5, s6;
    

    //input a pose here to test the inverse kinematics solution (we construct homogenous transform using forward kin)
    //then we can check if inverse kinematics gives us the same pose back.
    th1 = FMath::DegreesToRadians(th1deg); -PI;
    th2 = -FMath::DegreesToRadians(th2deg) -PI / 2;
    th3 = -FMath::DegreesToRadians(th3deg) +PI / 2;
    th4 = FMath::DegreesToRadians(th4deg);
    th5 = FMath::DegreesToRadians(th5deg);
    th6 = 0; //still gotta show end effector rotation so cant debug this one yet.
    th23 = th2 + th3;

    c1 = cos(th1);
    c2 = cos(th2);
    c3 = cos(th3);
    c23 = cos(th2 + th3);
    c4 = cos(th4);
    c5 = cos(th5);
    c6 = cos(th6);
    s1 = sin(th1);
    s2 = sin(th2);
    s3 = sin(th3);
    s23 = sin(th2 + th3);
    s4 = sin(th4);
    s5 = sin(th5);
    s6 = sin(th6);


    T[0][0] = c6 * (-c1 * s23 * s5 + c5 * (c1 * c23 * c4 - s1 * s4)) - s6 * (c1 * c23 * s4 + c4 * s1);

    T[1][0] = c6 * (c5 * (c1 * s4 + c23 * c4 * s1) - s1 * s23 * s5) + s6 * (c1 * c4 - c23 * s1 * s4);

        T[2][0] = -c6 * (c23 * s5 + c4 * c5 * s23) + s23 * s4 * s6;

    T[3][0] = 0;

    T[0][1] = -c6 * (c1 * c23 * s4 + c4 * s1) - s6 * (-c1 * s23 * s5 + c5 * (c1 * c23 * c4 - s1 * s4));
       

    T[1][1] = c6 * (c1 * c4 - c23 * s1 * s4) - s6 * (c5 * (c1 * s4 + c23 * c4 * s1) - s1 * s23 * s5);

    T[2][1] = c6 * s23 * s4 + s6 * (c23 * s5 + c4 * c5 * s23);

    T[3][1] = 0;

    T[0][2] = c1 * c5 * s23 + s5 * (c1 * c23 * c4 - s1 * s4);

    T[1][2] = c5 * s1 * s23 + s5 * (c1 * s4 + c23 * c4 * s1);

    T[2][2] = c23 * c5 - c4 * s23 * s5;

    T[3][2] = 0;

    T[0][3] = a_1*c1*c2 + c1*l_2*s23 - l_3*(-c1*c5*s23 - s5*(c1*c23*c4 - s1*s4));

    T[1][3] = a_1*c2*s1 + l_2*s1*s23 - l_3*(-c5*s1*s23 - s5*(c1*s4 + c23*c4*s1));

    T[2][3] = -a_1 * s2 + c23 * l_2 + l_1 - l_3*(-c23 * c5 + c4 * s23 * s5);

    T[3][3] = 1;

        //Comment when done with FK  ^
    

    double solution_list[64][6] ;
    FQuat rot = EndEffPos.GetRotation();
    FVector pos = EndEffPos.GetLocation();

    /* //uncomment when done with FK
    
    T[0][0] = rot.GetAxisX().Z;
    T[0][1] = rot.GetAxisX().Y;
    T[0][2] = rot.GetAxisX().X;
    T[1][0] = rot.GetAxisY().Z;
    T[1][1] = rot.GetAxisY().Y;
    T[1][2] = rot.GetAxisY().X;
    T[2][0] = rot.GetAxisZ().Z;
    T[2][1] = rot.GetAxisZ().Y;
    T[2][2] = rot.GetAxisZ().X;
    T[0][3] = pos.X;
    T[1][3] = pos.Y;
    T[2][3] = pos.Z;
    */
    
    //define the input vars
    double r_11 = T[0][0];
    double r_12 = T[0][1];
    double r_13 = T[0][2];
    double r_21 = T[1][0];
    double r_22 = T[1][1];
    double r_23 = T[1][2];
    double r_31 = T[2][0];
    double r_32 = T[2][1];
    double r_33 = T[2][2];
    double Px = T[0][3];
    double Py = T[1][3];
    double Pz = T[2][3];

    //double argument;

    //int True = 1;
    //int False = 0;

    int solvable_pose = true;
    // declare variables in solutions
    double th_1s1, th_1s2, th_3s1, th_3s2, th_3s3, th_3s4, th_23s1, th_23s2, th_23s3, th_23s4, th_2s1, th_2s2, th_2s3, th_2s4, th_4s1, th_4s2, th_4s3, th_4s4, th_4s5, th_4s6, th_4s7, th_4s8, th_5s1, th_5s2, th_5s3, th_5s4, th_5s5, th_5s6, th_5s7, th_5s8, th_6s1, th_6s2, th_6s3, th_6s4, th_6s5, th_6s6, th_6s7, th_6s8;


    //Theta 1
    th_1s1 = atan2(Py - l_3 * r_23, Px - l_3 * r_13);
    th_1s2 = atan2(Py - l_3 * r_23, Px - l_3 * r_13) + PI;



    //Theta 3

    if (solvable_pose && abs((-pow(a_1,2) - pow(l_2,2) + pow(-Pz + l_1 + l_3 * r_33,2) + pow((Px * cos(th_1s2) + Py * sin(th_1s2) - l_3 * (r_13 * cos(th_1s2) + r_23 * sin(th_1s2))), 2)) / (2 * a_1 * l_2)) > 1) {
        solvable_pose = false;
    }
    else{
        th_3s1 = asin((-pow(a_1,2) - pow(l_2,2) + pow(-Pz + l_1 + l_3 * r_33,2) + pow((Px * cos(th_1s2) + Py * sin(th_1s2) - l_3 * (r_13 * cos(th_1s2) + r_23 * sin(th_1s2))), 2)) / (2 * a_1 * l_2));
    }
               
    if (solvable_pose && abs((-pow(a_1,2) - pow(l_2,2) + pow(-Pz + l_1 + l_3 * r_33,2) + pow((Px * cos(th_1s2) + Py * sin(th_1s2) - l_3 * (r_13 * cos(th_1s2) + r_23 * sin(th_1s2))), 2)) / (2 * a_1 * l_2)) > 1) {
        solvable_pose = false;
    }
    else {
        th_3s2 = PI - asin((-pow(a_1,2) - pow(l_2,2) + pow(-Pz + l_1 + l_3 * r_33,2) + pow((Px * cos(th_1s2) + Py * sin(th_1s2) - l_3 * (r_13 * cos(th_1s2) + r_23 * sin(th_1s2))), 2)) / (2 * a_1 * l_2));
    }
    if (solvable_pose && abs((-pow(a_1,2) - pow(l_2,2) + pow(-Pz + l_1 + l_3 * r_33,2) + pow((Px * cos(th_1s1) + Py * sin(th_1s1) - l_3 * (r_13 * cos(th_1s1) + r_23 * sin(th_1s1))), 2)) / (2 * a_1 * l_2)) > 1) {
        solvable_pose = false;
    }
    else {
        th_3s3 = asin((-pow(a_1,2) - pow(l_2,2) + pow(-Pz + l_1 + l_3 * r_33,2) + pow((Px * cos(th_1s1) + Py * sin(th_1s1) - l_3 * (r_13 * cos(th_1s1) + r_23 * sin(th_1s1))),2) ) / (2 * a_1 * l_2));
    }
    if (solvable_pose && abs((-pow(a_1,2) - pow(l_2,2) + pow(-Pz + l_1 + l_3 * r_33,2) + pow((Px * cos(th_1s1) + Py * sin(th_1s1) - l_3 * (r_13 * cos(th_1s1) + r_23 * sin(th_1s1))), 2)) / (2 * a_1 * l_2)) > 1) {
        solvable_pose = false;
    }
    else {
        th_3s4 = PI - asin((-pow(a_1,2) - pow(l_2,2) + pow(-Pz + l_1 + l_3 * r_33,2) + pow((Px * cos(th_1s1) + Py * sin(th_1s1) - l_3 * (r_13 * cos(th_1s1) + r_23 * sin(th_1s1))), 2)) / (2 * a_1 * l_2));
    }

   

    //Theta 2

    th_2s1 = atan2(-l_2 * (-Px * cos(th_1s1) - Py * sin(th_1s1) + l_3 * r_13 * cos(th_1s1) + l_3 * r_23 * sin(th_1s1)) * cos(th_3s4) + (-a_1 - l_2 * sin(th_3s4)) * (Pz - l_1 - l_3 * r_33), l_2 * (Pz - l_1 - l_3 * r_33) * cos(th_3s4) + (-a_1 - l_2 * sin(th_3s4)) * (-Px * cos(th_1s1) - Py * sin(th_1s1) + l_3 * r_13 * cos(th_1s1) + l_3 * r_23 * sin(th_1s1)));
    th_2s2 = atan2(-l_2 * (-Px * cos(th_1s1) - Py * sin(th_1s1) + l_3 * r_13 * cos(th_1s1) + l_3 * r_23 * sin(th_1s1)) * cos(th_3s3) + (-a_1 - l_2 * sin(th_3s3)) * (Pz - l_1 - l_3 * r_33), l_2 * (Pz - l_1 - l_3 * r_33) * cos(th_3s3) + (-a_1 - l_2 * sin(th_3s3)) * (-Px * cos(th_1s1) - Py * sin(th_1s1) + l_3 * r_13 * cos(th_1s1) + l_3 * r_23 * sin(th_1s1)));
    th_2s3 = atan2(-l_2 * (-Px * cos(th_1s2) - Py * sin(th_1s2) + l_3 * r_13 * cos(th_1s2) + l_3 * r_23 * sin(th_1s2)) * cos(th_3s2) + (-a_1 - l_2 * sin(th_3s2)) * (Pz - l_1 - l_3 * r_33), l_2 * (Pz - l_1 - l_3 * r_33) * cos(th_3s2) + (-a_1 - l_2 * sin(th_3s2)) * (-Px * cos(th_1s2) - Py * sin(th_1s2) + l_3 * r_13 * cos(th_1s2) + l_3 * r_23 * sin(th_1s2)));
    th_2s4 = atan2(-l_2 * (-Px * cos(th_1s2) - Py * sin(th_1s2) + l_3 * r_13 * cos(th_1s2) + l_3 * r_23 * sin(th_1s2)) * cos(th_3s1) + (-a_1 - l_2 * sin(th_3s1)) * (Pz - l_1 - l_3 * r_33), l_2 * (Pz - l_1 - l_3 * r_33) * cos(th_3s1) + (-a_1 - l_2 * sin(th_3s1)) * (-Px * cos(th_1s2) - Py * sin(th_1s2) + l_3 * r_13 * cos(th_1s2) + l_3 * r_23 * sin(th_1s2)));

    //Theta 23

    th_23s1 = th_2s1 + th_3s4;
    th_23s2 = th_2s4 + th_3s1;
    th_23s3 = th_2s3 + th_3s2;
    th_23s4 = th_2s2 + th_3s3;

    //Theta 4

    th_4s1 = atan2(-r_13 * sin(th_1s1) + r_23 * cos(th_1s1), r_13 * cos(th_1s1) * cos(th_23s1) + r_23 * sin(th_1s1) * cos(th_23s1) - r_33 * sin(th_23s1));
    th_4s2 = atan2(r_13 * sin(th_1s1) - r_23 * cos(th_1s1), -r_13 * cos(th_1s1) * cos(th_23s1) - r_23 * sin(th_1s1) * cos(th_23s1) + r_33 * sin(th_23s1));
    th_4s3 = atan2(-r_13 * sin(th_1s2) + r_23 * cos(th_1s2), r_13 * cos(th_1s2) * cos(th_23s2) + r_23 * sin(th_1s2) * cos(th_23s2) - r_33 * sin(th_23s2));
    th_4s4 = atan2(r_13 * sin(th_1s2) - r_23 * cos(th_1s2), -r_13 * cos(th_1s2) * cos(th_23s2) - r_23 * sin(th_1s2) * cos(th_23s2) + r_33 * sin(th_23s2));
    th_4s5 = atan2(-r_13 * sin(th_1s2) + r_23 * cos(th_1s2), r_13 * cos(th_1s2) * cos(th_23s3) + r_23 * sin(th_1s2) * cos(th_23s3) - r_33 * sin(th_23s3));
    th_4s6 = atan2(r_13 * sin(th_1s2) - r_23 * cos(th_1s2), -r_13 * cos(th_1s2) * cos(th_23s3) - r_23 * sin(th_1s2) * cos(th_23s3) + r_33 * sin(th_23s3));
    th_4s7 = atan2(-r_13 * sin(th_1s1) + r_23 * cos(th_1s1), r_13 * cos(th_1s1) * cos(th_23s4) + r_23 * sin(th_1s1) * cos(th_23s4) - r_33 * sin(th_23s4));
    th_4s8 = atan2(r_13 * sin(th_1s1) - r_23 * cos(th_1s1), -r_13 * cos(th_1s1) * cos(th_23s4) - r_23 * sin(th_1s1) * cos(th_23s4) + r_33 * sin(th_23s4));

   //Theta 5

    th_5s1 = atan2((-r_13 * sin(th_1s1) + r_23 * cos(th_1s1)) / sin(th_4s2), r_13 * sin(th_23s1) * cos(th_1s1) + r_23 * sin(th_1s1) * sin(th_23s1) + r_33 * cos(th_23s1));
    th_5s2 = atan2((-r_13 * sin(th_1s2) + r_23 * cos(th_1s2)) / sin(th_4s3), r_13 * sin(th_23s2) * cos(th_1s2) + r_23 * sin(th_1s2) * sin(th_23s2) + r_33 * cos(th_23s2));
    th_5s3 = atan2((-r_13 * sin(th_1s2) + r_23 * cos(th_1s2)) / sin(th_4s6), r_13 * sin(th_23s3) * cos(th_1s2) + r_23 * sin(th_1s2) * sin(th_23s3) + r_33 * cos(th_23s3));
    th_5s4 = atan2((-r_13 * sin(th_1s1) + r_23 * cos(th_1s1)) / sin(th_4s1), r_13 * sin(th_23s1) * cos(th_1s1) + r_23 * sin(th_1s1) * sin(th_23s1) + r_33 * cos(th_23s1));
    th_5s5 = atan2((-r_13 * sin(th_1s2) + r_23 * cos(th_1s2)) / sin(th_4s4), r_13 * sin(th_23s2) * cos(th_1s2) + r_23 * sin(th_1s2) * sin(th_23s2) + r_33 * cos(th_23s2));
    th_5s6 = atan2((-r_13 * sin(th_1s1) + r_23 * cos(th_1s1)) / sin(th_4s7), r_13 * sin(th_23s4) * cos(th_1s1) + r_23 * sin(th_1s1) * sin(th_23s4) + r_33 * cos(th_23s4));
    th_5s7 = atan2((-r_13 * sin(th_1s2) + r_23 * cos(th_1s2)) / sin(th_4s5), r_13 * sin(th_23s3) * cos(th_1s2) + r_23 * sin(th_1s2) * sin(th_23s3) + r_33 * cos(th_23s3));
    th_5s8 = atan2((-r_13 * sin(th_1s1) + r_23 * cos(th_1s1)) / sin(th_4s8), r_13 * sin(th_23s4) * cos(th_1s1) + r_23 * sin(th_1s1) * sin(th_23s4) + r_33 * cos(th_23s4));

    //Theta 6

    th_6s1 = atan2((r_12 * sin(th_23s1) * cos(th_1s1) + r_22 * sin(th_1s1) * sin(th_23s1) + r_32 * cos(th_23s1)) / sin(th_5s4), (-r_11 * sin(th_23s1) * cos(th_1s1) - r_21 * sin(th_1s1) * sin(th_23s1) - r_31 * cos(th_23s1)) / sin(th_5s4));
    th_6s2 = atan2((r_12 * sin(th_23s3) * cos(th_1s2) + r_22 * sin(th_1s2) * sin(th_23s3) + r_32 * cos(th_23s3)) / sin(th_5s7), (-r_11 * sin(th_23s3) * cos(th_1s2) - r_21 * sin(th_1s2) * sin(th_23s3) - r_31 * cos(th_23s3)) / sin(th_5s7));
    th_6s3 = atan2((r_12 * sin(th_23s4) * cos(th_1s1) + r_22 * sin(th_1s1) * sin(th_23s4) + r_32 * cos(th_23s4)) / sin(th_5s8), (-r_11 * sin(th_23s4) * cos(th_1s1) - r_21 * sin(th_1s1) * sin(th_23s4) - r_31 * cos(th_23s4)) / sin(th_5s8));
    th_6s4 = atan2((r_12 * sin(th_23s1) * cos(th_1s1) + r_22 * sin(th_1s1) * sin(th_23s1) + r_32 * cos(th_23s1)) / sin(th_5s1), (-r_11 * sin(th_23s1) * cos(th_1s1) - r_21 * sin(th_1s1) * sin(th_23s1) - r_31 * cos(th_23s1)) / sin(th_5s1));
    th_6s5 = atan2((r_12 * sin(th_23s2) * cos(th_1s2) + r_22 * sin(th_1s2) * sin(th_23s2) + r_32 * cos(th_23s2)) / sin(th_5s5), (-r_11 * sin(th_23s2) * cos(th_1s2) - r_21 * sin(th_1s2) * sin(th_23s2) - r_31 * cos(th_23s2)) / sin(th_5s5));
    th_6s6 = atan2((r_12 * sin(th_23s4) * cos(th_1s1) + r_22 * sin(th_1s1) * sin(th_23s4) + r_32 * cos(th_23s4)) / sin(th_5s6), (-r_11 * sin(th_23s4) * cos(th_1s1) - r_21 * sin(th_1s1) * sin(th_23s4) - r_31 * cos(th_23s4)) / sin(th_5s6));
    th_6s7 = atan2((r_12 * sin(th_23s3) * cos(th_1s2) + r_22 * sin(th_1s2) * sin(th_23s3) + r_32 * cos(th_23s3)) / sin(th_5s3), (-r_11 * sin(th_23s3) * cos(th_1s2) - r_21 * sin(th_1s2) * sin(th_23s3) - r_31 * cos(th_23s3)) / sin(th_5s3));
    th_6s8 = atan2((r_12 * sin(th_23s2) * cos(th_1s2) + r_22 * sin(th_1s2) * sin(th_23s2) + r_32 * cos(th_23s2)) / sin(th_5s2), (-r_11 * sin(th_23s2) * cos(th_1s2) - r_21 * sin(th_1s2) * sin(th_23s2) - r_31 * cos(th_23s2)) / sin(th_5s2));

    

    



    

    //store each solution set (for each th_6 solution track back all dependedant solutions to create a set

    solution_list[0][0] = th_1s2;
    solution_list[0][1] = th_23s2;
    solution_list[0][2] = (th_2s3 + PI/2);
    solution_list[0][3] = th_3s2 - PI / 2;
    solution_list[0][4] = th_4s3;
    solution_list[0][5] = th_5s2;
    solution_list[0][6] = th_6s8;
    //



    //
    solution_list[1][0] = th_1s1;
    solution_list[1][1] = th_23s4;
    solution_list[1][2] = (th_2s1 + PI / 2);
    solution_list[1][3] = th_3s4 - PI / 2;
    solution_list[1][4] = th_4s7;
    solution_list[1][5] = th_5s6;
    solution_list[1][6] = th_6s6;
    //
    solution_list[2][0] = th_1s1;
    solution_list[2][1] = th_23s1 ;
    solution_list[2][2] = (th_2s1 + PI / 2);
    solution_list[2][3] = th_3s4 - PI / 2;
    solution_list[2][4] = th_4s1;
    solution_list[2][5] = th_5s4;
    solution_list[2][6] = th_6s1;
    //
    solution_list[3][0] = th_1s2;
    solution_list[3][1] = th_23s3 ;
    solution_list[3][2] = (th_2s3 + PI / 2);
    solution_list[3][3] = th_3s2 - PI / 2;
    solution_list[3][4] = th_4s5;
    solution_list[3][5] = th_5s7;
    solution_list[3][6] = th_6s2;
    //
    solution_list[4][0] = th_1s2;
    solution_list[4][1] = th_23s3;
    solution_list[4][2] = (th_2s3 + PI / 2);
    solution_list[4][3] = th_3s2 - PI / 2;
    solution_list[4][4] = th_4s6;
    solution_list[4][5] = th_5s3;
    solution_list[4][6] = th_6s7;
    //
    solution_list[5][0] = th_1s1;
    solution_list[5][1] = th_23s4 ;
    solution_list[5][2] = (th_2s1 + PI / 2);
    solution_list[5][3] = th_3s4 - PI / 2;
    solution_list[5][4] = th_4s8;
    solution_list[5][5] = th_5s8;
    solution_list[5][6] = th_6s3;
    //
    solution_list[6][0] = th_1s2;
    solution_list[6][1] = th_23s2;
    solution_list[6][2] = (th_2s3 + PI / 2);
    solution_list[6][3] = th_3s2 - PI / 2;
    solution_list[6][4] = th_4s4;
    solution_list[6][5] = th_5s5;
    solution_list[6][6] = th_6s5;
    //
    solution_list[7][0] = th_1s1;
    solution_list[7][1] = th_23s1;
    solution_list[7][2] = (th_2s1 + PI / 2);
    solution_list[7][3] = th_3s4 - PI / 2;
    solution_list[7][4] = th_4s2;
    solution_list[7][5] = th_5s1;
    solution_list[7][6] = th_6s4;

    UE_LOG(LogTemp, Warning, TEXT("Solution solvable = %d  th1 %f th2 %f th3 %f th4 %f th5 %f th6 %f"), solvable_pose, FMath::RadiansToDegrees(solution_list[0][0]), FMath::RadiansToDegrees(solution_list[0][2]), FMath::RadiansToDegrees(solution_list[0][3]), FMath::RadiansToDegrees(solution_list[0][4]), FMath::RadiansToDegrees(solution_list[0][5]), FMath::RadiansToDegrees(solution_list[0][6]));
    UE_LOG(LogTemp, Warning, TEXT("Solution solvable = %d  th1 %f th2 %f th3 %f th4 %f th5 %f th6 %f"), solvable_pose, FMath::RadiansToDegrees(solution_list[1][0]), FMath::RadiansToDegrees(solution_list[1][2]), FMath::RadiansToDegrees(solution_list[1][3]), FMath::RadiansToDegrees(solution_list[1][4]), FMath::RadiansToDegrees(solution_list[0][5]), FMath::RadiansToDegrees(solution_list[1][6]));
    UE_LOG(LogTemp, Warning, TEXT("Solution solvable = %d  th1 %f th2 %f th3 %f th4 %f th5 %f th6 %f"), solvable_pose, FMath::RadiansToDegrees(solution_list[2][0]), FMath::RadiansToDegrees(solution_list[2][2]), FMath::RadiansToDegrees(solution_list[2][3]), FMath::RadiansToDegrees(solution_list[2][4]), FMath::RadiansToDegrees(solution_list[0][5]), FMath::RadiansToDegrees(solution_list[2][6]));
    UE_LOG(LogTemp, Warning, TEXT("Solution solvable = %d  th1 %f th2 %f th3 %f th4 %f th5 %f th6 %f"), solvable_pose, FMath::RadiansToDegrees(solution_list[3][0]), FMath::RadiansToDegrees(solution_list[3][2]), FMath::RadiansToDegrees(solution_list[3][3]), FMath::RadiansToDegrees(solution_list[3][4]), FMath::RadiansToDegrees(solution_list[0][5]), FMath::RadiansToDegrees(solution_list[3][6]));
    UE_LOG(LogTemp, Warning, TEXT("Solution solvable = %d  th1 %f th2 %f th3 %f th4 %f th5 %f th6 %f"), solvable_pose, FMath::RadiansToDegrees(solution_list[4][0]), FMath::RadiansToDegrees(solution_list[4][2]), FMath::RadiansToDegrees(solution_list[4][3]), FMath::RadiansToDegrees(solution_list[4][4]), FMath::RadiansToDegrees(solution_list[0][5]), FMath::RadiansToDegrees(solution_list[4][6]));
    UE_LOG(LogTemp, Warning, TEXT("Solution solvable = %d  th1 %f th2 %f th3 %f th4 %f th5 %f th6 %f"), solvable_pose, FMath::RadiansToDegrees(solution_list[5][0]), FMath::RadiansToDegrees(solution_list[5][2]), FMath::RadiansToDegrees(solution_list[5][3]), FMath::RadiansToDegrees(solution_list[5][4]), FMath::RadiansToDegrees(solution_list[0][5]), FMath::RadiansToDegrees(solution_list[5][6]));
    UE_LOG(LogTemp, Warning, TEXT("Solution solvable = %d  th1 %f th2 %f th3 %f th4 %f th5 %f th6 %f"), solvable_pose, FMath::RadiansToDegrees(solution_list[6][0]), FMath::RadiansToDegrees(solution_list[6][2]), FMath::RadiansToDegrees(solution_list[6][3]), FMath::RadiansToDegrees(solution_list[6][4]), FMath::RadiansToDegrees(solution_list[0][5]), FMath::RadiansToDegrees(solution_list[6][6]));
    UE_LOG(LogTemp, Warning, TEXT("End effector Pos = %f %f %f"), Px, Py, Pz);


    DrawDebugSphere(
        GetWorld(),
        FVector(Px, Py, Pz),
        1,
        10,
        FColor(0, 255, 255),
        true,
        10.0f,
        2);

	return solvable_pose;
}

