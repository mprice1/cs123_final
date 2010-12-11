#ifndef ORBSHOT_H
#define ORBSHOT_H
#include "shot.h"

class OrbShot : public Shot
{
public:
    OrbShot(DrawEngine* parent,QHash<QString, QGLShaderProgram *>* shad, QHash<QString, GLuint>* tex, QHash<QString, Model>* mod);
    ~OrbShot();

    //In begin, initialize things that could not have been initialized beforehand
    //(gl state)
    void begin();

    //called every frame before draw.
    void update();

    //draw!
    void draw();

    rope ropeFromNailVecs(Vector4 nv1, Vector4 nv2, float rad=.05, float sag =0);
    void drawNailFromVec(Vector4 nv, bool usebrad = false);

protected:
    Vector4* nlist;
    Vector4* blist;
    rope* rlist;

    int numNails;
    int numBrads;
    int numRopes;
};

#endif // ORBSHOT_H
