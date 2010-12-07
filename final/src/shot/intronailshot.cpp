#include "intronailshot.h"
#include "drawengine.h"

#define MAIN_DEPTH 2.0


introNailShot::introNailShot(DrawEngine* parent,QHash<QString, QGLShaderProgram *>* shad, QHash<QString, GLuint>* tex, QHash<QString, Model>* mod) : Shot(parent,shad,tex,mod)
{

}

introNailShot::~introNailShot()
{

}


void introNailShot::begin()
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


void introNailShot::update()
{
    m_framesElapsed++;
    /*if(m_framesElapsed>= m_lifespan)
    {
        m_engine->endShot();
    }*/
}

//draw!
void introNailShot::draw()
{

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //PANNING
    glTranslatef(-((float)m_framesElapsed)/25.0,0.0,0.0);

    //FIRST NAIL
    glPushMatrix();
    glTranslatef(8.0,0,-MAIN_DEPTH);
    glCallList(models_->value(BRAD_MODEL).idx);
    glPopMatrix();

    //PAIR OF NAILS IN DISTANCE
    glPushMatrix();
    glTranslatef(12.0,0,-2*MAIN_DEPTH);

    glPopMatrix();
}
