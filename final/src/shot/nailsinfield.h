#ifndef NAILSINFIELD_H
#define NAILSINFIELD_H
#include "shot.h"
#include <Structs.cpp>

class nailSinField : public Shot
{
public:
    nailSinField(DrawEngine* parent,QHash<QString, QGLShaderProgram *>* shad, QHash<QString, GLuint>* tex, QHash<QString, Model>* mod);

    ~nailSinField();

    //In begin, initialize things that could not have been initialized beforehand
    //(gl state)
    void begin();

    //called every frame before draw.
    void update();

    //draw!
    void draw();

private:
    int m_framecount;
    QList<Shapes>* m_shapes;
    NMPlane* nm_plane;
    rope myrope;

};

#endif // NAILSINFIELD_H
