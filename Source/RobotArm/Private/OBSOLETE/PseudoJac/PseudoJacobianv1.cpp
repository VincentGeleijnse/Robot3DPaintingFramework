// Fill out your copyright notice in the Description page of Project Settings.


#include "OBSOLETE/PseudoJac/PseudoJacobianv1.h"
#include "CustomUEMatrix.h"

THIRD_PARTY_INCLUDES_START
#include <Eigen/QR>
THIRD_PARTY_INCLUDES_END


#define NJOINTS 2

// Sets default values
APseudoJacobianv1::APseudoJacobianv1()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APseudoJacobianv1::BeginPlay()
{
	Super::BeginPlay();

    TArray<float> startThetas = GetTheta(Joint);
    
    //t = GetT(Joint);
    t.Init(FVector(0, 0, 0), 2);
    t[1] = FVector(0, 0, 50);
    k.Init(FVector(0, 0, 0), 2);
    k[0] = FVector(0, -1, 0);
    k[1] = FVector(0, -1, 0);

    TArray<float> Theta0;
    Theta0.Init(0, 2);
    Theta0[0] = FMath::DegreesToRadians(20);
    Theta0[1] = FMath::DegreesToRadians(20);

    //k = GetK(Joint);

    TArray<AActor*> attached;
    Joint[NJOINTS - 1]->GetAttachedActors(attached);

    
    v_step_size = 0.05;
   
    theta_max_step = 0.2;

    
    Q_j = Theta0; // Array contianing the starting joint angles;
    p_end = GoalPos; //  desired x, y, z coordinate of the end effector in the base frame

    //p_j = position(Q_j, -1, FVector(0,0,50)); // x, y, z coordinate of the position of the end effector in the global reference frame
    p_j = EndEff->GetRootComponent()->GetComponentLocation();
    delta_p = p_end - p_j;

    iterationCount = 0;
    max_steps = 5000;

    p_eff_Nah = FVector(50, 0, 0);
    

    
    //TArray<float> finalThetas = pseudo_inverse(Theta0, FVector(0,0,50) , GoalPos, 500);



	
}

// Called every frame
void APseudoJacobianv1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    //Get the current position of the end - effector in the global frame
    p_j = EndEff->GetRootComponent()->GetComponentLocation();
    //p_j = position(Q_j, -1, p_eff_Nah);

    // Increment the time step
    

    //Determine the difference between the new positionand the desired end position
    delta_p = p_end - p_j;


    if (delta_p.Length() > 0.01 && iterationCount < max_steps) {
        //print(f'j{j}: Q[{Q_j}] , P[{p_j}]') # Print the current joint angles and position of the end effector in the global frame
        // 
        // Reduce the delta_p 3 - element delta_p vector by some scaling factor
        // delta_p represents the distance between where the end effector is now and our goal position.

        TArray<FFloatMatrix> v_p;
        v_p = InitMatrix(3, 1);
        FVector vpv = delta_p * v_step_size / delta_p.Length();
        v_p[0].C[0] = vpv.X;
        v_p[1].C[0] = vpv.Y;
        v_p[2].C[0] = vpv.Z;


        // Get the jacobian matrix given the current joint angles 
        TArray<FFloatMatrix> J = jacobian(Q_j, p_eff_Nah);
        //transpose Jacobian



        Eigen::MatrixXd JEig = Eigen::MatrixXd::Random(J.Num(), J[0].C.Num());

        for (int r = 0; r < J.Num(); r++) {
            {
                for (int c = 0; c < J[0].C.Num(); c++) {
                    JEig(r, c) = J[r].C[c];
                }
            }
        }

        Eigen::MatrixXd pinv = JEig.completeOrthogonalDecomposition().pseudoInverse();


        TArray<FFloatMatrix> PINV = InitMatrix(J.Num(), J[0].C.Num());
        for (int r = 0; r < J.Num(); r++) {
            {
                for (int c = 0; c < J[0].C.Num(); c++) {
                    PINV[r].C[c] = pinv(r, c);
                }
            }
        }
        PINV = TransposeMatrix(PINV);

        //UE_LOG(LogTemp, Warning, TEXT("V_p product"));
        TArray<FFloatMatrix> v_Q = MatrixProduct(PINV, v_p);
        /*
        # We use clip method here so that the joint doesn't move too much. We
            # just want the joints to move a tiny amount at each time step because
            # the full motion of the end effector is nonlinear, and we're approximating the
            # big nonlinear motion of the end effector as a bunch of tiny linear motions.

        //Move the joints to new angles
        */
        TArray<FFloatMatrix> Q_jM;
        Q_jM = InitMatrix(1, 1);
        Q_jM[0].C = Q_j;
        TArray<FFloatMatrix> v_Q_clip = TransposeMatrix(MatrixClip(v_Q, -1 * theta_max_step, theta_max_step));
        TArray<FFloatMatrix> Q_jAdd;
        Q_jAdd = MatrixAdd(Q_jM, v_Q_clip);
        Q_j = Q_jAdd[0].C;


        ApplyTheta(Joint, Q_j);

        iterationCount++;

    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("'done cuz it %d "),iterationCount);
    }



}

TArray<FFloatMatrix> APseudoJacobianv1::axis_angle_rot_matrix(FVector kv, float qa) {
    /*

    Creates a 3x3 rotation matrix in 3D space from an axis and an angle.

    Input
    :param kv: A 3 element array containing the unit axis to rotate around (kx,ky,kz)
    :param qv: The angle (in radians) to rotate by

    Output
    :return: A 3x3 element matix containing the rotation matrix

    */

    float c_theta = cos(qa);
    float s_theta = sin(qa);
    float v_theta = 1 - cos(qa);
    float kx = kv.X;
    float ky = kv.Y;
    float kz = kv.Z;

    // First row of the rotation matrix
    float r00 = kx * kx * v_theta + c_theta;
    float r01 = kx * ky * v_theta - kz * s_theta;
    float r02 = kx * kz * v_theta + ky * s_theta;

    // Second row of the rotation matrix
    float r10 = kx * ky * v_theta + kz * s_theta;
    float r11 = ky * ky * v_theta + c_theta;
    float r12 = ky * kz * v_theta - kx * s_theta;

    // Third row of the rotation matrix
    float r20 = kx * kz * v_theta - ky * s_theta;
    float r21 = ky * kz * v_theta + kx * s_theta;
    float r22 = kz * kz * v_theta + c_theta;

    TArray<FFloatMatrix> rot_matrix = InitMatrix(3, 3);

    //row1
    rot_matrix[0].C[0]  = r00;
    rot_matrix[0].C[1] = r01;
    rot_matrix[0].C[2] = r02;

    //row2
    rot_matrix[1].C[0] = r10;
    rot_matrix[1].C[1] = r11;
    rot_matrix[1].C[2] = r12;

    //row3
    rot_matrix[2].C[0] = r20;
    rot_matrix[2].C[1] = r21;
    rot_matrix[2].C[2] = r22;

    return rot_matrix;
}



TArray<FFloatMatrix> APseudoJacobianv1::hr_matrix(FVector kv, FVector tv, float qa)
{
    /*
        Create the Homogenous Representation matrix that transforms a point from Frame B to Frame A.
        Using the axis - angle representation
        Input
        : param k : A 3 element array containing the unit axis to rotate around(kx, ky, kz)
        : param t : The translation from the current frame(e.g.Frame A) to the next frame(e.g.Frame B)
        : param q : The rotation angle(i.e.joint angle)

        Output
        : return : A 4x4 Homogenous representation matrix
        */

    TArray<FFloatMatrix> rot_matrix_A_B = axis_angle_rot_matrix(kv, qa);
    FVector translation_vec_A_B = tv;

    TArray<FFloatMatrix> homgen_mat = InitMatrix(4, 4);

    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            homgen_mat[r].C[c] = rot_matrix_A_B[r].C[c];
        }
    }

    homgen_mat[0].C[3] = translation_vec_A_B.X;
    homgen_mat[1].C[3] = translation_vec_A_B.Y;
    homgen_mat[2].C[3] = translation_vec_A_B.Z;
    homgen_mat[3].C[0] = 0;
    homgen_mat[3].C[1] = 0;
    homgen_mat[3].C[2] = 0;
    homgen_mat[3].C[3] = 1;

    return homgen_mat;
}

//Below functions are part of robo arm class and need to be edited once thats resolved.




FVector4 APseudoJacobianv1::Matrix4x4Vector4Mult(FMatrix M, FVector4 V) 
{
    FVector4 res = FVector4(M.M[0][0] * V.X + M.M[0][1] * V.Y + M.M[0][2] * V.Z + M.M[0][3] * V.W,
                            M.M[1][0] * V.X + M.M[1][1] * V.Y + M.M[1][2] * V.Z + M.M[1][3] * V.W,
                            M.M[2][0] * V.X + M.M[2][1] * V.Y + M.M[2][2] * V.Z + M.M[2][3] * V.W,
                            M.M[3][0] * V.X + M.M[3][1] * V.Y + M.M[3][2] * V.Z + M.M[3][3] * V.W);
    return res;
}

FVector APseudoJacobianv1::position(TArray<float> Q, int index, FVector p_i)
{
    TArray<FFloatMatrix> this_joint_position = InitMatrix(1, 4);
    this_joint_position[0].C[0] = p_i.X;
    this_joint_position[0].C[1] = p_i.Y;
    this_joint_position[0].C[2] = p_i.Z;
    this_joint_position[0].C[3] = 1;
    //End effector joint
    if (index == -1) {
        index = NJOINTS - 1;
    }
    // Store the original index of this joint  
    int orig_joint_index = index;

    TArray<FFloatMatrix> running_multiplication;

    //Start from the index of this joint and work backwards to index 0
    while (index >= 0) {

        // If we are at the original joint index
        if (index == orig_joint_index) {
            running_multiplication = MatrixProduct (hr_matrix(k[index], t[index], Q[index]),   this_joint_position);

        }
        // If we are not at the original joint index
        else {
            running_multiplication = MatrixProduct(hr_matrix(k[index], t[index], Q[index]),   running_multiplication);
        }

        index = index - 1;
    }
    //extract the points
    float px = running_multiplication[0].C[0];
    float py = running_multiplication[0].C[1];
    float pz = running_multiplication[0].C[2];




    return FVector(px,py,pz);
}

TArray<FFloatMatrix> APseudoJacobianv1::MatrixProduct(TArray<FFloatMatrix> A, TArray<FFloatMatrix> B)
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

// Get the inverse of matrix.
TArray<FFloatMatrix> APseudoJacobianv1::MatrixInverse(TArray<FFloatMatrix> M)
{
    int rows = M.Num();
    int cols = M[0].C.Num();

    TArray<FFloatMatrix> MI;
    if (rows != cols) {
        UE_LOG(LogTemp, Warning, TEXT("MI is empty, matrix is not square "));
        return MI; //matrix must be square
    }
    float det = MatrixDeterminant(M, rows); //rows should == cols
    if (det == 0) {
        UE_LOG(LogTemp, Warning, TEXT("'Determinant 0, cant be inverted "));
        return MI; //determinant 0, cant be inverted
    }
    MI = InitMatrix(rows, rows);
    TArray<FFloatMatrix> Adj = MatrixAdjoint(M);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < rows; j++) {
            MI[i].C[j] = Adj[i].C[j] / (float)det;
        }
    }
    return MI;
}

TArray<FFloatMatrix> APseudoJacobianv1::MatrixCofactor(TArray<FFloatMatrix> M, int a, int b, int n)
{
    TArray<FFloatMatrix> tf;
    tf = InitMatrix(NJOINTS, NJOINTS);
    int i = 0, j = 0;
    //Copy only those elements which are not in given row r and column c: 
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++)
        { 
            if (r != a && c != b) {
                tf[i].C[j++] = M[r].C[c]; //If row is filled increase r index and reset c index
                if (j == n - 1) {
                    j = 0; 
                    i++;
                }
            }
        }
    }
    return tf;
}

TArray<FFloatMatrix> APseudoJacobianv1::MatrixRowEchelon(TArray<FFloatMatrix> M)
{
    const int rows = M.Num();
    const int cols = M[0].C.Num();

    TArray<FFloatMatrix> A;
    A = M;

    int lead = 0;

    while (lead < rows) {
        float d, m;

        for (int r = 0; r < rows; r++) { 
            /* calculate divisor and multiplier */
            if (A[lead].C[lead] == 0) {
                d = 1;
                m = A[r].C[lead] / 1;
            }
            else {
                d = 1;
                m = A[r].C[lead] / A[lead].C[lead];
            }

            for (int c = 0; c < cols; c++) { 
                if (r == lead) {
                    A[r].C[c] /= d; // make pivot = 1 (except we dont want this) so we remove this part
                }
                else {
                    A[r].C[c] -= A[lead].C[c] * m;  // make other = 0
                }
            }
        }
        lead++;

    }
    TArray<int> b;
    b.Init(0,rows);
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (A[r].C[c] != 0) {
                b[r] = 1;
            }
        }
    }
    TArray<FFloatMatrix> Out;
    
    
    int i = 0;
    int a = 0;
    for (int j = 0; j < b.Num(); j++) {
        if (b[j]) {
            a++;
        }
    }
    Out = InitMatrix(a, A[0].C.Num());
    for (int j = 0; j < b.Num(); j++) {
        if (b[j]) {
            Out[i] = A[j];
            i++;
        }
    }

    return Out;
}


TArray<FFloatMatrix> APseudoJacobianv1::MatrixAdd(TArray<FFloatMatrix> A, TArray<FFloatMatrix> B)
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
        for(int c = 0; c < cA; c++) {
            C[r].C[c] = A[r].C[c] + B[r].C[c]; 
        }
    }



    return C;
}

TArray<FFloatMatrix> APseudoJacobianv1::MatrixClip(TArray<FFloatMatrix> M, float min, float max)
{
    int rows = M.Num();
    int cols = M[0].C.Num();
    TArray<FFloatMatrix> MC;
    MC = InitMatrix( rows, cols);

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            MC[r].C[c] = FMath::Clamp(M[r].C[c], min, max);
        }
    }

    return MC;
}

void APseudoJacobianv1::ApplyTheta(TArray<AJoint*> Joints, TArray<float> Thetas)
{
    for (int i = 0; i < Thetas.Num(); i++) {
        if (Joints[i]->bX) {
            Joints[i]->SetActorRelativeRotation(FRotator(0,0, FMath::RadiansToDegrees(Thetas[i])));
        }
        else if (Joints[i]->bY) {
            Joints[i]->SetActorRelativeRotation(FRotator(FMath::RadiansToDegrees(Thetas[i]), 0, 0));
        }
        else if (Joint[i]->bZ) {
            Joints[i]->SetActorRelativeRotation(FRotator(0, FMath::RadiansToDegrees(Thetas[i]), 0));
        }
    }
}

TArray<float> APseudoJacobianv1::GetTheta(TArray<AJoint*> Joints)
{
    TArray<float> thetas;
    for (int i = 0; i < Joints.Num(); i++) {
        if (Joints[i]->bX) {
            thetas.Add(FMath::DegreesToRadians(Joints[i]->GetActorRotation().Roll));
        }
        else if (Joints[i]->bY) {
            thetas.Add(FMath::DegreesToRadians(Joints[i]->GetActorRotation().Pitch));
        }
        else if (Joints[i]->bZ) {
            thetas.Add(FMath::DegreesToRadians(Joints[i]->GetActorRotation().Yaw));
        }


        
    }

    

    return thetas;
}

TArray<FVector> APseudoJacobianv1::GetT(TArray<AJoint*> Joints)
{
    TArray<FVector> tvec;
    for (int i = 0; i < Joints.Num(); i++) {
        if (i == 0) {
            tvec.Add(Joints[i]->GetActorLocation());
        }
        else {
            tvec.Add(Joints[i]->GetActorLocation() - Joints[i - 1]->GetActorLocation());
        }
        
    }

    return tvec;
}

TArray<FVector> APseudoJacobianv1::GetK(TArray<AJoint*> Joints)
{
    TArray<FVector> kvec;
    for (int i = 0; i < Joints.Num(); i++) {
        if (Joints[i]->bX) {
            kvec.Add(Joints[i]->GetActorRightVector());
        }
        else if (Joints[i]->bY) {
            kvec.Add(Joints[i]->GetActorForwardVector());
        }
        else if (Joints[i]->bZ) {
            kvec.Add(Joints[i]->GetActorUpVector());
        }
    }

    return kvec;
}

float APseudoJacobianv1::MatrixDeterminant(TArray<FFloatMatrix> M, int n)
{
    int D = 0;
    int rows = M.Num();
    int cols = M[0].C.Num();
    if (n == 1) {
        return M[0].C[0];
    }
    TArray<FFloatMatrix> tf; //store cofactors
    int s = 1;// store sign multiplier
    //iterate each element of first row
    for (int f = 0; f < n; f++) {
        tf = MatrixCofactor(M, 0, f, n);
        D += s * M[0].C[f] * MatrixDeterminant(tf, n - 1);
        s = -s;
    }
    


    return D;
}


TArray<FFloatMatrix> APseudoJacobianv1::MatrixAdjoint(TArray<FFloatMatrix> M)
{
    int N = M.Num();
    TArray<FFloatMatrix> adj;
    adj = InitMatrix(N, N);
    if (N == 1) {
        adj[0].C[0] = 1;
        return adj;
    }
    //store cofactors
    TArray<FFloatMatrix> tf;
    int s = 1;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            tf = MatrixCofactor(M, i, j, N);
            s = ((i + j) % 2 == 0) ? 1 : -1; //sign of adj[j][i] positive if sum of row and column indexes is even.
            adj[j].C[i] = (s) * (MatrixDeterminant(tf, N - 1));
        }
    }
    return adj;
}

TArray<FFloatMatrix> APseudoJacobianv1::TransposeMatrix(TArray<FFloatMatrix> Mat)
{
     int rows = Mat.Num();
     int cols = Mat[0].C.Num();

     TArray<FFloatMatrix> TMat;
     TMat = InitMatrix( cols, rows);
     for (int r = 0; r < rows; r++) {
         for (int c = 0; c < cols; c++) {
             TMat[c].C[r] = Mat[r].C[c];
         }
     }
    return TMat;
}

TArray<FFloatMatrix> APseudoJacobianv1::InitMatrix( int Rows,  int Cols)
{
    TArray<FFloatMatrix> M;
    M.Init({}, Rows);
    for (int i = 0; i < Rows; i++) {
        M[i].C.Init(0.0, Cols);
    }
    return M;
}




TArray<FFloatMatrix> APseudoJacobianv1::jacobian(TArray<float> Q, FVector p_eff_N) 
{
    /*
        Computes the Jacobian (just the position, not the orientation)
 
        :param Q: An N element array containing the current joint angles in radians
        :param p_eff_N: A 3 element vector containing translation from the last joint to the end effector in the last joints frame of reference
 
        Output
        :return: A 3xN 2D matrix containing the Jacobian matrix
    */

    //Position of the end effector in global frame
    //FVector p_eff = position(Q, -1, p_eff_N);
    FVector p_eff = position(Q, -1, p_eff_N); //EndEff->GetRootComponent()->GetComponentLocation();


    TArray<FFloatMatrix> jacobian_matrix; 
    FVector p_eff_minus_this_p;
    FVector k2;
    jacobian_matrix = InitMatrix(3, NJOINTS); // create a 3xNJOINTS matrix

    for (int i = 0; i < NJOINTS; i++) {

        // Difference in the position of the end effector in the global frame
        //and this joint in the global frame
        p_eff_minus_this_p = p_eff - Joint[i]->GetRootComponent()->GetComponentLocation();
            
        //Axes
        k2 = k[i];
        FVector jacVector = FVector::CrossProduct(k2, p_eff_minus_this_p);
        jacobian_matrix[0].C[i] = jacVector.X; // jacobian in form      | x1 x2 x3 x4 x5 .. |
        jacobian_matrix[1].C[i] = jacVector.Y;  //                      | y1 y2 y3 y4 y5 .. |
        jacobian_matrix[2].C[i] = jacVector.Z; //                       | z1 z2 z3 z4 z5 .. |
    }
    return jacobian_matrix;
}



