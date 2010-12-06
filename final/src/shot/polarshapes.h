#ifndef POLARSHAPES_H
#define POLARSHAPES_H

#include "shot.h"

#include <Structs.cpp>


class PolarShapes : public Shot
{
public:
    PolarShapes(DrawEngine* parent,QHash<QString, QGLShaderProgram *>* shad, QHash<QString, GLuint>* tex, QHash<QString, Model>* mod);

    ~PolarShapes();

    //In begin, initialize things that could not have been initialized beforehand
    //(gl state)
    void begin();

    //called every frame before draw.
    void update();

    //draw!
    void draw();

    static QList<Shapes>* makeShapes(int numshapes, double radius);
    static QList<Shapes>* makeRectShapes(int numx, int numy, double xmin, double xmax, double ymin, double ymax);
    static QList<Shapes>* makeRectShapesNonRand(int numx, int numy, double xmin, double xmax, double ymin, double ymax);


private:
    GLUquadric     * m_quadric;
    //float mFPS, m_increment;
    double m_framecount;
    QList<Shapes>* m_shapes;
};

#endif // POLARSHAPES_H
