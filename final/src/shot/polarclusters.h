#ifndef POLARCLUSTERS_H
#define POLARCLUSTERS_H

#include <shot.h>
#include <Structs.cpp>

class PolarClusters : public Shot
{
public:
    PolarClusters(DrawEngine* parent,QHash<QString, QGLShaderProgram *>* shad, QHash<QString, GLuint>* tex, QHash<QString, Model>* mod);
    ~PolarClusters();

    void begin();

    //called every frame before draw.
    void update();

    //draw!
    void draw();

private:
    GLUquadric     * m_quadric;
    //float mFPS, m_increment;
    double m_framecount;
    QList<QList<Shapes>*> m_shapes_lists;
    QList<Translation> m_locations;
    QList<rope> m_ropes;

    NMPlane* ground;

};

#endif // POLARCLUSTERS_H
