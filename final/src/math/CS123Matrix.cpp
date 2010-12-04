/*!
   @file   CS123Matrix.cpp
   @author Travis Fischer (fisch0920@gmail.com)
   @date   Fall 2008
   
   @brief
      Provides basic functionality for a templated, arbitrarily-sized matrix.
      You will need to fill this file in for the Camtrans assignment.

**/

// CS123Matrix.h can be found in /course/cs123/include/algebra
#include <CS123Matrix.h>
#include <iostream>
#include <math.h>

//@name Routines which generate specific-purpose transformation matrices
//@{---------------------------------------------------------------------
// @returns the scale matrix described by the vector
Matrix4x4 getScaleMat(const Vector4 &v) {

    // @TODO: [CAMTRANS] Fill this in...
    //return Matrix4x4::identity();

    return Matrix4x4(v.x, 0, 0, 0, 0, v.y, 0, 0, 0, 0, v.z, 0, 0, 0, 0, 1);

}

// @returns the translation matrix described by the vector
Matrix4x4 getTransMat(const Vector4 &v) {

    // @TODO: [CAMTRANS] Fill this in...
    //return Matrix4x4::identity();
    //REAL matData[16];
   // memset(matData,0,sizeof(REAL));


    return Matrix4x4(1,0,0,v.x,0,1,0,v.y,0,0,1,v.z,0,0,0,1);
    //return Matrix4x4(matData);

}

// @returns the rotation matrix about the x axis by the specified angle
Matrix4x4 getRotXMat (const REAL radians) {

    // @TODO: [CAMTRANS] Fill this in...
    //return Matrix4x4::identity();
        //REAL matData[16];
	REAL cosAngle = cos(radians);
	REAL sinAngle = sin(radians);
        /*matData[0] = 1;
	matData[5] = cosAngle;
	matData[6] = -1*sinAngle;
	matData[9] = sinAngle;
	matData[10] = cosAngle;
	matData[15] = 1;


        return Matrix4x4(matData);*/
        return Matrix4x4(1,0,0,0,0,cosAngle,-1*sinAngle,0,0,sinAngle,cosAngle,0,0,0,0,1);
	

}

// @returns the rotation matrix about the y axis by the specified angle
Matrix4x4 getRotYMat (const REAL radians) {

    // @TODO: [CAMTRANS] Fill this in...
    //return Matrix4x4::identity();
	REAL cosAngle = cos(radians);
	REAL sinAngle = sin(radians);
        /*REAL matData[16];
	matData[0] = cosAngle;
	matData[2] = sinAngle;
	matData[5] = 1;
	matData[8] = -1*sinAngle;
	matData[10] = cosAngle;
	matData[15] = 1;
        return Matrix4x4(matData);*/
        return Matrix4x4(cosAngle,0,sinAngle,0,0,1,0,0,-1*sinAngle,0,cosAngle,0,0,0,0,1);
}

// @returns the rotation matrix about the z axis by the specified angle
Matrix4x4 getRotZMat (const REAL radians) {

    // @TODO: [CAMTRANS] Fill this in...
    //return Matrix4x4::identity();
	REAL cosAngle = cos(radians);
	REAL sinAngle = sin(radians);
        //cout<<cosAngle<<endl;
        //REAL matData[16];
        /*matData[0] = cosAngle;
	matData[4] = sinAngle;
	matData[10] = 1;
	matData[1] = -1*sinAngle;
	matData[5] = cosAngle;
        matData[15] = 1;*/

        return Matrix4x4(cosAngle, -1*sinAngle, 0, 0, sinAngle, cosAngle, 0,0,0,0,1,0,0,0,0,1);

       // cout<<"?"<<matData[9]<<endl;
        //return Matrix4x4(matData);
	

}

// @returns the rotation matrix around the vector and point by the specified angle
    //the angle is in degrees
Matrix4x4 getRotMat  (const Vector4 &p, const Vector4 &v, const REAL a) {

       /* cout<<"point in: "<<p.x<<" "<<p.y<<" "<<p.z<<" "<<p.w<<endl;
        cout<<"vec in:"<<v.x<<" "<<v.y<<" "<<v.z<<" "<<v.w<<endl;
        cout<<"angle in:"<<a<<endl;*/

	Matrix4x4 trans = getTransMat(p);

	Matrix4x4 invtrans = getInvTransMat(p);


	REAL theta = atan2(v.z,v.x);
        REAL phi = -1 * atan2(v.y , sqrt(v.x*v.x + v.z*v.z));

        Matrix4x4 M1 = getRotYMat(theta);
        Matrix4x4 M2 = getRotZMat(phi);
        Matrix4x4 M2Inv = getInvRotZMat(phi);
        Matrix4x4 M1Inv = getInvRotYMat(theta);
        //Matrix4x4 M3 = getRotXMat(M_PI*a/180);
        Matrix4x4 M3 = getRotXMat(a);
	

        Matrix4x4 toReturn = trans * M1Inv * M2Inv * M3 * M2 * M1 * invtrans;



        return toReturn;
        //return trans * M1 * M2 * M3 * M2Inv * M1Inv * invtrans;

}


// @returns the inverse scale matrix described by the vector
Matrix4x4 getInvScaleMat(const Vector4 &v) {


    return getScaleMat(Vector4(1/v.x,1/v.y,1/v.z,0));

}

// @returns the inverse translation matrix described by the vector
Matrix4x4 getInvTransMat(const Vector4 &v) {


    return getTransMat(v*-1);

}

// @returns the inverse rotation matrix about the x axis by the specified angle
Matrix4x4 getInvRotXMat (const REAL radians) {


        return getRotXMat(-1*radians);

}

// @returns the inverse rotation matrix about the y axis by the specified angle
Matrix4x4 getInvRotYMat (const REAL radians) {


        return getRotYMat(-1*radians);

}

// @returns the inverse rotation matrix about the z axis by the specified angle
Matrix4x4 getInvRotZMat (const REAL radians) {

    return getRotZMat(-1*radians);

}

// @returns the inverse rotation matrix around the vector and point by the specified angle
Matrix4x4 getInvRotMat  (const Vector4 &p, const Vector4 &v, const REAL a) {

    // @TODO: [CAMTRANS] Fill this in...
    //return Matrix4x4::identity();
        return getRotMat(p,v,-1*a);

}

void printMatrix(Matrix4x4 mat)
{
    for(int row=0;row<4;row++)
    {
        for(int col=0;col<4;col++)
        {
            cout<<mat.data[row*4+col]<<" ";
        }
        cout<<endl;
    }
}

void printVector4(Vector4 v)
{
    cout<<"("<<v.x<<", "<<v.y<<", "<<v.z<<", "<<v.w<<")"<<endl;
}

//@}---------------------------------------------------------------------

