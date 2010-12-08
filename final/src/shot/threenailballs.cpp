#include "threenailballs.h"
#include "drawengine.h"


threeNailBalls::threeNailBalls(DrawEngine* parent,QHash<QString, QGLShaderProgram *>* shad, QHash<QString, GLuint>* tex, QHash<QString, Model>* mod):Shot(parent,shad,tex,mod)
{
m_lifespan = 700;
}

threeNailBalls::~threeNailBalls()
{

}

//In begin, initialize things that could not have been initialized beforehand
//(gl state)
void threeNailBalls::begin()
{
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

//called every frame before draw.
void threeNailBalls::update()
{
    m_framesElapsed++;
    if(m_framesElapsed >= m_lifespan)
    {
        m_engine->endShot();
    }
}

//draw!
void threeNailBalls::draw()
{
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //BIND NAIL SHADER
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textures_->value("cube_map_1"));
    shader_programs_->value(NAIL_SHADER)->bind();
    shader_programs_->value(NAIL_SHADER)->setUniformValue("CubeMap",GL_TEXTURE0);
    shader_programs_->value(NAIL_SHADER)->setUniformValue("eyept",m_engine->camera_.eye.x, m_engine->camera_.eye.y, m_engine->camera_.eye.z);
    //******* DO STUFF

    glPushMatrix();
    glRotatef(30,0,0,1);
    glTranslatef(-8+(m_framesElapsed/40.0),0.25*sin(m_framesElapsed/30.0),-2);
    glRotatef(m_framesElapsed/2.0,1,0,0);
    glScalef(.2,.2,.2);
    drawNailBall();
    glPopMatrix();

    glPushMatrix();
    glRotatef(-15,0,0,1);
    glTranslatef(-8+(m_framesElapsed/20.0),2+0.25*sin(m_framesElapsed/10.0),-7);
    drawNailBall();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(2-(m_framesElapsed/200.0),-1+0.25*sin(m_framesElapsed/10.0),11.0-m_framesElapsed/30.0);
    glRotatef(m_framesElapsed/1.0,1,0,0);
    drawNailBall();
    glPopMatrix();



    //****************
    //RELEASE NAIL SHADER
    shader_programs_->value(NAIL_SHADER)->release();

}
