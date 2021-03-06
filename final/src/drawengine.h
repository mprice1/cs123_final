#ifndef DRAWENGINE_H
#define DRAWENGINE_H

#include <QHash>
#include <QList>
#include <QString>
#include <qgl.h>
#include "glm.h"
#include "common.h"
#include "glwidget.h"

/*
 Common shader and texture names are defined as constants
 For all shots to use!
 */
#define ROPE_SHADER "ropetest"
#define ROPE_OCC "rope_occlusion"
#define ROPE_NORM "rope_normal"

#define NAIL_SHADER "reflect"
#define NAIL_MODEL "nailgeom"
#define BRAD_MODEL "bradgeom"

#define CRACK_SHADER "crackshader"
#define CRACK_COLOR "crackcolor"
#define CRACK_NORM "cracknormal"

#define SPRITE_ONE "sprite_one"
#define SPRITE_TWO "sprite_two"

#define PERLIN_TEXTURE "perlin"

#define SPRITE_SHADER "spriteshade"


class QGLContext;
class QGLShaderProgram;
class QFile;
class QGLFramebufferObject;
class QKeyEvent;
class Shot;
class NMSphere;


class DrawEngine {
public:

    //ctor and dtor
    DrawEngine(const QGLContext *context, int w, int h, GLWidget* widget);
    ~DrawEngine();

    //methods
    void draw_frame(float time, int w, int h);
    void resize_frame(int w, int h);
    void mouse_wheel_event(int dx);
    void mouse_drag_event(float2 p0, float2 p1);
    void key_press_event(QKeyEvent *event);
    //getters and setters
    float fps() { return fps_; }

    void perspective_camera();
    void orthogonal_camera();
    void endShot();
    void prevShot();

    void fadeShots(int frames);


    void text(double x, double y, QString s);

    //member variables
    Camera                                      camera_; ///a simple camera struct
    NMSphere* nm_sphere;
protected:

    //methods
    void perspective_camera(int w, int h);
    void orthogonal_camera(int w, int h);
    void textured_quad(int w, int h, bool flip);
    void realloc_framebuffers(int w, int h);
    void load_models();
    void load_textures();
    void load_shaders();
    GLuint load_cube_map(QList<QFile *> files);
    GLuint load_texture(QFile * tex);
    void create_fbos(int w, int h);
    int m_w, m_h;
    GLWidget* m_widget;
    int frameNumber;

    int m_fadetimer;
    int m_fade;


    //member variables
    QHash<QString, QGLShaderProgram *>          shader_programs_; ///hash map of all shader programs
    QHash<QString, QGLFramebufferObject *>      framebuffer_objects_; ///hash map of all framebuffer objects
    QHash<QString, Model>                       models_; ///hashmap of all models
    QHash<QString, GLuint>                      textures_; ///hashmap of all textures
    const QGLContext                            *context_; ///the current OpenGL context to render to
    float                                       previous_time_, fps_; ///the previous time and the fps counter


    int     m_curShot;//current shot
    QList<Shot*>* m_shots; //list of shots


};

#endif // DRAWENGINE_H
