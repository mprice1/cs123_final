#ifndef THREENAILBALLS_H
#define THREENAILBALLS_H
#include <shot.h>

class threeNailBalls : public Shot
{
public:
    threeNailBalls(DrawEngine* parent,QHash<QString, QGLShaderProgram *>* shad, QHash<QString, GLuint>* tex, QHash<QString, Model>* mod);
    ~threeNailBalls();

    //In begin, initialize things that could not have been initialized beforehand
    //(gl state)
    void begin();

    //called every frame before draw.
    void update();

    //draw!
    void draw();
};

#endif // THREENAILBALLS_H
