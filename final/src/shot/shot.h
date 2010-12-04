#ifndef SHOT_H
#define SHOT_H

#include <QHash>
#include <QString>
#include <QGLShaderProgram>
#include <qgl.h>
#include <common.h>
#include <CS123Algebra.h>
class DrawEngine;


class Shot
{
public:
    //In constructor, setup all things that can just sit there
    //(geometry, etc, things that do not require changing gl state)
    Shot(DrawEngine* parent,QHash<QString, QGLShaderProgram *>* shad, QHash<QString, GLuint>* tex);

    ~Shot();

    //In begin, initialize things that could not have been initialized beforehand
    //(gl state)
    virtual void begin();

    //called every frame before draw.
    virtual void update();

    //draw!
    virtual void draw();
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
    //QHash<QString, Model>                       models_; ///hashmap of all models
    QHash<QString, GLuint>*                      textures_;

};

#endif // SHOT_H
