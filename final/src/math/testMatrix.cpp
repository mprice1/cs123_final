/*
 *
 *  Class to test matrix functions in math/CS123Matrix
 *
 */

#include "testMatrix.h"
#include <iostream>
#include <math.h>
#define PI 3.141592653589793

testMatrix::testMatrix()
{
}

void testMatrix::testMatrices()
{
    Vector4 testTransV = Vector4(4,5,6,1);
    Matrix4x4 testTrans = Matrix4x4(1,0,0,-4,0,1,0,-5,0,0,1,-6,0,0,0,1);
    Matrix4x4 testRotZ90 = Matrix4x4(0.f,-1.f,0.f,0.f,1.f,0.f,0.f,0.f,0.f,0.f,1.f,0.f,0.f,0.f,0.f,1.f);
    Matrix4x4 testRotY90 = Matrix4x4(0.f,0.f,1.f,0.f,0.f,1.f,0.f,0.f,-1.f,0.f,0.f,0.f,0.f,0.f,0.f,1.f);
    Matrix4x4 testRotX90 = Matrix4x4(1.f,0.f,0.f,0.f,0.f,0.f,-1.f,0.f,0.f,1.f,0.f,0.f,0.f,0.f,0.f,1.f);

    //y
    Matrix4x4 testRot1 = getRotMat(Vector4(0,0,0,1),Vector4(0,1,0,1),90);
    //x
    Matrix4x4 testRot2 = getRotMat(Vector4(0,0,0,1),Vector4(1,0,0,1), 90);
    //z
    Matrix4x4 testRot3 = getRotMat(Vector4(0,0,0,1),Vector4(0,0,1,1), 90);


    Matrix4x4 trz90 = getRotZMat(M_PI/2);
    Matrix4x4 try90 = getRotYMat(M_PI/2);
    Matrix4x4 trx90 = getRotXMat(M_PI/2);
    Matrix4x4 trans = getTransMat(testTransV);


    //Test rotation on z axis
    compareMatrices(&testRotZ90,&trz90);

    //Z axis as arbitrary rotation
    compareMatrices(&testRotZ90,&testRot3);

    //inverses are equivalent:
    compareMatrices(&getInvRotZMat(M_PI/2),&getInvRotMat(Vector4(0,0,0,1),Vector4(0,0,1,1),90));

    //Test rotation on y axis
    compareMatrices(&testRotY90,&try90);

    //Y axis as arbitrary rotation
    compareMatrices(&testRotY90,&testRot1);

    //inverses are equivalent:
    compareMatrices(&getInvRotYMat(M_PI/2),&getInvRotMat(Vector4(0,0,0,1),Vector4(0,1,0,1),90));

    //Test rotation on x axis
    compareMatrices(&testRotX90,&trx90);

    //X axis as arbitrary rotation
    compareMatrices(&testRotX90,&testRot2);

    //inverses are equivalent:
    compareMatrices(&getInvRotXMat(M_PI/2),&getInvRotMat(Vector4(0,0,0,1),Vector4(1,0,0,1),90));

    //test translation
    compareMatrices(&testTrans,&trans);

}


void testMatrix::compareMatrices(Matrix4x4* a, Matrix4x4* b)
{
    assert(EQ(a->a,b->a));
    assert(EQ(a->b,b->b));
    assert(EQ(a->c,b->c));
    assert(EQ(a->d,b->d));
    assert(EQ(a->e,b->e));
    assert(EQ(a->f,b->f));
    assert(EQ(a->g,b->g));
    assert(EQ(a->h,b->h));
    assert(EQ(a->i,b->i));
    assert(EQ(a->j,b->j));
    assert(EQ(a->k,b->k));
    assert(EQ(a->l,b->l));
    assert(EQ(a->m,b->m));
    assert(EQ(a->n,b->n));
    assert(EQ(a->o,b->o));
    assert(EQ(a->p,b->p));
}
