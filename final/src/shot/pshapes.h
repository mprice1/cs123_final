#ifndef PSHAPES_H
#define PSHAPES_H

#include <shot.h>
#include <QList>
#include <Structs.cpp>

class PShapes : public Shot
{
public:
    PShapes(DrawEngine* parent,QHash<QString, QGLShaderProgram *>* shad, QHash<QString, GLuint>* tex, QHash<QString, Model>* mod);

    ~PShapes();

    //In begin, initialize things that could not have been initialized beforehand
    //(gl state)
    void begin();

    //called every frame before draw.
    void update();

    //draw!
    void draw();

private:
    GLUquadric     * m_quadric;
    //float mFPS, m_increment;
    double m_framecount;
    QList<Translation> m_shapesLocations;
    QList<Rotation> m_shapesRotations;
};

#endif // PSHAPES_H
