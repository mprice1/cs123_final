#ifndef POLARANIMATED_H
#define POLARANIMATED_H

#include <shot.h>
#include <Structs.cpp>

class PolarAnimated : public Shot
{
public:
    PolarAnimated(DrawEngine* parent,QHash<QString, QGLShaderProgram *>* shad, QHash<QString, GLuint>* tex, QHash<QString, Model>* mod);
    ~PolarAnimated();

    void begin();

    //called every frame before draw.
    void update();

    //draw!
    void draw();

//    static QList<Shapes>* makeShapes(int numshapes, double radius);
//    static QList<Shapes>* makeRectShapes(int numx, int numy, double xmin, double xmax, double ymin, double ymax);
//    static QList<Shapes>* makeRectShapesNonRand(int numx, int numy, double xmin, double xmax, double ymin, double ymax);
//    void drawStaticShapes(QList<Shapes>* shapes);

    static QList<Shapes>* makeRing(int num, double radius);

private:
    GLUquadric     * m_quadric;
    //float mFPS, m_increment;
    double m_framecount;
    QList<Shapes>* m_shapes;
    QList<Shapes>* m_shapes_ring;
    QList<rope>* m_ropes;

};

#endif // POLARANIMATED_H
