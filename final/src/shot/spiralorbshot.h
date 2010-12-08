#ifndef SPIRALORBSHOT_H
#define SPIRALORBSHOT_H
#include "shot.h"

class spiralOrbShot : public Shot
{
public:
    spiralOrbShot(DrawEngine* parent,QHash<QString, QGLShaderProgram *>* shad, QHash<QString, GLuint>* tex, QHash<QString, Model>* mod);
    ~spiralOrbShot();

    //In begin, initialize things that could not have been initialized beforehand
    //(gl state)
    void begin();

    //called every frame before draw.
    void update();

    //draw!
    void draw();

    rope ropeFromNailVecs(Vector4 nv1, Vector4 nv2, float rad=.05, float sag =0);
    void drawNailFromVec(Vector4 nv, bool brad=false);

protected:
    int n;
    Vector4* nlist;
    rope* rlist;
};
#endif // SPIRALORBSHOT_H
