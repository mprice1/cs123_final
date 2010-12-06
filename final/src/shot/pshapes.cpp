#include "pshapes.h"
#include <math.h>

PShapes::PShapes(DrawEngine* parent,QHash<QString, QGLShaderProgram *>* shad, QHash<QString, GLuint>* tex, QHash<QString, Model>* mod) : Shot(parent,shad,tex,mod), m_quadric(NULL), m_shapesLocations(), m_shapesRotations()
{
    //lasts 150 frames
    m_lifespan = 150000;
    //m_increment = 0;
    //mFPS = 30;
    m_framecount = 0;
}

PShapes::~PShapes(){
    gluDeleteQuadric(m_quadric);
}

void PShapes::begin(){
    glShadeModel(GL_SMOOTH);
    glClearColor(0.2f,0.2f,0.2f,0.0f);
        glEnable(GL_LIGHTING);

        //Make some lights!
    glEnable(GL_LIGHT0);
    float lightpos[4];
    lightpos[0]=0.f;
    lightpos[1]=0.f;
    lightpos[2]=2.f;
    lightpos[3]=0.f;

    glLightfv(GL_LIGHT0,GL_POSITION,lightpos);

    m_quadric = gluNewQuadric();

    for (int i = 0; i < 30; i++){
        for (int j = 0; j < 30; j++){
            float fi = i;
            float fj = j;
            double r1 = (double) rand() / (double) RAND_MAX;
            r1 *= 2;
            r1 -= 1;

            double r2 = (double) rand() / (double) RAND_MAX;
            r2 *= 2;
            r2 -= 1;

            double r3 = (double) rand() / (double) RAND_MAX;
            r3 *= 2;
            r3 -= 1;

            double r4 = (double) rand() / (double) RAND_MAX;
            r4 *= 2;
            r4 -= 1;

            double r5 = (double) rand() / (double) RAND_MAX;
            r5 *= 2;
            r5 -= 1;

            double r6 = (double) rand() / (double) RAND_MAX;
            r6 *= 2;
            r6 -= 1;

            Translation l;
            l.x = -30 + 4*j + r1;
            l.y = -15 + 4*i + r2;
            l.z = 0;

            Rotation r;
            r.angle = r3 * 12;
            r.x = r4;
            r.y = r5;
            r.z = r6;


            m_shapesLocations.append(l);
            m_shapesRotations.append(r);
        }
    }

}

//called every frame before draw.
void PShapes::update(){
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

}

//draw!
void PShapes::draw(){
    glMatrixMode(GL_MODELVIEW);

    m_framecount++;
    //float time = m_increment++ / (float)mFPS;
    //    glPushMatrix();
    //    glTranslatef(1,1,1);
    //    gluCylinder(m_quadric,.5,.5,.5,10,10);
    //    glPopMatrix();
    glPushMatrix();
    glScalef(.05,.05,.05);

//    for (int i = 0; i < 5; i++){
//        for (int j = 0; j < 15; j++){
//            float fi = i;
//            float fj = j;
//            glPushMatrix();
//            glTranslatef(-30 + 4*j,-10 + 4*i,3 * sin(m_framecount / 30 + fi / 2 + fj / 2));
//            gluSphere(m_quadric,1,30,30);
//            glPopMatrix();
//        }
//    }

    glPushMatrix();
    glTranslated(0,0,2);
    gluDisk(m_quadric,0,100,4,4);
    glPopMatrix();

    for (int i = 0; i < 30; i++){
        for (int j = 0; j < 30; j++){
            float fi = i;
            float fj = j;
            Translation l = m_shapesLocations.at(i * 15 + j);
            Rotation r = m_shapesRotations.at(i * 15 + j);
            glPushMatrix();
            glRotated(r.angle, r.x,r.y,r.z);
            //glRotated(90,1,0,0);
            glTranslated(l.x,l.y,2 * sin(m_framecount / 50 + fi / 2 + fj / 2));
            //3 * sin(m_framecount / 30 + fi / 2 + fj / 2)
            gluCylinder(m_quadric,.05,.5,10,10,10);
            glPopMatrix();
        }
    }

    glPopMatrix();

}
