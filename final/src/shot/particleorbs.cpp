#include "particleorbs.h"
#include "drawengine.h"


particleOrbs::particleOrbs(DrawEngine* parent,QHash<QString, QGLShaderProgram *>* shad, QHash<QString, GLuint>* tex, QHash<QString, Model>* mod) : Shot (parent,shad,tex,mod)
    {
    force = 0.2;
    numParticles = 25;
    particles = new particle[numParticles];
    //srand(time(NULL));
    //init particles
        for(int i=0; i<numParticles; i++)
        {
            float x,y,z;
            particles[i].pos = Vector4(0,0,0,1);
            particles[i].acc = Vector4(0,0,0,0);
            x = random() % 1000 / 100.0;
            y = random() % 1000 / 100.0;
            z = random() % 1000 / 100.0;
            particles[i].dir = Vector4(x,y,0,0);
        }

    }

particleOrbs::~particleOrbs()
{

}

    //In begin, initialize things that could not have been initialized beforehand
    //(gl state)
void particleOrbs::begin()
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
void particleOrbs::update()
{
    float clr = 0.8 - m_framesElapsed/1000.0;
    if(clr>0)
        glClearColor(clr,clr,clr,0.0f);
m_framesElapsed++;


//UPDATE EACH PARTICLE

for(int i=0; i<numParticles; i++)
{
    particles[i].pos += particles[i].dir * .001;
    particles[i].dir += particles[i].acc;
   // particles[i].acc -=    (particles[i].pos - Vector4(0,0,0,1));
}


}

    //draw!
void particleOrbs::draw()
{
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0,0,-10);

    glEnable(GL_TEXTURE_2D);
    //glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA_SATURATE,GL_ONE);

    glBindTexture(GL_TEXTURE_2D,textures_->value(SPRITE_ONE));
    glBegin(GL_QUADS);

    float h = 1.0;
    for(int i=0; i<numParticles/2; i++)
    {
        glTexCoord2f(0,0);
        glVertex3f(particles[i].pos.x,particles[i].pos.y,particles[i].pos.z);

        glTexCoord2f(1,0);
        glVertex3f(particles[i].pos.x+h,particles[i].pos.y,particles[i].pos.z);

        glTexCoord2f(1,1);
        glVertex3f(particles[i].pos.x+h,particles[i].pos.y+h,particles[i].pos.z);

        glTexCoord2f(0,1);
        glVertex3f(particles[i].pos.x,particles[i].pos.y+h,particles[i].pos.z);
    }
    glEnd();

    glBindTexture(GL_TEXTURE_2D,textures_->value(SPRITE_TWO));
    glBegin(GL_QUADS);
    for(int i=numParticles/2; i<numParticles; i++)
    {
        glTexCoord2f(0,0);
        glVertex3f(particles[i].pos.x,particles[i].pos.y,particles[i].pos.z);

        glTexCoord2f(1,0);
        glVertex3f(particles[i].pos.x+h,particles[i].pos.y,particles[i].pos.z);

        glTexCoord2f(1,1);
        glVertex3f(particles[i].pos.x+h,particles[i].pos.y+(h*1.5),particles[i].pos.z);

        glTexCoord2f(0,1);
        glVertex3f(particles[i].pos.x,particles[i].pos.y+(h*1.5),particles[i].pos.z);
    }
    glEnd();





}

void particleOrbs::drawQuad()
{
    glTexCoord2f(0,0);
    glVertex3f(0,0,0);
    glTexCoord2f(1,0);
    glVertex3f(1,0,0);
    glTexCoord2f(1,1);
    glVertex3f(1,1,0);
    glTexCoord2f(0,1);
    glVertex3f(0,1,0);
}


