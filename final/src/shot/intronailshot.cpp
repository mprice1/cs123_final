#include "intronailshot.h"
#include "drawengine.h"

#define MAIN_DEPTH 2.0


introNailShot::introNailShot(DrawEngine* parent,QHash<QString, QGLShaderProgram *>* shad, QHash<QString, GLuint>* tex, QHash<QString, Model>* mod) : Shot(parent,shad,tex,mod)
{
    m_lifespan = 1050;
    //m_lifespan = 200;

    m_fadespan = 50;

    //CREATE GROUND PLANE
    nm_plane = new NMPlane(20,40);
    //world pt 1:  11,0,0.5
    //world pt 2:  14,0,-4*MAIN_DEPTH
    r1 = makeRopeLine(Vector4(13,0.0,0.5,1.0), Vector4(17,0.0,-4*MAIN_DEPTH,1.0),.1,15);

    //world pt 1:  12,0,0.5
    //world pt 2:  15,0,0.5
    r2 = makeRopeLine(Vector4(16,0.3,1.35,1.0), Vector4(19,0.3,1.4,1.0),.1,15);
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

    m_engine->camera_.eye.y += 0.6;
    m_engine->perspective_camera();

}


void introNailShot::update()
{
    m_framesElapsed++;


    if(m_framesElapsed>= m_lifespan)
    {
        m_engine->endShot();
    }

    int frames = m_lifespan - m_fadespan;

    if(m_framesElapsed == frames)
    {
        m_engine->fadeShots(m_lifespan - frames);
    }
}

//draw!
void introNailShot::draw()
{

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //PANNING
    glTranslatef(-((float)m_framesElapsed)/50.0,0.0,0.0);

   // glDisable(GL_LIGHTING);
    //glActiveTexture(GL_TEXTURE1);
/*    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D,textures_->value(PERLIN_TEXTURE));
    glBegin(GL_QUADS);
    glTexCoord2f(0,0);
    glVertex3f(0,0,0);
    glTexCoord2f(1,0);
    glVertex3f(1,0,0);
    glTexCoord2f(1,1);
    glVertex3f(1,0,1);
    glTexCoord2f(0,1);
    glVertex3f(0,0,1);
    glEnd();*/


    //GROUND PLANE
    glPushMatrix();
    glTranslatef(0,-0.5,0.5);
    glScalef(200,0,20);
        //shader stuff
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,textures_->value(CRACK_COLOR));
    shader_programs_->value(CRACK_SHADER)->setUniformValue("colormap",0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,textures_->value(CRACK_NORM));
    shader_programs_->value(CRACK_SHADER)->setUniformValue("normalmap",1);

    shader_programs_->value(CRACK_SHADER)->bind();
    shader_programs_->value(CRACK_SHADER)->setUniformValue("eyept",m_engine->camera_.eye.x, m_engine->camera_.eye.y, m_engine->camera_.eye.z);
        //end of shader stuff
    nm_plane->draw();
    shader_programs_->value(CRACK_SHADER)->release();
    glPopMatrix();

    //************NAILS**************************

    //BIND NAIL SHADER
    shader_programs_->value(NAIL_SHADER)->bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textures_->value("cube_map_1"));

    shader_programs_->value(NAIL_SHADER)->setUniformValue("CubeMap",0);

    shader_programs_->value(NAIL_SHADER)->setUniformValue("eyept",m_engine->camera_.eye.x, m_engine->camera_.eye.y, m_engine->camera_.eye.z);

    //FIRST NAIL
    glPushMatrix();
    glTranslatef(8.0,0,-MAIN_DEPTH);
    glCallList(models_->value(BRAD_MODEL).idx);
    glPopMatrix();

    //TWO NAILS IN DISTANCE
    glPushMatrix();
    glTranslatef(12.0,0,-3*MAIN_DEPTH);
    glRotatef(30,0,0,1);
    glCallList(models_->value(BRAD_MODEL).idx);
    glRotatef(-30,0,0,1);
    glTranslatef(0.5,0,0);
    glRotatef(-15,0,0,1);
    glCallList(models_->value(NAIL_MODEL).idx);
    glPopMatrix();

    //REALLY CLOSE NAIL
    glPushMatrix();
    glTranslatef(10,0,0.25);
    glRotatef(15,0,0,1);
    glCallList(models_->value(BRAD_MODEL).idx);
    glPopMatrix();


    //ONE CLOSE ONE FAR WITH ROPE
    glPushMatrix();
    glTranslatef(13,0,0.5);
    glCallList(models_->value(NAIL_MODEL).idx);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(17,0,-4*MAIN_DEPTH);
    glCallList(models_->value(NAIL_MODEL).idx);
    glPopMatrix();
    //world pt 1:  11,0,0.5
    //world pt 2:  14,0,-4*MAIN_DEPTH

    //TWO CLOSE TO SHOW ROPE NORMAL MAP
    glPushMatrix();
    glTranslatef(16,0,1.5);
    glRotatef(30,-1,0,0);
    glCallList(models_->value(BRAD_MODEL).idx);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(19,0,1.5);
    glRotatef(25,-1,0,0);
    glCallList(models_->value(NAIL_MODEL).idx);
    glPopMatrix();



    //RELEASE NAIL SHADER
    shader_programs_->value(NAIL_SHADER)->release();


    //****************ROPES*************************
    //BIND ROPE SHADER
    shader_programs_->value(ROPE_SHADER)->bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,textures_->value(ROPE_OCC));
    shader_programs_->value(ROPE_SHADER)->setUniformValue("colormap",0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,textures_->value(ROPE_NORM));
    shader_programs_->value(ROPE_SHADER)->setUniformValue("normalmap",1);
    shader_programs_->value(ROPE_SHADER)->setUniformValue("sag",(GLfloat)(r1.sag));
    shader_programs_->value(ROPE_SHADER)->setUniformValue("eyept",m_engine->camera_.eye.x, m_engine->camera_.eye.y, m_engine->camera_.eye.z);
    //DRAW ROPE
    drawRope(r1,true);
    drawRope(r2,true);
    shader_programs_->value(ROPE_SHADER)->release();




}
