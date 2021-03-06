#include "polarclusters.h"
#include <polarshapes.h>
#include <drawengine.h>
#include <CS123Common.h>

#define NUM_CLUSTERS 12
#define wtf .1

PolarClusters::PolarClusters(DrawEngine* parent,QHash<QString, QGLShaderProgram *>* shad, QHash<QString, GLuint>* tex, QHash<QString, Model>* mod) : Shot(parent,shad,tex, mod), m_quadric(NULL), m_shapes_lists(), m_locations()//, m_ropes()
{
    //lasts 150 frames
    m_lifespan = 800;
    //m_lifespan = 200;
    //m_increment = 0;
    //mFPS = 30;
    m_framecount = 0;
    m_fadespan = 50;

    for(int i = 0; i < NUM_CLUSTERS; i++){
        QList<Shapes>* l = new QList<Shapes>();
        m_shapes_lists.append(l);
    }

    ground = new NMPlane(100,80);
}

PolarClusters::~PolarClusters(){
    for (int i = 0; i < m_shapes_lists.length(); i++){
        QList<Shapes>* l = m_shapes_lists.at(i);
        if(l){
            l->clear();
            delete l;
            l = NULL;
        }
    }
    delete ground;
    m_locations.clear();
    //m_ropes.clear();
    gluDeleteQuadric(m_quadric);
}
extern "C"{
    extern void APIENTRY glActiveTexture(GLenum);
}
void PolarClusters::update(){

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    if(m_framecount>=m_lifespan)
    {
        m_engine->endShot();
    }
    int frames = m_lifespan - m_fadespan;

    if(m_framecount == frames)
    {
        m_engine->fadeShots(m_lifespan - frames);
    }
}

void PolarClusters::begin(){
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

    QList<Shapes>* temp = NULL;


    QList<Shapes>* l = m_shapes_lists.at(1);
    temp = PolarShapes::makeShapes(10,30);
    l->append(*temp);
    temp->clear();
    delete temp;

    l = m_shapes_lists.at(2);
    temp = PolarShapes::makeShapes(10,30);
    l->append(*temp);
    temp->clear();
    delete temp;

    l = m_shapes_lists.at(3);
    temp = PolarShapes::makeShapes(40,25);
    l->append(*temp);
    temp->clear();
    delete temp;


    l = m_shapes_lists.at(4);
    temp = PolarShapes::makeShapes(10,10);
    l->append(*temp);
    temp->clear();
    delete temp;

    l = m_shapes_lists.at(5);
    temp = PolarShapes::makeShapes(70,25);
    l->append(*temp);
    temp->clear();
    delete temp;

    l = m_shapes_lists.at(6);
    temp = PolarShapes::makeShapes(70,25);
    l->append(*temp);
    temp->clear();
    delete temp;

    l = m_shapes_lists.at(7);
    temp = PolarShapes::makeShapes(170,40);
    l->append(*temp);
    temp->clear();
    delete temp;

    l = m_shapes_lists.at(8);
    temp = PolarShapes::makeShapes(100,30);
    l->append(*temp);
    temp->clear();
    delete temp;

    l = m_shapes_lists.at(9);
    temp = PolarShapes::makeShapes(70,30);
    l->append(*temp);
    temp->clear();
    delete temp;

    l = m_shapes_lists.at(10);
    temp = PolarShapes::makeShapes(1600,100);
    l->append(*temp);
    temp->clear();
    delete temp;

//    l = m_shapes_lists.at(11);
//    Shapes s;
//    s.shape = 0;
//    s.t.y = -350;
//    s.t.x = 15;
//    s.t.z = 0;
//    l->append(s);
//    Shapes s2;
//    s2.shape = 0;
//    s2.t.y = -360;
//    s2.t.x = 5;
//    s2.t.z = 0;
//    l->append(s2);

//    rope r = makeRopeLine(Vector4(wtf * s.t.x,wtf * s.t.y,-.1,1),Vector4(wtf * s2.t.x,wtf * s2.t.y,-.1,1),.5 * wtf,100 * wtf * wtf);
//    m_ropes.append(r);

//    Shapes s3;
//    s3.shape = 0;
//    s3.t.y = -570;
//    s3.t.x = -15;
//    s3.t.z = 0;
//    l->append(s3);
//    Shapes s4;
//    s4.shape = 0;
//    s4.t.y = -570;
//    s4.t.x = -20;
//    s4.t.z = 0;
//    l->append(s4);

//    r = makeRopeLine(Vector4(wtf * s3.t.x,wtf * s3.t.y,-.1,1),Vector4(wtf * s4.t.x,wtf * s4.t.y,-.1,1),.5 * wtf,20 * wtf * wtf);
//    m_ropes.append(r);

    Translation t;
    m_locations.append(t);
    m_locations.append(t);
    m_locations.append(t);
    m_locations.append(t);
    m_locations.append(t);
    m_locations.append(t);
    m_locations.append(t);
    m_locations.append(t);
    m_locations.append(t);
    m_locations.append(t);
    m_locations.append(t);
    m_locations.append(t);
    t.x = 30;
    t.y = -150;
    m_locations[1] = t;
    t.x = -30;
    t.y = -400;
    m_locations[2] = t;
    t.x = 15;
    t.y = -530;
    m_locations[3] = t;
    t.x = 30;
    t.y = -480;
    m_locations[4] = t;
    t.x = 30;
    t.y = -670;
    m_locations[5] = t;
    t.x = -30;
    t.y = -690;
    m_locations[6] = t;
    t.x = 5;
    t.y = -770;
    m_locations[7] = t;
    t.x = -25;
    t.y = -860;
    m_locations[8] = t;
    t.x = 30;
    t.y = -875;
    m_locations[9] = t;
    t.x = 0;
    t.y = -1030;
    m_locations[10] = t;
//    t.x = 0;
//    t.y = 0;
//    m_locations[11] = t;




    //m_engine->camera_.eye.y += .7;
    m_engine->camera_.eye.x += 0;
    m_engine->camera_.eye.y += 2;
    m_engine->camera_.eye.z += 8;

    m_engine->perspective_camera();

}
//draw!
void PolarClusters::draw(){
    glMatrixMode(GL_MODELVIEW);

    m_framecount++;

    double framecount = m_framecount;

    glPushMatrix();
//    glRotated(framecount / 3,0,-1,0);
//    double scale = (60 - framecount) / 20;
//    scale = MAX(scale, .2);
//    glScaled(scale, scale, scale);

    //cout << m_framecount << endl;
    double speed = framecount / 8;
    //speed = MIN(speed, framecount / 5);

    glTranslated(0,0,12+speed);

    glPushMatrix();
    glTranslatef(0,-.5,100);
    glScalef(100,1,1000);

glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D,textures_->value(CRACK_COLOR));
shader_programs_->value(CRACK_SHADER)->setUniformValue("colormap",0);
glActiveTexture(GL_TEXTURE1);
glBindTexture(GL_TEXTURE_2D,textures_->value(CRACK_NORM));
shader_programs_->value(CRACK_SHADER)->setUniformValue("normalmap",1);
shader_programs_->value(CRACK_SHADER)->bind();
shader_programs_->value(CRACK_SHADER)->setUniformValue("eyept",m_engine->camera_.eye.x, m_engine->camera_.eye.y, m_engine->camera_.eye.z);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,textures_->value(CRACK_COLOR));

    ground->draw();
shader_programs_->value(CRACK_SHADER)->release();

    glPopMatrix();

    glPushMatrix();
    glScaled(.1,.1,.1);
    glRotated(90,1,0,0);




    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textures_->value("cube_map_1"));
    shader_programs_->value(NAIL_SHADER)->bind();
    shader_programs_->value(NAIL_SHADER)->setUniformValue("CubeMap",GL_TEXTURE0);
    shader_programs_->value(NAIL_SHADER)->setUniformValue("eyept",m_engine->camera_.eye.x, m_engine->camera_.eye.y, m_engine->camera_.eye.z);

    for(int i = 0; i < m_shapes_lists.length(); i++){
        QList<Shapes>* l = m_shapes_lists.at(i);
        Translation t = m_locations.at(i);
        glPushMatrix();
        glTranslated(t.x,t.y,0);

        for(int j = 0; j < l->length(); j++){
            Shapes s = l->at(j);

            glPushMatrix();
            glTranslated(s.t.x,s.t.y,0);
            glRotated(s.r.angle, s.r.x,s.r.y,s.r.z);
            //glTranslated(s.t.x,s.t.y,5 * sin(m_framecount / 20 + s.polt));
            //glTranslated(s.t.x,s.t.y,(40 - s.polr) * sin(m_framecount / 20 - s.polr / 15) / (m_framecount / 20 - s.polr / 15));
            //gluCylinder(m_quadric,.25,0,10,10,10);

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
        glPopMatrix();
    }


    shader_programs_->value(NAIL_SHADER)->release();


//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D,textures_->value(ROPE_OCC));
//    shader_programs_->value(ROPE_SHADER)->setUniformValue("colormap",0);

//    glActiveTexture(GL_TEXTURE1);
//    glBindTexture(GL_TEXTURE_2D,textures_->value(ROPE_NORM));

//    shader_programs_->value(ROPE_SHADER)->setUniformValue("normalmap",1);


//    //shader_programs_->value(ROPE_SHADER)->setUniformValue("sag",(GLfloat)(0.5 + 0.01*(m_framesElapsed%100)));
//    shader_programs_->value(ROPE_SHADER)->setUniformValue("eyept",m_engine->camera_.eye.x, m_engine->camera_.eye.y, m_engine->camera_.eye.z);
//    shader_programs_->value(ROPE_SHADER)->bind();


//    //float wtf=25;
//   for (int i = 0; i < m_ropes.length(); i++){
//       glPushMatrix();
//       glScaled(1/((double) wtf),1/((double) wtf),1/((double) wtf));

//       rope r = m_ropes.at(i);

//       //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
//       drawRope(r,true);

//       glPopMatrix();
//   }

//   shader_programs_->value(ROPE_SHADER)->release();


    glPopMatrix();
    glPopMatrix();
}
