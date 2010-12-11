#ifndef MANYNAILBALLS_H
#define MANYNAILBALLS_H
#include "shot.h"

struct nailcell
{
    Vector4 pos;
    Vector4 rotvec;
    float amp;
    float rot;
    float scale;
    float freq;
};

class manyNailBalls : public Shot
{
public:
    manyNailBalls(DrawEngine* parent,QHash<QString, QGLShaderProgram *>* shad, QHash<QString, GLuint>* tex, QHash<QString, Model>* mod);
    ~manyNailBalls();

    //In begin, initialize things that could not have been initialized beforehand
    //(gl state)
    void begin();

    //called every frame before draw.
    void update();

    //draw!
    void draw();

protected:
    nailcell* cells;
    int numCells;
    int m_fadespan;
};

#endif // MANYNAILBALLS_H
