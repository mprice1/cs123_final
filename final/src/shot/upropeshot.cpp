#include "upropeshot.h"
#include "drawengine.h"





upRopeShot::upRopeShot(DrawEngine* parent,QHash<QString, QGLShaderProgram *>* shad, QHash<QString, GLuint>* tex, QHash<QString, Model>* mod) : Shot(parent,shad,tex,mod)
    {
    m_rope = makeRopeLine(Vector4(0,5,0,1), Vector4(0,-5,0,1),.05,100);
    dh = 0;
    }

    upRopeShot::~upRopeShot()
    {

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


        //ROPE TRANSFORM
        dh += 0.001;
        if(dh>10)
            dh=0;

         glPushMatrix();
        glTranslatef(0,-dh,0);
        glRotatef((float)m_framesElapsed/2.0,0,1,0);
        drawRope(m_rope);
        glTranslatef(0,10,0);
        drawRope(m_rope);



        glPopMatrix();
        //******** RELEASE ROPE SHADER
        shader_programs_->value(ROPE_SHADER)->release();

    }


