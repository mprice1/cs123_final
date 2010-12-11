#include "particleorbs.h"
#include "drawengine.h"

#define RADIUS 5

particleOrbs::particleOrbs(DrawEngine* parent,QHash<QString, QGLShaderProgram *>* shad, QHash<QString, GLuint>* tex, QHash<QString, Model>* mod) : Shot (parent,shad,tex,mod)
    {
    force = 0.2;
    numParticles = 600;
    particles = new particle[numParticles];
    srand(time(NULL));
    //init particles
        for(int i=0; i<numParticles; i++)
        {
            float xp,yp,zp;
            xp = (double) rand() / (double) RAND_MAX;
            xp *= 2;
            xp -= 1;
            xp *= RADIUS;

#if 1
            yp = (double) rand() / (double) RAND_MAX;
            yp *= 2;
            yp -= 1;
            yp *= RADIUS;

            zp = RADIUS * RADIUS - xp * xp - yp * yp;
            zp = sqrt(zp);


            int neg = rand() % 2;
            if (neg)
                zp = -1 * zp;

            particles[i].pos = Vector4(xp,yp,zp,1);

#else
            yp = RADIUS * RADIUS - xp * xp;
            yp = sqrt(yp);



            int neg = rand() % 2;
            if (neg)
                yp = -1 * yp;

            particles[i].pos = Vector4(xp,yp,0,1);

#endif

            //float x,y,z;
            particles[i].acc = Vector4(0,0,0,0);
//            x = (double) rand() / (double) RAND_MAX;
//            x *= 2;
//            x -= 1;
//            x *= 5;
//            y = (double) rand() / (double) RAND_MAX;
//            y *= 2;
//            y -= 1;
//            y *= 5;
//            z = (double) rand() / (double) RAND_MAX;
//            z *= 2;
//            z -= 1;
//            z *= 5;
            float t = 75;
            int neg2 = rand() % 2;
            if (neg2){
                yp = -1 * yp;
                xp = -1 * xp;
            }
            particles[i].dir = Vector4(-t * yp,t * xp,0,0);
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
    glClearColor(0.0f,0.0f,0.0f,0.0f);
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

m_framesElapsed++;


//UPDATE EACH PARTICLE

for(int i=0; i<numParticles; i++)
{
    particle& p = particles[i];
   particles[i].pos += particles[i].dir * .001;
    particles[i].dir += particles[i].acc;
    Vector4 rad = Vector4(0,0,0,1) - particles[i].pos;
    particles[i].acc =    15.0 * rad.getNormalized();
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
    //glEnable(GL_BLEND);
    //glDisable(GL_DEPTH_TEST);
  //  glBlendFunc(GL_SRC_ALPHA,GL_ONE);

    //glBindTexture(GL_TEXTURE_2D,textures_->value(SPRITE_ONE));


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,textures_->value(SPRITE_ONE));
    shader_programs_->value(SPRITE_SHADER)->bind();
    shader_programs_->value(SPRITE_SHADER)->setUniformValue("colormap",0);


    glBegin(GL_QUADS);
    float h = .8;
    for(int i=0; i<numParticles/4; i++)
    {
        glTexCoord2f(0,1);
        glVertex3f(particles[i].pos.x,particles[i].pos.y,particles[i].pos.z);

        glTexCoord2f(1,1);
        glVertex3f(particles[i].pos.x+h,particles[i].pos.y,particles[i].pos.z);

        glTexCoord2f(1,0);
        glVertex3f(particles[i].pos.x+h,particles[i].pos.y+(h*1.5),particles[i].pos.z);

        glTexCoord2f(0,0);
        glVertex3f(particles[i].pos.x,particles[i].pos.y+(h*1.5),particles[i].pos.z);
    }
    glEnd();

    glBindTexture(GL_TEXTURE_2D,textures_->value(SPRITE_TWO));
    glBegin(GL_QUADS);
    for(int i=numParticles/4; i<numParticles/2; i++)
    {
        glTexCoord2f(0,0);
        glVertex3f(particles[i].pos.x,particles[i].pos.y,particles[i].pos.z);

        glTexCoord2f(1,0);
        glVertex3f(particles[i].pos.x+h,particles[i].pos.y,particles[i].pos.z);

        glTexCoord2f(1,1);
        glVertex3f(particles[i].pos.x+h,particles[i].pos.y+(h),particles[i].pos.z);

        glTexCoord2f(0,1);
        glVertex3f(particles[i].pos.x,particles[i].pos.y+(h),particles[i].pos.z);
    }
    glEnd();

    glDisable(GL_TEXTURE_2D);






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


