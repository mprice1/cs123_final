#ifndef INTRONAILSHOT_H
#define INTRONAILSHOT_H
#include "shot.h"

class introNailShot : public Shot
{
public:
    introNailShot(DrawEngine* parent,QHash<QString, QGLShaderProgram *>* shad, QHash<QString, GLuint>* tex, QHash<QString, Model>* mod);
    ~introNailShot();

    //In begin, initialize things that could not have been initialized beforehand
    //(gl state)
    void begin();

    //called every frame before draw.
    void update();

    //draw!
    void draw();

};

#endif // INTRONAILSHOT_H
