/**
  A simple OpenGL drawing engine.

  @author psastras
**/

#include "drawengine.h"
#include "glm.h"
#include <qgl.h>
#include <QKeyEvent>
#include <QGLContext>
#include <QHash>
#include <QGLShaderProgram>
#include <QQuaternion>
#include <QVector3D>
#include <QString>
#include <GL/glu.h>
#include <iostream>
#include <QFile>
#include <QGLFramebufferObject>
#define GL_GLEXT_PROTOTYPES
#include <GL/glext.h>

#include <testShot.h>
#include <polarshapes.h>
#include <orbshot.h>
#include <spiralorbshot.h>
#include <tempshot.h>
#include <intronailshot.h>
#include <polaranimated.h>
#include <polaranimated2.h>
#include <nailsinfield.h>
#include <upropeshot.h>
#include <polarclusters.h>
#include <threenailballs.h>
#include <twonailballs.h>

/*
 Common shader and texture names are defined as constants
 For all shots to use!
 */


using std::cout;
using std::endl;

extern "C"{
    extern void APIENTRY glActiveTexture (GLenum);
}

/**
  @paragraph DrawEngine ctor.  Expects a Valid OpenGL context and the viewport's current
  width and height.  Initializes the draw engine.  Loads models,textures,shaders,
  and allocates framebuffers.  Also sets up OpenGL to begin drawing.

  @param context The current OpenGL context this drawing engine is associated with.
  Probably should be the context from the QGLWidget.

  @param w The viewport width used to allocate void drawUnitRope();the correct framebuffer size.
  @param h The viewport heigh used to alloacte the correct framebuffer size.

**/
DrawEngine::DrawEngine(const QGLContext *context,int w,int h) : context_(context) {
    //initialize ogl settings
    glEnable(GL_TEXTURE_2D);
    glFrontFace(GL_CCW);
    glDisable(GL_DITHER);
    glDisable(GL_LIGHTING);
    glShadeModel(GL_FLAT);
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    //init member variables
    previous_time_ = 0.0f;
    camera_.center.x = 0.f,camera_.center.y = 0.f,camera_.center.z = 0.f;
    camera_.eye.x = 0.f,camera_.eye.y = 0.0f,camera_.eye.z = 2.f;
    camera_.up.x = 0.f,camera_.up.y = 1.f,camera_.up.z = 0.f;
    camera_.near = 0.1f,camera_.far = 100.f;
    camera_.fovy = 60.f;

    nm_sphere = new NMSphere(12,12);

    //init resources - so i heard you like colored text?
    cout << "Using OpenGL Version " << glGetString(GL_VERSION) << endl << endl;
    //ideally we would now check to make sure all the OGL functions we use are supported
    //by the video card.  but that's a pain to do so we're not going to.
    cout << "\033[31;1mLoading Resources\033[0m" << endl;
    load_models();
    load_shaders();
    load_textures();
    create_fbos(w,h);
    cout << "\033[31;1mRenderingi...\033[0m" << endl;


    /****************************************/
    //  SHOT ARRAY

    m_shots = new QList<Shot*>();
    m_curShot =0;


    //m_shots->append(  new testShot(this, &shader_programs_, &textures_, &models_));
    //m_shots->append(  new introNailShot(this, &shader_programs_, &textures_, &models_));
    //m_shots->append(  new PolarAnimated(this, &shader_programs_, &textures_, &models_));

    m_shots->append(new PolarClusters(this,  &shader_programs_, &textures_, &models_));
     m_shots->append(new threeNailBalls(this,  &shader_programs_, &textures_, &models_));
    m_shots->append(new upRopeShot(this,  &shader_programs_, &textures_, &models_));
    m_shots->append(  new PolarAnimated2(this, &shader_programs_, &textures_, &models_));
    m_shots->append(  new nailSinField(this, &shader_programs_, &textures_, &models_));
   m_shots->append(  new spiralOrbShot(this, &shader_programs_, &textures_, &models_));


 //  m_shots->append(  new PolarShapes(this, &shader_programs_, &textures_, &models_));

    m_shots->at(m_curShot)->begin();
    /****************************************/
}

/**
  @paragraph Dtor
**/
DrawEngine::~DrawEngine() {
    foreach(QGLShaderProgram *sp,shader_programs_)
        delete sp;
    foreach(QGLFramebufferObject *fbo,framebuffer_objects_)
        delete fbo;
    foreach(GLuint id,textures_)
        ((QGLContext *)(context_))->deleteTexture(id);
    foreach(Model m,models_)
        glmDelete(m.model);

    for(int i=0;i<m_shots->size();i++)
    {
        delete m_shots->at(i);
    }
    delete m_shots;

}

/**
  @paragraph Loads models used by the program.  Called by the ctor once upon
  initialization.
**/
void DrawEngine::load_models() {
    cout << "\033[1mLoading models...\033[0m" << endl;

    /*models_["dragon"].model = glmReadOBJ("models/xyzrgb_dragon.obj");
    glmUnitize(models_["dragon"].model);
    models_["dragon"].idx = glmList(models_["dragon"].model,GLM_SMOOTH);
    cout << "\t \033[32m/course/cs123/data/mesh/xyzrgb_dragon_old.obj\033[0m" << endl;*/

    models_[NAIL_MODEL].model = glmReadOBJ("models/nail.obj");
    glmUnitize(models_[NAIL_MODEL].model);
    models_[NAIL_MODEL].idx = glmList(models_[NAIL_MODEL].model,GLM_SMOOTH);

    models_[BRAD_MODEL].model = glmReadOBJ("models/brad.obj");
    glmUnitize(models_[BRAD_MODEL].model);
    models_[BRAD_MODEL].idx = glmList(models_[BRAD_MODEL].model,GLM_SMOOTH);

    /*
    //Create grid
    models_["grid"].idx = glGenLists(1);
    glNewList(models_["grid"].idx,GL_COMPILE);
    float r = 1.f,dim = 10,delta = r * 2 / dim;
    for(int y = 0; y < dim; ++y) {
        glBegin(GL_QUAD_STRIP);
        for(int x = 0; x <= dim; ++x) {
            glVertex2f(x * delta - r,y * delta - r);
            glVertex2f(x * delta - r,(y + 1) * delta - r);
        }
        glEnd();
    }
    glEndList();
    cout << "\t \033[32mgrid compiled\033[0m" << endl;
    models_["skybox"].idx = glGenLists(1);
    glNewList(models_["skybox"].idx,GL_COMPILE);
    */

    /*
    //Be glad we wrote this for you...ugh.
    glBegin(GL_QUADS);
    float fExtent = 50.f;
    glTexCoord3f(1.0f,-1.0f,-1.0f); glVertex3f(fExtent,-fExtent,-fExtent);
    glTexCoord3f(-1.0f,-1.0f,-1.0f);glVertex3f(-fExtent,-fExtent,-fExtent);
    glTexCoord3f(-1.0f,1.0f,-1.0f);glVertex3f(-fExtent,fExtent,-fExtent);
    glTexCoord3f(1.0f,1.0f,-1.0f); glVertex3f(fExtent,fExtent,-fExtent);
    glTexCoord3f(1.0f,-1.0f,1.0f);glVertex3f(fExtent,-fExtent,fExtent);
    glTexCoord3f(1.0f,-1.0f,-1.0f); glVertex3f(fExtent,-fExtent,-fExtent);
    glTexCoord3f(1.0f,1.0f,-1.0f);  glVertex3f(fExtent,fExtent,-fExtent);
    glTexCoord3f(1.0f,1.0f,1.0f); glVertex3f(fExtent,fExtent,fExtent);
    glTexCoord3f(-1.0f,-1.0f,1.0f);  glVertex3f(-fExtent,-fExtent,fExtent);
    glTexCoord3f(1.0f,-1.0f,1.0f); glVertex3f(fExtent,-fExtent,fExtent);
    glTexCoord3f(1.0f,1.0f,1.0f);  glVertex3f(fExtent,fExtent,fExtent);
    glTexCoord3f(-1.0f,1.0f,1.0f); glVertex3f(-fExtent,fExtent,fExtent);
    glTexCoord3f(-1.0f,-1.0f,-1.0f); glVertex3f(-fExtent,-fExtent,-fExtent);
    glTexCoord3f(-1.0f,-1.0f,1.0f);glVertex3f(-fExtent,-fExtent,fExtent);
    glTexCoord3f(-1.0f,1.0f,1.0f); glVertex3f(-fExtent,fExtent,fExtent);
    glTexCoord3f(-1.0f,1.0f,-1.0f);glVertex3f(-fExtent,fExtent,-fExtent);
    glTexCoord3f(-1.0f,1.0f,-1.0f);glVertex3f(-fExtent,fExtent,-fExtent);
    glTexCoord3f(-1.0f,1.0f,1.0f);glVertex3f(-fExtent,fExtent,fExtent);
    glTexCoord3f(1.0f,1.0f,1.0f);glVertex3f(fExtent,fExtent,fExtent);
    glTexCoord3f(1.0f,1.0f,-1.0f);glVertex3f(fExtent,fExtent,-fExtent);
    glTexCoord3f(-1.0f,-1.0f,-1.0f);glVertex3f(-fExtent,-fExtent,-fExtent);
    glTexCoord3f(-1.0f,-1.0f,1.0f);glVertex3f(-fExtent,-fExtent,fExtent);
    glTexCoord3f(1.0f,-1.0f,1.0f); glVertex3f(fExtent,-fExtent,fExtent);
    glTexCoord3f(1.0f,-1.0f,-1.0f);glVertex3f(fExtent,-fExtent,-fExtent);
    glEnd();
    glEndList();
    cout << "\t \033[32mskybox compiled\033[0m" << endl;
    */
}
/**
  @paragraph Loads shaders used by the program.  Caleed by the ctor once upon
  initialization.
**/
void DrawEngine::load_shaders() {
    shader_programs_["reflect"] = new QGLShaderProgram(context_);
    shader_programs_["reflect"]->addShaderFromSourceFile(QGLShader::Vertex,"shaders/reflect.vert");
    shader_programs_["reflect"]->addShaderFromSourceFile(QGLShader::Fragment,"shaders/reflect.frag");
   shader_programs_["reflect"]->link();

    shader_programs_["ropetest"] = new QGLShaderProgram(context_);
    shader_programs_["ropetest"]->addShaderFromSourceFile(QGLShader::Vertex,"shaders/ropetest.vert");
    shader_programs_["ropetest"]->addShaderFromSourceFile(QGLShader::Fragment,"shaders/ropetest.frag");
   shader_programs_["ropetest"]->link();

   shader_programs_[CRACK_SHADER] = new QGLShaderProgram(context_);
   shader_programs_[CRACK_SHADER]->addShaderFromSourceFile(QGLShader::Vertex,"shaders/crack.vert");
   shader_programs_[CRACK_SHADER]->addShaderFromSourceFile(QGLShader::Fragment,"shaders/crack.frag");
  shader_programs_[CRACK_SHADER]->link();
}
/**
  @paragraph Loads textures used by the program.  Called by the ctor once upon
  initialization.
**/
void DrawEngine::load_textures() {
    cout << "Loading textures..." << endl;
    QList<QFile *> fileList;
    fileList.append(new QFile("textures/astra/posx.jpg"));
    fileList.append(new QFile("textures/astra/negx.jpg"));
    fileList.append(new QFile("textures/astra/posy.jpg"));
    fileList.append(new QFile("textures/astra/negy.jpg"));
    fileList.append(new QFile("textures/astra/posz.jpg"));
    fileList.append(new QFile("textures/astra/negz.jpg"));
    textures_["cube_map_1"] = load_cube_map(fileList);

    textures_["rope_occlusion"] = load_texture(new QFile("textures/ropeOcc.png"));
    textures_["rope_normal"] = load_texture(new QFile("textures/ropeNorm2.png"));
    textures_[CRACK_COLOR] = load_texture(new QFile("textures/crackColor.png"));
    textures_[CRACK_NORM] = load_texture(new QFile("textures/crackNorm.png"));

}
/**
  @paragraph Creates the intial framebuffers for drawing.  Called by the ctor once
  upon initialization.

  @todo Finish filling this in.

  @param w:    the viewport width
  @param h:    the viewport height
**/
void DrawEngine::create_fbos(int w,int h) {

    //Allocate the main framebuffer object for rendering the scene to
    //This needs a depth attachment.
    framebuffer_objects_["fbo_0"] = new QGLFramebufferObject(w,h,QGLFramebufferObject::Depth,
                                                             GL_TEXTURE_2D,GL_RGB16F_ARB);
    framebuffer_objects_["fbo_0"]->format().setSamples(16);
    //Allocate the secondary framebuffer obejcts for rendering textures to (post process effects)
    //These do not require depth attachments.
    framebuffer_objects_["fbo_1"] = new QGLFramebufferObject(w,h,QGLFramebufferObject::NoAttachment,
                                                             GL_TEXTURE_2D,GL_RGB16F_ARB);
    //You need to create another framebuffer here.  Look up two lines to see how to do this... =.=
    framebuffer_objects_["fbo_2"] = new QGLFramebufferObject(w,h,QGLFramebufferObject::NoAttachment,
                                                             GL_TEXTURE_2D,GL_RGB16F_ARB);
}
/**
  @paragraph Reallocates all the framebuffers.  Called when the viewport is
  resized.

  @param w:    the viewport width
  @param h:    the viewport height
**/
void DrawEngine::realloc_framebuffers(int w,int h) {
    foreach(QGLFramebufferObject *fbo,framebuffer_objects_)  {
        const QString &key = framebuffer_objects_.key(fbo);
        QGLFramebufferObjectFormat format = fbo->format();
        delete fbo;
        framebuffer_objects_[key] = new QGLFramebufferObject(w,h,format);
    }
}

/**
  @paragraph Should render one frame at the given elapsed time in the program.
  Assumes that the GL context is valid when this method is called.

  @todo Finish filling this in

  @param time: the current program time in milliseconds
  @param w:    the viewport width
  @param h:    the viewport height

**/
void DrawEngine::draw_frame(float time,int w,int h) {
    fps_ = 1000.f / (time - previous_time_),previous_time_ = time;
    m_w = w;
    m_h = h;
    //Render the scene to a framebuffer
    //framebuffer_objects_["fbo_0"]->bind();
    perspective_camera(w,h);    
    //render_scene(time,w,h);
    //framebuffer_objects_["fbo_0"]->release();
    //copy the rendered scene into framebuffer 1
    //framebuffer_objects_["fbo_0"]->blitFramebuffer(framebuffer_objects_["fbo_1"],
    //                                               QRect(0,0,w,h),framebuffer_objects_["fbo_0"],
    //                                               QRect(0,0,w,h),GL_COLOR_BUFFER_BIT,GL_NEAREST);

    //you may want to add code here
    //orthogonal_camera(w,h);
    //glBindTexture(GL_TEXTURE_2D,framebuffer_objects_["fbo_1"]->texture());
    //textured_quad(w,h,true);
    //glBindTexture(GL_TEXTURE_2D, 0);
    //framebuffer_objects_["fbo_2"]->bind();
    //shader_programs_["brightpass"]->bind();
    //glBindTexture(GL_TEXTURE_2D,framebuffer_objects_["fbo_1"]->texture());
    //textured_quad(w,h,true);
    //shader_programs_["brightpass"]->release();
    //glBindTexture(GL_TEXTURE_2D,0);
    //framebuffer_objects_["fbo_2"]->release();

    //Uncomment this section in step 2 of the lab...
    //float scales[] = {4.f,8.f,16.f,32.f};
    /*for(int i = 0; i < 4; ++i) {
        render_blur(w /scales[i],h /scales[i]);
        glBindTexture(GL_TEXTURE_2D,framebuffer_objects_["fbo_1"]->texture());
        glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE,GL_ONE);
        glTranslatef(0.f,(scales[i] - 1)* -h,0.f);
        textured_quad(w * scales[i],h * scales[i],false);
        glDisable(GL_BLEND);
        glBindTexture(GL_TEXTURE_2D,0);
     }*/


    m_shots->at(m_curShot)->update();
    m_shots->at(m_curShot)->draw();
}

void DrawEngine::endShot()
{
    /**
      RESET THE FUCKING CAMERA
    **/
    camera_.center.x = 0.f,camera_.center.y = 0.f,camera_.center.z = 0.f;
    camera_.eye.x = 0.f,camera_.eye.y = 0.0f,camera_.eye.z = 2.f;
    camera_.up.x = 0.f,camera_.up.y = 1.f,camera_.up.z = 0.f;
    camera_.near = 0.1f,camera_.far = 100.f;
    camera_.fovy = 60.f;
    /**  OKAY? **/


    if(m_curShot>=m_shots->size()-1)
    {
    //some sort of end action...
    }
    else
    {
        m_curShot++;
        m_shots->at(m_curShot)->begin();
    }
}



/*
 Will be useful so keep around.
 */
/**
  @paragraph Draws a textured quad. The texture most be bound and unbound
  before and after calling this method - this method assumes that the texture
  has been bound before hand.

  @param w: the width of the quad to draw
  @param h: the height of the quad to draw
  @param flip: flip the texture vertically

**/
void DrawEngine::textured_quad(int w,int h,bool flip) {
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f,flip ? 1.0f : 0.0f);
    glVertex2f(0.0f,0.0f);
    glTexCoord2f(1.0f,flip ? 1.0f : 0.0f);
    glVertex2f(w,0.0f);
    glTexCoord2f(1.0f,flip ? 0.0f : 1.0f);
    glVertex2f(w,h);
    glTexCoord2f(0.0f,flip ? 0.0f : 1.0f);
    glVertex2f(0.0f,h);
    glEnd();
}




/**
  @paragraph Called to switch to the perspective OpenGL camera.
  Used to render the scene regularly with the current camera parameters.

  @param w: the viewport width
  @param h: the viewport height

**/
void DrawEngine::perspective_camera(int w,int h) {
    float ratio = w / static_cast<float>(h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(camera_.fovy,ratio,camera_.near,camera_.far);
    gluLookAt(camera_.eye.x,camera_.eye.y,camera_.eye.z,
              camera_.center.x,camera_.center.y,camera_.center.z,
              camera_.up.x,camera_.up.y,camera_.up.z);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void DrawEngine::perspective_camera() {
    int w= m_w;
    int h = m_h;
    float ratio = w / static_cast<float>(h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(camera_.fovy,ratio,camera_.near,camera_.far);
    gluLookAt(camera_.eye.x,camera_.eye.y,camera_.eye.z,
              camera_.center.x,camera_.center.y,camera_.center.z,
              camera_.up.x,camera_.up.y,camera_.up.z);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/**
  @paragraph Called to switch to an orthogonal OpenGL camera.
  Useful for rending a textured quad across the whole screen.

  @param w: the viewport width
  @param h: the viewport height

**/
void DrawEngine::orthogonal_camera(int w,int h) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,static_cast<float>(w),static_cast<float>(h),0.f,-1.f,1.f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/**
  @paragraph Called when the viewport has been resized. Needs to
  resize the camera perspective and reallocate the framebuffer
  sizes.

  @param w: the viewport width
  @param h: the viewport height

**/
void DrawEngine::resize_frame(int w,int h) {
    glViewport(0,0,w,h);
    realloc_framebuffers(w,h);
}

/**
  @paragraph Called by GLWidget when the mouse is dragged.  Rotates the camera
  based on mouse movement.

  @param p0: the old mouse position
  @param p1: the new mouse position
**/
void DrawEngine::mouse_drag_event(float2 p0,float2 p1) {
    int dx = p1.x - p0.x,dy = p1.y - p0.y;
    QQuaternion qq = QQuaternion::fromAxisAndAngle(0, 1, 0, -dx / 5.0);
    QVector3D qv3 = qq.rotatedVector(QVector3D(camera_.eye.x, camera_.eye.y,
                                               camera_.eye.z));
    qq = QQuaternion::fromAxisAndAngle(qq.rotatedVector(QVector3D(1, 0, 0)), dy / 5.0);
    qv3 = qq.rotatedVector(qv3);
    camera_.eye.x = qv3.x(), camera_.eye.y = qv3.y(), camera_.eye.z = qv3.z();
}

/**
  @paragraph Called by GLWidget when the mouse wheel is turned. Zooms the camera in
  and out.

  @param dx: The delta value of the mouse wheel movement.
**/
void DrawEngine::mouse_wheel_event(int dx) {
    if((camera_.center - camera_.eye).getMagnitude() > .5 || dx < 0)
        camera_.eye += (camera_.center - camera_.eye).getNormalized() * dx * .005;
}

/**
  @paragraph Loads the cube map into video memory.

  @param files: a list of files containing the cube map images (should be length
  six) in order.
  @return The assigned OpenGL id to the cube map.
**/
GLuint DrawEngine::load_cube_map(QList<QFile *> files) {
    GLuint id;
    glGenTextures(1,&id);
    glBindTexture(GL_TEXTURE_CUBE_MAP,id);
    for(unsigned i = 0; i < 6; ++i) {
        QImage image,texture;
        image.load(files[i]->fileName());
        image = image.mirrored(false,true);
        texture = QGLWidget::convertToGLFormat(image);
        texture = texture.scaledToWidth(1024,Qt::SmoothTransformation);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,3,3,texture.width(),texture.height(),0,GL_RGBA,GL_UNSIGNED_BYTE,texture.bits());
        gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_X +i, 3, texture.width(), texture.height(), GL_RGBA, GL_UNSIGNED_BYTE, texture.bits());
        cout << "\t \033[32m" << files[i]->fileName().toStdString() << "\033[0m" << endl;
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_NEAREST_MIPMAP_NEAREST);
    glBindTexture(GL_TEXTURE_CUBE_MAP,0);
    return id;
}


/**
  Loads a texture into video memory.

  Param: a Qfile, texture
**/

GLuint DrawEngine::load_texture(QFile * tex)
{

 QImage image,texture;
 GLuint id;

 image.load(tex->fileName());
 image = image.mirrored(false,true);
 texture = QGLWidget::convertToGLFormat(image);


 glGenTextures(1,&id);

 glBindTexture(GL_TEXTURE_2D,id);



 gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture.width(), texture.height(), GL_RGBA, GL_UNSIGNED_BYTE, texture.bits());
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

 return id;
}


/**
  @paragraph Called when a key has been pressed in the GLWidget.

  @param event: The key press event associated with the current key press.
  **/
void DrawEngine::key_press_event(QKeyEvent *event) {
    switch(event->key()) {

    }
}
