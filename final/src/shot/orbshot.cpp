#include "orbshot.h"
#include "drawengine.h"




OrbShot::OrbShot(DrawEngine* parent,QHash<QString, QGLShaderProgram *>* shad, QHash<QString, GLuint>* tex, QHash<QString, Model>* mod) : Shot(parent,shad,tex,mod)
{
    m_lifespan = 350;

    //make nailvecs and ropes
    numNails = 9;
    numRopes = 12;
    numBrads = 3;

    nlist = new Vector4[numNails];
    rlist = new rope[numRopes];
    blist = new Vector4[numBrads];

    //Vector encoding: (ry, rz, dy, sc)
    nlist[0] = Vector4(15, 45, 1.25, 0.5);
    nlist[1] = Vector4(180, 135, 1.25, 0.5);
    nlist[2] = Vector4(-30, 15, 1.25, 0.5);
    nlist[3] = Vector4(90, 70, 1.15, 0.5);

    nlist[4] = Vector4(0,180,1.25,0.5);
    nlist[5] = Vector4(0,90,1.25,0.4);
    nlist[6] = Vector4(90,90,1.25,0.45);
    nlist[7] = Vector4(180,90,1.25,0.5);
    nlist[8] = Vector4(270,90,1.25,0.55);

    blist[0] = Vector4(45,30,1.25,0.5);
    blist[1] = Vector4(-15, 115, 1.25, 0.5);
    blist[2] = Vector4(-60,35,1.25,0.5);

    rlist[0] = ropeFromNailVecs(nlist[2],nlist[1],.04,0.4);
    rlist[1] = ropeFromNailVecs(nlist[2],nlist[3],.05,5.0);

    rlist[2] = ropeFromNailVecs(nlist[1],nlist[2],.05,0.5);
    rlist[3] = ropeFromNailVecs(nlist[1],nlist[5],.05,0.5);

    rlist[4] = ropeFromNailVecs(nlist[4],nlist[5],.05,0.5);
    rlist[5] = ropeFromNailVecs(nlist[4],nlist[6],.05,0.55);
    rlist[6] = ropeFromNailVecs(nlist[4],nlist[7],.05,0.49);
    rlist[7] = ropeFromNailVecs(nlist[4],nlist[8],.05,0.6);

    rlist[8] = ropeFromNailVecs(nlist[5],nlist[6],.05,0.6);
    rlist[9] = ropeFromNailVecs(nlist[6],nlist[7],.05,0.6);
    rlist[10] = ropeFromNailVecs(nlist[7],nlist[8],.05,0.6);
    rlist[11] = ropeFromNailVecs(nlist[8],nlist[5],.05,0.6);

}

void OrbShot::begin()
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

    m_engine->camera_.eye.z += 1;
}


void OrbShot::update()
{
    m_engine->camera_.eye.z += 1.0 / m_lifespan;

    m_framesElapsed++;
    if(m_framesElapsed >= m_lifespan)
    {  m_engine->endShot();
    }
}


void OrbShot::draw()
{

glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
glEnable(GL_DEPTH_TEST);

glMatrixMode(GL_MODELVIEW);
glLoadIdentity();

glPushMatrix();
glTranslatef(sin(m_framesElapsed*0.01),0,0);
//glRotatef(60,1,0,0);
glRotatef(m_framesElapsed,0,1,0);

//sphere
glPushMatrix();
glScalef(2,2,2);
drawSphere();
glPopMatrix();
/*
glTranslatef(5,0,0);
drawSphere();*/


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
rope myrope = makeRopeLine(Vector4(0.0,0.7,0.0,1),Vector4(0.5,0.5,0,1), .025, 0.5);
//drawRope(myrope, true);



//nails
shader_programs_->value(ROPE_SHADER)->release();
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_CUBE_MAP, textures_->value("cube_map_1"));
shader_programs_->value(NAIL_SHADER)->bind();
shader_programs_->value(NAIL_SHADER)->setUniformValue("CubeMap",GL_TEXTURE0);
shader_programs_->value(NAIL_SHADER)->setUniformValue("eyept",m_engine->camera_.eye.x, m_engine->camera_.eye.y, m_engine->camera_.eye.z);
//*****
//float ry, rz, dy, sc;



for(int i=0; i<numNails; i++)
{
    drawNailFromVec(nlist[i]);
}
for(int i=0; i<numBrads; i++)
{
    drawNailFromVec(blist[i],true);
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

 for(int i =0; i<numRopes; i++)
 {
     drawRope(rlist[i],true);
 }




glPopMatrix();

shader_programs_->value(ROPE_SHADER)->release();
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_CUBE_MAP, textures_->value("cube_map_1"));
shader_programs_->value(NAIL_SHADER)->bind();
shader_programs_->value(NAIL_SHADER)->setUniformValue("CubeMap",GL_TEXTURE0);
shader_programs_->value(NAIL_SHADER)->setUniformValue("eyept",m_engine->camera_.eye.x, m_engine->camera_.eye.y, m_engine->camera_.eye.z);




glPushMatrix();
glTranslatef(2 * sin(m_framesElapsed*.045),-1.5 * cos(m_framesElapsed*.05), 2*sin(m_framesElapsed*.05));
glRotatef(m_framesElapsed,1,0,0);
glPushMatrix();
glScalef(.3,.3,.3);
drawNailBall();
glPopMatrix();
drawSphere();
glPopMatrix();





}

rope OrbShot::ropeFromNailVecs(vec4<REAL> nv1, vec4<REAL> nv2, float rad, float sag)
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
    glRotatef(nv2.x,0,1,0);

 //   glTranslatef(0,nv2.z,0);

    glGetDoublev(GL_MODELVIEW_MATRIX, modelview.data);

    Vector4 pt2 = modelview * Vector4(0,0.75+(nv2.z*nv2.w),0,1);
    glPopMatrix();

pt1= pt1/pt1.w;
pt2 = pt2/pt2.w;

pt1.x = -pt1.x;
pt2.x = -pt2.x;
//cout<<"1: "<<pt1<<" 2: "<<pt2<<endl;
    return makeRopeLine(pt1, pt2, rad, sag);

}

void OrbShot::drawNailFromVec(Vector4 nv, bool usebrad)
{
    //VECTOR ENCODED LIKE SO:  ry rz dy sc
    glPushMatrix();
    glRotatef(nv.x,0,1,0);
    glRotatef(nv.y,0,0,1);
    glTranslatef(0,nv.z,0);
    glScalef(nv.w,nv.w,nv.w);
    if(usebrad)
        glCallList(models_->value(BRAD_MODEL).idx);
    else
        glCallList(models_->value(NAIL_MODEL).idx);
    glPopMatrix();
}
