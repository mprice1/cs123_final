#ifndef TESTMATRIX_H
#define TESTMATRIX_H

#include <CS123Algebra.h>
#include <assert.h>

class testMatrix
{
public:
    testMatrix();
    static void testMatrices();
protected:
    static void compareMatrices(Matrix4x4* a, Matrix4x4* b);
};

#endif // TESTMATRIX_H
