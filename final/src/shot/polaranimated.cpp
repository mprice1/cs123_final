#include "polaranimated.h"
#include <polarshapes.h>
#include <drawengine.h>
#include <CS123Common.h>

PolarAnimated::PolarAnimated(DrawEngine* parent,QHash<QString, QGLShaderProgram *>* shad, QHash<QString, GLuint>* tex, QHash<QString, Model>* mod) : Shot(parent,shad,tex, mod), m_quadric(NULL)
{
    //lasts 150 frames
    m_lifespan = 150000;
    //m_increment = 0;
    //mFPS = 30;
    m_framecount = 0;
    m_shapes = new QList<Shapes>();
    m_shapes_ring = new QList<Shapes>();
    m_ropes = new QList<rope>();
    ground = new NMPlane(6,6);
}

PolarAnimated::~PolarAnimated(){
    if(m_shapes){
        m_shapes->clear();
        delete m_shapes;
        m_shapes = NULL;
    }
    if(m_shapes_ring){
        m_shapes_ring->clear();
        delete m_shapes_ring;
        m_shapes_ring = NULL;
    }
    delete ground;
    gluDeleteQuadric(m_quadric);
}
extern "C"{
    extern void APIENTRY glActiveTexture(GLenum);
}
void PolarAnimated::update(){

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

}

void PolarAnimated::begin(){
    srand(time(NULL));

    glShadeModel(GL_SMOOTH);
    glClearColor(0.8f,0.8f,0.8f,0.0f);
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


    m_shapes->clear();
    QList<Shapes>* temp = NULL;

    temp = PolarShapes::makeShapes(400,40);
    m_shapes->append(*temp);
    temp->clear();
    delete temp;

    temp = PolarAnimated::makeRing(9,55);
    m_shapes_ring->append(*temp);
    temp->clear();
    delete temp;

    //temp = PolarShapes::makeRectShapesNonRand(20,100,-40,80,-120,200);
    //    temp = PolarShapes::makeRectShapesNonRand(80,80,-160,160,-160,160);
    //    m_shapes->append(*temp);
    //    temp->clear();
    //    delete temp;

    //    temp = PolarShapes::makeRectShapes(5,5,-50,50,-50,50);
    //    m_shapes->append(*temp);
    //    temp->clear();
    //    delete temp;
    for (int i = 0; i < m_shapes_ring->length(); i++){
        Shapes s = m_shapes_ring->at(i);

        Shapes s2;
        if (i + 1  == m_shapes_ring->length())
            s2 = m_shapes_ring->at(0);
        else
            s2 = m_shapes_ring->at(i + 1);

        float wtf=25;
        rope r = makeRopeLine(Vector4(s.t.x/wtf,s.t.y/wtf,-.1,1),Vector4(s2.t.x/wtf,s2.t.y/wtf,-.1,1),0.025,1.5);
        m_ropes->append(r);
    }

    m_engine->camera_.eye.y += .7;
    m_engine->perspective_camera();

}
//draw!
void PolarAnimated::draw(){
    glMatrixMode(GL_MODELVIEW);

    m_framecount++;

    double framecount = m_framecount * .3;

    glPushMatrix();
    glRotated(framecount / 3,0,-1,0);
    double scale = (60 - framecount) / 20;
    scale = MAX(scale, .2);
    glScaled(scale, scale, scale);


    glPushMatrix();
    glTranslatef(0,-1.0,0);
    glScalef(100,1,50);
    //shader stuff
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D,textures_->value(CRACK_COLOR));
shader_programs_->value(CRACK_SHADER)->setUniformValue("colormap",0);
glActiveTexture(GL_TEXTURE1);
glBindTexture(GL_TEXTURE_2D,textures_->value(CRACK_NORM));
shader_programs_->value(CRACK_SHADER)->setUniformValue("normalmap",1);
shader_programs_->value(CRACK_SHADER)->bind();
shader_programs_->value(CRACK_SHADER)->setUniformValue("eyept",m_engine->camera_.eye.x, m_engine->camera_.eye.y, m_engine->camera_.eye.z);
    //glEnable(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D,textures_->value(CRACK_COLOR));
    //end shader stuff
    ground->draw();
shader_programs_->value(CRACK_SHADER)->release();

    glPopMatrix();

    glPushMatrix();
    glScaled(.1,.1,.1);
    glRotated(90,1,0,0);



//    glPushMatrix();
//    glTranslated(0,0,2);
//    gluDisk(m_quadric,0,100,4,4);
//    gluDisk(m_quadric,0,400,30,30);
//    glPopMatrix();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textures_->value("cube_map_1"));
    shader_programs_->value(NAIL_SHADER)->bind();
    shader_programs_->value(NAIL_SHADER)->setUniformValue("CubeMap",GL_TEXTURE0);
    shader_programs_->value(NAIL_SHADER)->setUniformValue("eyept",m_engine->camera_.eye.x, m_engine->camera_.eye.y, m_engine->camera_.eye.z);

    for(int i = 0; i < m_shapes->length(); i++){
        Shapes s = m_shapes->at(i);

        glPushMatrix();
        glTranslated(s.t.x,s.t.y,0);
        glRotated(s.r.angle, s.r.x,s.r.y,s.r.z);
        //glTranslated(s.t.x,s.t.y,5 * sin(m_framecount / 20 + s.polt));
        //glTranslated(s.t.x,s.t.y,(40 - s.polr) * sin(m_framecount / 20 - s.polr / 15) / (m_framecount / 20 - s.polr / 15));
        //gluCylinder(m_quadric,.25,0,10,10,10);

        double temp = framecount / 15 - s.polr / 10 - 3;
        temp = MAX(temp, .1);
        glTranslated(0,0,10 * sin(temp) / (temp * temp * temp * temp));

        glRotated(-90,1,0,0);
        glScaled(5,5,5);
        if (s.shape){
            glCallList(models_->value(BRAD_MODEL).idx);
        } else {
            glScaled(1,2,1);
            glCallList(models_->value(NAIL_MODEL).idx);
        }
        glPopMatrix();
    }

    for (int i = 0; i < m_shapes_ring->length(); i++){
        Shapes s = m_shapes_ring->at(i);

        glPushMatrix();
        glTranslated(s.t.x,s.t.y,0);

        glRotated(-90,1,0,0);
        glScaled(5,5,5);

        glScaled(1,2,1);
        glCallList(models_->value(NAIL_MODEL).idx);

        glPopMatrix();
    }


    shader_programs_->value(NAIL_SHADER)->release();

     glActiveTexture(GL_TEXTURE0);
     glBindTexture(GL_TEXTURE_2D,textures_->value(ROPE_OCC));
     shader_programs_->value(ROPE_SHADER)->setUniformValue("colormap",0);

     glActiveTexture(GL_TEXTURE1);
     glBindTexture(GL_TEXTURE_2D,textures_->value(ROPE_NORM));

     shader_programs_->value(ROPE_SHADER)->setUniformValue("normalmap",1);


     //shader_programs_->value(ROPE_SHADER)->setUniformValue("sag",(GLfloat)(0.5 + 0.01*(m_framesElapsed%100)));
     shader_programs_->value(ROPE_SHADER)->setUniformValue("eyept",m_engine->camera_.eye.x, m_engine->camera_.eye.y, m_engine->camera_.eye.z);
     shader_programs_->value(ROPE_SHADER)->bind();


     float wtf=25;
    for (int i = 0; i < m_ropes->length(); i++){
        glPushMatrix();
        glScaled(wtf,wtf,wtf);

        rope r = m_ropes->at(i);

        //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        drawRope(r,true);

        glPopMatrix();
    }

    shader_programs_->value(ROPE_SHADER)->release();

    glPopMatrix();
    glPopMatrix();
}

QList<Shapes>* PolarAnimated::makeRing(int num, double radius){
    QList<Shapes>* toReturn = new QList<Shapes>();

    for (int i = 0; i < num; i++){
        Shapes s;

        double frac = (double) i / (double) num;
        frac *= 2 * M_PI;

        s.polr = radius;
        s.polt = frac;

        s.t.x = radius * cos(frac);
        s.t.y = radius * sin(frac);

        toReturn->append(s);
    }
    return toReturn;
}
