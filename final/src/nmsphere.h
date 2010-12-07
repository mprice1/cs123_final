#ifndef NMSPHERE_H
#define NMSPHERE_H
#include <CS123Algebra.h>

class NMSphere
{
public:
    NMSphere(int stacks, int slices);
    ~NMSphere();
    void makeGeometry(int stacks, int slices);


    Vector4* verts;
    Vector4* norms;
    Vector4* tans;
    vec2<REAL>* texs;

    int numVerts;
};

#endif // NMSPHERE_H
