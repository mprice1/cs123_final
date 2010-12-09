#ifndef TWONAILBALLS_H
#define TWONAILBALLS_H
#include <shot.h>

class twoNailBalls : public Shot
{
public:
    twoNailBalls(DrawEngine* parent,QHash<QString, QGLShaderProgram *>* shad, QHash<QString, GLuint>* tex, QHash<QString, Model>* mod);
    ~twoNailBalls();

    //In begin, initialize things that could not have been initialized beforehand
    //(gl state)
    void begin();

    //called every frame before draw.
    void update();

    //draw!
    void draw();

protected:
    rope* myrope;
    Vector4 p1;
    Vector4 p2;
};

#endif // TWONAILBALLS_H
