#include "twonailballs.h"
#include "drawengine.h"


twoNailBalls::twoNailBalls(DrawEngine* parent,QHash<QString, QGLShaderProgram *>* shad, QHash<QString, GLuint>* tex, QHash<QString, Model>* mod):Shot(parent,shad,tex,mod)
{
m_lifespan = 700;
p1 = Vector4(-5,-2,-5,1);
p2 = Vector4(5,2,-5,1);
myrope = new rope;
*myrope = makeRopeLine(p1,p2,.1,10);
}

twoNailBalls::~twoNailBalls()
{

}

//In begin, initialize things that could not have been initialized beforehand
//(gl state)
void twoNailBalls::begin()
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
void twoNailBalls::update()
{
    m_framesElapsed++;
    if(m_framesElapsed >= m_lifespan)
    {
        m_engine->endShot();
    }
    if(m_framesElapsed%150 == 0)
    {
        m_engine->endShot();
    }

    //ANIMATE OUR SHIT!!!
    p1.x += 0.005;
    p1.y += 0.001;
    p2.x -= 0.005;
    p2.y -= 0.001;
    float sag =  ((p1-p2).getMagnitude()) / 4.0;
    cout<<sag<<endl;
    *myrope = makeRopeLine(p1,p2,.05,sag);

}

//draw!
void twoNailBalls::draw()
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
    glTranslatef(p1.x,p1.y,p1.z);
    drawNailBall();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(p2.x,p2.y,p2.z);
    drawNailBall();
    glPopMatrix();

    //****************
    //RELEASE NAIL SHADER
    shader_programs_->value(NAIL_SHADER)->release();

    //***** BIND ROPE SHADER
    shader_programs_->value(ROPE_SHADER)->bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,textures_->value(ROPE_OCC));
    shader_programs_->value(ROPE_SHADER)->setUniformValue("colormap",0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,textures_->value(ROPE_NORM));

    shader_programs_->value(ROPE_SHADER)->setUniformValue("normalmap",1);

    shader_programs_->value(ROPE_SHADER)->setUniformValue("sag",(GLfloat)(0.5 + 0.01*(m_framesElapsed%100)));
    shader_programs_->value(ROPE_SHADER)->setUniformValue("eyept",m_engine->camera_.eye.x, m_engine->camera_.eye.y, m_engine->camera_.eye.z);


    drawRope(*myrope,true);
    shader_programs_->value(ROPE_SHADER)->release();

}
