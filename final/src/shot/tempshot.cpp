#include "tempshot.h"
#include <drawengine.h>

TempShot::TempShot(DrawEngine* parent,QHash<QString, QGLShaderProgram *>* shad, QHash<QString, GLuint>* tex, QHash<QString, Model>* mod) : Shot(parent,shad,tex, mod)
{
    m_plane = new NMPlane(5,5);
}

TempShot::~TempShot(){
    delete m_plane;
}

void TempShot::begin(){
    glShadeModel(GL_SMOOTH);
    glClearColor(0.8f,0.8f,0.8f,0.0f);
        glEnable(GL_LIGHTING);

        //Make some lights!
    glEnable(GL_LIGHT0);
    float lightpos[4];
    lightpos[0]=0.f;
    lightpos[1]=2.f;
    lightpos[2]=1.f;
    lightpos[3]=0.f;

    glLightfv(GL_LIGHT0,GL_POSITION,lightpos);
}

void TempShot::update(){
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
}

void TempShot::draw(){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,textures_->value(CRACK_COLOR));
    m_plane->draw();
}
