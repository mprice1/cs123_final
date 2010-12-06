#ifndef TESTSHOT_H
#define TESTSHOT_H
#include <shot.h>


class testShot : public Shot
{
public:
    testShot(DrawEngine* parent,QHash<QString, QGLShaderProgram *>* shad, QHash<QString, GLuint>* tex, QHash<QString, Model>* mod);

    ~testShot();

    //In begin, initialize things that could not have been initialized beforehand
    //(gl state)
    void begin();

    //called every frame before draw.
    void update();

    //draw!
    void draw();

    /**
        Not inherited methods
     **/
    void drawUnitRope(int tess=1);
    void ropeLine(Vector4 pt1, Vector4 pt2);


};

#endif // TESTSHOT_H
