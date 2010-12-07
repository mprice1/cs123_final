#ifndef NMPLANE_H
#define NMPLANE_H

#include <CS123Algebra.h>

class NMPlane
{
public:
    NMPlane(int xtess, int ytess);
    ~NMPlane();
    void makeGeometry();
    void draw();


    Vector4* verts;
    Vector4* norms;
    Vector4* tans;
    vec2<REAL>* texs;

    int numVerts;
private:
    int xtess, ytess;
};

#endif // NMPLANE_H
