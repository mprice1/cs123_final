#include "upropeshot.h"
#include "drawengine.h"





upRopeShot::upRopeShot(DrawEngine* parent,QHash<QString, QGLShaderProgram *>* shad, QHash<QString, GLuint>* tex, QHash<QString, Model>* mod) : Shot(parent,shad,tex,mod)
    {
    m_lifespan = 1150;
    m_rope = makeRopeLine(Vector4(0,5,0,1), Vector4(0,-5,0,1),.05,100);

    r1 = new rope;
    *r1 = makeRopeLine(Vector4(-2,0,0,1), Vector4(2,0,0,1), .05, 1 );
    //*r2 = makeRopeLine();
    dh = 0;
    }

    upRopeShot::~upRopeShot()
    {
        delete r1;

    }

    //In begin, initialize things that could not have been initialized beforehand
    //(gl state)
    void upRopeShot::begin()
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
    void upRopeShot::update()
    {
        m_framesElapsed++;
        if(m_framesElapsed>=m_lifespan)
        {
            m_engine->endShot();
        }
    }

    //draw!
    void upRopeShot::draw()
    {
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

    //******** BIND ROPE SHADER
        shader_programs_->value(ROPE_SHADER)->bind();
         glActiveTexture(GL_TEXTURE0);
         glBindTexture(GL_TEXTURE_2D,textures_->value(ROPE_OCC));
         shader_programs_->value(ROPE_SHADER)->setUniformValue("colormap",0);

         glActiveTexture(GL_TEXTURE1);
         glBindTexture(GL_TEXTURE_2D,textures_->value(ROPE_NORM));

         shader_programs_->value(ROPE_SHADER)->setUniformValue("normalmap",1);

         shader_programs_->value(ROPE_SHADER)->setUniformValue("sag",(GLfloat)m_rope.sag);
         shader_programs_->value(ROPE_SHADER)->setUniformValue("eyept",m_engine->camera_.eye.x, m_engine->camera_.eye.y, m_engine->camera_.eye.z);

         //MASTER TRANSFORM
        glPushMatrix();
        glTranslatef(0,0,1.25);
        glRotatef(-m_framesElapsed/40.0,1,0,0);

        //ROPE TRANSFORM
        dh += 0.005;
        if(dh>10)
            dh=0;

        glPushMatrix();
        glTranslatef(-0.5,-dh,0);
        glRotatef(-15,0,0,1);
        glRotatef((float)m_framesElapsed/2.0,0,1,0);
        drawRope(m_rope);
        glTranslatef(0,10,0);
        drawRope(m_rope);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-0.5,-dh,0.5);
        glRotatef(-10,0,0,1);
        glRotatef((float)m_framesElapsed/2.0,0,1,0);
        drawRope(m_rope);
        glTranslatef(0,10,0);
        drawRope(m_rope);
        glPopMatrix();

        float h = 5.0;



        //****** FIRST DISTANT ROPE
        glPushMatrix();
        glScalef(2,1,1);
        glTranslatef(0,h- (m_framesElapsed/50.0),-3);
        glRotatef(15,0,0,1);
        drawRope(*r1,true);
        glPopMatrix();

        h = 7.0;
        //****** SECOND DISTANT ROPES
        glPushMatrix();
        glScalef(2.5,1,1);
        glTranslatef(0,h- (m_framesElapsed/50.0),-1);
        glRotatef(-15,0,0,1);
        drawRope(*r1,true);
        glPopMatrix();
        glPushMatrix();
        glScalef(2.5,1,1);
        glTranslatef(0,h - (m_framesElapsed/50.0),-2);
        glRotatef(-7,0,0,1);
        drawRope(*r1,true);
        glPopMatrix();

        h=8.0;
        //****** THIRD DISTANT ROPES
        glPushMatrix();
        glScalef(2.5,1,1);
        glTranslatef(0,h- (m_framesElapsed/50.0),-2);
        glRotatef(-40,0,0,1);
        drawRope(*r1,true);
        glPopMatrix();
        glPushMatrix();
        glScalef(2.5,1,1);
        glTranslatef(0,h- (m_framesElapsed/50.0),-3);
        glRotatef(12,0,0,1);
        drawRope(*r1,true);
        glPopMatrix();
        glPushMatrix();
        glScalef(2.5,1,1);
        glTranslatef(0,h - (m_framesElapsed/50.0),-2.5);
        glRotatef(-7,0,0,1);
        drawRope(*r1,true);
        glPopMatrix();

        h=12.0;
        //****** THIRD DISTANT ROPES
        glPushMatrix();
        glScalef(2.5,1,1);
        glTranslatef(0,h- (m_framesElapsed/50.0),-2);
        glRotatef(-40,0,0,1);
        drawRope(*r1,true);
        glPopMatrix();
        glPushMatrix();
        glScalef(2.5,1,1);
        glTranslatef(0,h- (m_framesElapsed/50.0),-3);
        glRotatef(12,0,0,1);
        drawRope(*r1,true);
        glPopMatrix();
        glPushMatrix();
        glScalef(2.5,1,1);
        glTranslatef(0,h - (m_framesElapsed/50.0),-2.5);
        glRotatef(-7,0,0,1);
        drawRope(*r1,true);
        glPopMatrix();

        //***** CLOSE ASS ROPE
        h=10.0;
          glPushMatrix();
          glScalef(2.5,1,1);
          glTranslatef(0,h- (m_framesElapsed/100.0),0.25);
          glRotatef(30,0,1,0);
          drawRope(*r1,true);
          glPopMatrix();


        //******** RELEASE ROPE SHADER
        shader_programs_->value(ROPE_SHADER)->release();


        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textures_->value("cube_map_1"));
        shader_programs_->value(NAIL_SHADER)->bind();
        shader_programs_->value(NAIL_SHADER)->setUniformValue("CubeMap",GL_TEXTURE0);
        shader_programs_->value(NAIL_SHADER)->setUniformValue("eyept",m_engine->camera_.eye.x, m_engine->camera_.eye.y, m_engine->camera_.eye.z);
        //******* NAIL BALL
        glPushMatrix();
        glTranslatef(4,-4,-1);

        glRotatef(-55,0,0,1);
        glTranslatef(-(float)m_framesElapsed / 150.0, 0.1 * sin((float)m_framesElapsed/25.0),0);
        glRotatef(m_framesElapsed/3.0,1,0,0);

        glScalef(.1,.1,.1);
        drawNailBall();
        glPopMatrix();

        shader_programs_->value(NAIL_SHADER)->release();

        //MASTER TRANSFORM
        glPopMatrix();

    }


