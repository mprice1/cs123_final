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
};

#endif // ORBSHOT_H
