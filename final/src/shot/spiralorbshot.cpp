#include "spiralorbshot.h"
#include "drawengine.h"

spiralOrbShot::spiralOrbShot(DrawEngine* parent,QHash<QString, QGLShaderProgram *>* shad, QHash<QString, GLuint>* tex, QHash<QString, Model>* mod) : Shot(parent,shad,tex,mod)
{
    m_lifespan = 300;

     n =25;
     nlist = new Vector4[n];
     rlist =  new rope[n-1];

    for(int i=0; i<n; i++)
    {
        float ry,rz,dy,sc;
        ry = i * 360.0/((float)n);
        rz = ry/2;
        dy = 0.7;
        sc = .25;

        nlist[i] = Vector4(ry,rz,dy,sc);
        if(i!=0)
        {
            rlist[i-1]= ropeFromNailVecs(nlist[i-1],nlist[i],.05,.3);
        }


    }
}

void spiralOrbShot::begin()
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


void spiralOrbShot::update()
{
    m_framesElapsed++;
    if(m_framesElapsed>= m_lifespan)
    {
        m_engine->endShot();
    }

}


void spiralOrbShot::draw()
{

glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
glEnable(GL_DEPTH_TEST);

glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
glRotatef( ((float)m_framesElapsed+1.0), 0, 1, 0);
//sphere
drawSphere();



/*
 BEFORE ROPES
 */
shader_programs_->value(ROPE_SHADER)->bind();
 glActiveTexture(GL_TEXTURE0);
 glBindTexture(GL_TEXTURE_2D,textures_->value(ROPE_OCC));
 shader_programs_->value(ROPE_SHADER)->setUniformValue("colormap",0);

 glActiveTexture(GL_TEXTURE1);
 glBindTexture(GL_TEXTURE_2D,textures_->value(ROPE_NORM));

 shader_programs_->value(ROPE_SHADER)->setUniformValue("normalmap",1);

 shader_programs_->value(ROPE_SHADER)->setUniformValue("sag",(GLfloat)(0.5 + 0.01*(m_framesElapsed%100)));
 shader_programs_->value(ROPE_SHADER)->setUniformValue("eyept",m_engine->camera_.eye.x, m_engine->camera_.eye.y, m_engine->camera_.eye.z);
//******




//nails
shader_programs_->value(ROPE_SHADER)->release();

glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_CUBE_MAP, textures_->value("cube_map_1"));
shader_programs_->value(NAIL_SHADER)->bind();
shader_programs_->value(NAIL_SHADER)->setUniformValue("CubeMap",GL_TEXTURE0);
shader_programs_->value(NAIL_SHADER)->setUniformValue("eyept",m_engine->camera_.eye.x, m_engine->camera_.eye.y, m_engine->camera_.eye.z);
//*****
//float ry, rz, dy, sc;

Vector4 nail1 = Vector4(    0,  m_framesElapsed%360,  1,    .5);
Vector4 nail2 = Vector4(    45.0, m_framesElapsed%360,    1,    .5);
//drawNailFromVec(nail1);
//drawNailFromVec(nail2);
rope r = ropeFromNailVecs(nail1,nail2,.05,.5);



for(int i=0; i<n; i++)
{
    drawNailFromVec(nlist[i],false);
    drawNailFromVec(Vector4(nlist[i].x-180, nlist[i].y, nlist[i].z, nlist[i].w),true);
}

 shader_programs_->value(ROPE_SHADER)->bind();
 glActiveTexture(GL_TEXTURE0);
 glBindTexture(GL_TEXTURE_2D,textures_->value(ROPE_OCC));
 shader_programs_->value(ROPE_SHADER)->setUniformValue("colormap",0);

 glActiveTexture(GL_TEXTURE1);
 glBindTexture(GL_TEXTURE_2D,textures_->value(ROPE_NORM));

 shader_programs_->value(ROPE_SHADER)->setUniformValue("normalmap",1);

 shader_programs_->value(ROPE_SHADER)->setUniformValue("sag",(GLfloat)(0.5 + 0.01*(m_framesElapsed%100)));
 shader_programs_->value(ROPE_SHADER)->setUniformValue("eyept",m_engine->camera_.eye.x, m_engine->camera_.eye.y, m_engine->camera_.eye.z);

 for(int i = 4; i<n-5; i++)
 {
     drawRope(rlist[i],true);
 }


//.75 is the golden number for nail attatchment?
glPopMatrix();

}

rope spiralOrbShot::ropeFromNailVecs(vec4<REAL> nv1, vec4<REAL> nv2, float rad, float sag)
{
    //VECTOR ENCODED LIKE SO:  ry rz dy sc
    //get world space points
    Matrix4x4 modelview;
    glPushMatrix();
 //   glScalef(nv1.w,nv1.w,nv1.w);
     glRotatef(nv1.y,0,0,1);
    glRotatef(nv1.x,0,1,0);

  //  glTranslatef(0,nv1.z,0);

    glGetDoublev(GL_MODELVIEW_MATRIX, modelview.data);
      Vector4 pt1 = modelview * Vector4(0,0.75+(nv1.z * nv1.w),0,1);
     glPopMatrix();

    glPushMatrix();
//    glScalef(nv2.w,nv2.w,nv2.w);
   glRotatef(nv2.y,0,0,1);
    glRotatef(nv2.x,0,1,0);glTranslatef(0,-0.5,0.5);

 //   glTranslatef(0,nv2.z,0);

    glGetDoublev(GL_MODELVIEW_MATRIX, modelview.data);

    Vector4 pt2 = modelview * Vector4(0,0.75+(nv2.z*nv2.w),0,1);
    glPopMatrix();

//pt1= pt1/pt1.w;
//pt2 = pt2/pt2.w;

pt1.x = -pt1.x;
pt2.x = -pt2.x;
//cout<<"1: "<<pt1<<" 2: "<<pt2<<endl;
    return makeRopeLine(pt1, pt2, rad, sag);

}

void spiralOrbShot::drawNailFromVec(Vector4 nv, bool brad)
{
    //VECTOR ENCODED LIKE SO:  ry rz dy sc
    glPushMatrix();
    glRotatef(nv.x,0,1,0);
    glRotatef(nv.y,0,0,1);
    glTranslatef(0,nv.z,0);
    glScalef(nv.w,nv.w,nv.w);
    if(brad)
        glCallList(models_->value(BRAD_MODEL).idx);
    else
        glCallList(models_->value(NAIL_MODEL).idx);
    glPopMatrix();
}
