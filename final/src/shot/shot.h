#ifndef SHOT_H
#define SHOT_H

#include <QHash>
#include <QString>
#include <QGLShaderProgram>
#include <qgl.h>
#include <common.h>
#include <CS123Algebra.h>

#include "nmsphere.h"
class DrawEngine;

struct rope
{
int numVerts;
Vector4 start;
Vector4 end;
Vector4* pts;
Vector4* norms;
vec2<REAL>* texs;
float sag;

/*~rope()
{
    delete[] texs;
    delete[] pts;
    delete[] norms;
}*/
};

class Shot
{
public:
    //In constructor, setup all things that can just sit there
    //(geometry, etc, things that do not require changing gl state)
    Shot(DrawEngine* parent,QHash<QString, QGLShaderProgram *>* shad, QHash<QString, GLuint>* tex, QHash<QString, Model>* mod);

    ~Shot();

    //In begin, initialize things that could not have been initialized beforehand
    //(gl state)
    virtual void begin();

    //called every frame before draw.
    virtual void update();

    //draw!
    virtual void draw();

    rope makeRopeLine(Vector4 pt1, Vector4 pt2, float radius=.05, float sag = 0);
    void drawRope(rope r, bool useSag = false);

    void drawSphere();
    void drawNailBall();

protected:
    //increment at the beginning of update or something.
    int m_framesElapsed;
    int m_lifespan;
    DrawEngine* m_engine;    

    /**
      Resources taken from engine:
     **/
    /*
    QHash<QString, QGLShaderProgram *>          shader_programs_; ///hash map of all shader programs
    QHash<QString, QGLFramebufferObject *>      framebuffer_objects_; ///hash map of all framebuffer objects
    QHash<QString, Model>                       models_; ///hashmap of all models
    QHash<QString, GLuint>
    */

    QHash<QString, QGLShaderProgram *>*          shader_programs_; ///hash map of all shader programs
    //QHash<QString, QGLFramebufferObject *>      framebuffer_objects_; ///hash map of all framebuffer objects
    QHash<QString, Model>*                       models_; ///hashmap of all models
    QHash<QString, GLuint>*                      textures_;

};

extern "C"{
    extern void APIENTRY glActiveTexture(GLenum);
}
#endif // SHOT_H
