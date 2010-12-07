#ifndef NMPLANE_H
#define NMPLANE_H

#include <CS123Algebra.h>

class NMPlane
{
public:
    NMPlane(int stacks, int slices);
    void makeGeometry(int stacks, int slices);


    Vector4* verts;
    Vector4* norms;
    Vector4* tans;
    vec2<REAL>* texs;

    int numVerts;
};

#endif // NMPLANE_H
