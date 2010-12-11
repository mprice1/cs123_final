#ifndef PARTICLEORBS_H
#define PARTICLEORBS_H
#include "shot.h"

struct particle
{
Vector4 pos;
Vector4 dir;
Vector4 acc;
};

class particleOrbs : public Shot
{
public:
    particleOrbs(DrawEngine* parent,QHash<QString, QGLShaderProgram *>* shad, QHash<QString, GLuint>* tex, QHash<QString, Model>* mod);
    ~particleOrbs();

    //In begin, initialize things that could not have been initialized beforehand
    //(gl state)
    void begin();

    //called every frame before draw.
    void update();

    //draw!
    void draw();
    void drawQuad();


    float force;
    particle* particles;
    int numParticles;


};
#endif // PARTICLEORBS_H
