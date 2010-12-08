#ifndef UPROPESHOT_H
#define UPROPESHOT_H
#include <shot.h>

class upRopeShot : public Shot
{
public:
    upRopeShot(DrawEngine* parent,QHash<QString, QGLShaderProgram *>* shad, QHash<QString, GLuint>* tex, QHash<QString, Model>* mod);
    ~upRopeShot();

    //In begin, initialize things that could not have been initialized beforehand
    //(gl state)
    void begin();

    //called every frame before draw.
    void update();

    //draw!
    void draw();

protected:
    rope m_rope;

    rope* r1;
    rope* r2;

    float dh;
};

#endif // UPROPESHOT_H
