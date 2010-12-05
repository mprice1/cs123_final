#include "testShot.h"
#include "drawengine.h"
using std::cout;
using std::endl;

testShot::testShot(DrawEngine* parent,QHash<QString, QGLShaderProgram *>* shad, QHash<QString, GLuint>* tex) : Shot(parent,shad,tex)
{
    //lasts 150 frames
    m_lifespan = 150;
}

testShot::~testShot()
{

}

//In begin, initialize things that could not have been initialized beforehand
//(gl state)
void testShot::begin()
{

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
}

//called every frame before draw.
void testShot::update()
{
    m_framesElapsed++;
    if(m_framesElapsed>m_lifespan)
    {
        //end shot and call engine

        //shader_programs_->value(ROPE_SHADER)->release();
    }
}

//draw!
extern "C"{
    extern void APIENTRY glActiveTexture(GLenum);
}
void testShot::draw()
{
   // glEnable(GL_FOG);

    glMatrixMode(GL_MODELVIEW);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,textures_->value(ROPE_OCC));

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,textures_->value(ROPE_NORM));

   /* cout<<"t0: "<<GL_TEXTURE0<<endl;
    cout<<"occ id: "<<textures_->value(ROPE_OCC)<<endl;
    cout<<"occ off: "<<(textures_->value(ROPE_OCC) + GL_TEXTURE0)<<endl;

    cout<<"norm id: "<<textures_->value(ROPE_NORM)<<endl;*/



    shader_programs_->value(ROPE_SHADER)->setUniformValue("colormap",3);

    shader_programs_->value(ROPE_SHADER)->setUniformValue("normalmap",3);
    shader_programs_->value(ROPE_SHADER)->bind();

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    /*
     WHAT THE FUCK

     HOW DO I GET BOTH MAPS IN???
     */



    glPushMatrix();
    glTranslatef(0,3.0 + ((float)(m_framesElapsed%100))/100.0,-5.0);
    glRotatef(((float)(m_framesElapsed)),0,1.0,0);
    /*for(int i=0; i<2000; i++)
    {
    glTranslatef(0,-1.0,0);
    drawUnitRope();
    }*/
    glPopMatrix();

   // drawUnitRope();
    Vector4 start = Vector4(0,0.5,0,1);
    Vector4 end = Vector4(1,1,0,1);
    Vector4 end2 = Vector4(1,-1,0,1);

    ropeLine(start,end2);
   /* for(int i=0;i<5;i++)
    {
        for(int j=0;j<5;j++)
        {
            end = Vector4((double)i,(double)j,-4.0,1);
            ropeLine(start,end);
        }
    }*/

    rope myrope = makeRopeLine(start,end);
    glBegin(GL_QUAD_STRIP);
    for(int i=0; i<myrope.numVerts;i++)
    {
        glTexCoord2f(myrope.texs[i].x, myrope.texs[i].y);
        glNormal3f(myrope.norms[i].x,myrope.norms[i].y,myrope.norms[i].z);
        glVertex4f(myrope.pts[i].x,myrope.pts[i].y,myrope.pts[i].z,myrope.pts[i].w);
    }
    glEnd();

    /*start = Vector4(0,0,2,1);
    ropeLine(start,end);

    start = Vector4(0,-2,2,1);
    ropeLine(start,end);*/



    shader_programs_->value(ROPE_SHADER)->release();
}

/*
 * Draws a rectangular prism with rounded normals and texture coordinates.
    BELONGS IN A SHOT CLASS
 */
void testShot::drawUnitRope(int tess)
{
   // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    float h = 1.0/((float)tess);
    float m = 0.707106781;

    glBegin(GL_QUAD_STRIP);

    for(int i=0; i<tess;i++)
    {
    glTexCoord2f(0.0,1.0);
    glNormal3f(-m,0.0,m);
    glVertex3f(-0.5,(i+1)*h,0.5);
    glTexCoord2f(0.0,0.0);
    glNormal3f(-m,0.0,m);
    glVertex3f(-0.5,i*h,0.5);

    glTexCoord2f(0.25,1.0);
    glNormal3f(m,0.0,m);
    glVertex3f(0.5,(i+1)*h,0.5);
    glTexCoord2f(0.25,0.0);
    glNormal3f(m,0.0,m);
    glVertex3f(0.5,i*h,0.5);

    glTexCoord2f(0.5,1.0);
    glNormal3f(m,0.0,-m);
    glVertex3f(0.5,(i+1)*h,-0.5);
    glTexCoord2f(0.5,0.0);
    glNormal3f(m,0.0,-m);
    glVertex3f(0.5,i*h,-0.5);

    glTexCoord2f(0.75,1.0);
    glNormal3f(-m,0.0,-m);
    glVertex3f(-0.5,(i+1)*h,-0.5);
    glTexCoord2f(0.75,0.0);
    glNormal3f(-m,0.0,-m);
    glVertex3f(-0.5,i*h,-0.5);

    glTexCoord2f(1.0,1.0);
    glNormal3f(-m,0.0,m);
    glVertex3f(-0.5,(i+1)*h,0.5);
    glTexCoord2f(1.0,0.0);
    glNormal3f(-m,0.0,m);
    glVertex3f(-0.5,i*h,0.5);
    }

    glEnd();
}

/**
 Tesselates and draws a rope line from point start to point end
 **/
void testShot::ropeLine(Vector4 pt1, Vector4 pt2)
{
    float stringRadius = .05;
    Vector4 line = pt2-pt1;
    float distance = (line).getMagnitude();
    int tess = (int)(2*distance);
    //get distance to use as scale factor, tessalation parameter
    //calculate tessellation

    //cross product of normalized is vector to rotate around
    //point to rotate around is origin
    Vector4 rotVec = Vector4(0,1.0,0,0).cross(line.getNormalized());

    //angle is acos( v1 dot v2)
    float angle = acos(Vector4(0,1,0,0).dot(line.getNormalized()));
    angle *= 180;
    angle /= M_PI;

    //translation is pt1

    //now we have the matrix to transform STRING SPACE into WORLD/OBJECT space
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    /**
cout<<"start:"<<pt1<<" end: "<<pt2<<endl;
cout<<"angle: "<<angle<<" rotvec: "<<rotVec<<endl;
cout<<"line: "<<line<<endl;
cout<<"trans: "<<(pt1.x)<<", "<<(pt1.y)<<", "<<(pt1.z)<<endl;
cout<<"tess: "<<tess<<endl;
**/


    glTranslatef(pt1.x,pt1.y,pt1.z);
    glRotatef(angle,rotVec.x,rotVec.y,rotVec.z);
    glScalef(stringRadius, distance, stringRadius);

    drawUnitRope(tess);

    glPopMatrix();

}


rope testShot::makeRopeLine(Vector4 pt1, Vector4 pt2)
{
    //Same math as ropeLine()
    float stringRadius = .05;
    Vector4 line = pt2-pt1;
    float distance = (line).getMagnitude();
    int tess = (int)(2*distance);
    Vector4 rotVec = Vector4(0,1.0,0,0).cross(line.getNormalized());
    float angle = acos(Vector4(0,1,0,0).dot(line.getNormalized()));
    angle *= 180;
    angle /= M_PI;
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(pt1.x,pt1.y,pt1.z);
    glRotatef(angle,rotVec.x,rotVec.y,rotVec.z);
    glScalef(stringRadius, distance, stringRadius);
    //*********************************************
    //  STORE THE MATRIX
    Matrix4x4 modelview;
    Matrix4x4 normalmat;
    glGetDoublev(GL_MODELVIEW_MATRIX,modelview.data);
    glGetDoublev(GL_MODELVIEW_MATRIX,normalmat.data);
    normalmat.invert();
    normalmat = Matrix4x4::transpose(normalmat);
    normalmat.d = normalmat.h = normalmat.l = normalmat.m = normalmat.n = normalmat.o = normalmat.p = 0;
    //*********************************************
    glPopMatrix();
    //*********************************************
    //  TESSELATE THE ROPE
    rope* t = new rope();
    rope newRope = *t;
    newRope.numVerts = tess * 10;
    newRope.start = pt1;
    newRope.end = pt2;
    newRope.pts = new Vector4[newRope.numVerts];
    newRope.norms = new Vector4[newRope.numVerts];
    newRope.texs = new vec2<REAL>[newRope.numVerts];

    int index=0;

    float h = 1.0/((float)tess);
    float m = 0.707106781;

    for(int i=0; i<tess;i++)
    {
    newRope.texs[index] = vec2<REAL>(0.0,1.0);
    newRope.norms[index] = Vector4(-m,0.0,m,0.0);
    newRope.pts[index] = Vector4(-0.5,(i+1)*h,0.5,1.0);

    newRope.texs[index+1] = vec2<REAL>(0.0,0.0);
    newRope.norms[index+1] = Vector4(-m,0.0,m,0.0);
    newRope.pts[index+1] = Vector4(-0.5,i*h,0.5,1.0);

    newRope.texs[index+2] = vec2<REAL>(0.25,1.0);
    newRope.norms[index+2] = Vector4(m,0.0,m,0.0);
    newRope.pts[index+2] = Vector4(0.5,(i+1)*h,0.5,1.0);

    newRope.texs[index+3] = vec2<REAL>(0.25,0.0);
    newRope.norms[index+3] = Vector4(m,0.0,m,0.0);
    newRope.pts[index+3] = Vector4(0.5,i*h,0.5,1.0);

    newRope.texs[index+4] = vec2<REAL>(0.5,1.0);
    newRope.norms[index+4] = Vector4(m,0.0,-m,0.0);
    newRope.pts[index+4] = Vector4(0.5,(i+1)*h,-0.5,1.0);

    newRope.texs[index+5] = vec2<REAL>(0.5,0.0);
    newRope.norms[index+5] = Vector4(m,0.0,-m,0.0);
    newRope.pts[index+5] = Vector4(0.5,i*h,-0.5,1.0);

    newRope.texs[index+6] = vec2<REAL>(0.75,1.0);
    newRope.norms[index+6] = Vector4(-m,0.0,-m,0.0);
    newRope.pts[index+6] = Vector4(-0.5,(i+1)*h,-0.5,1.0);

    newRope.texs[index+7] = vec2<REAL>(0.75,0.0);
    newRope.norms[index+7] = Vector4(-m,0.0,-m,0);
    newRope.pts[index+7] = Vector4(-0.5,i*h,-0.5, 1.0);

    newRope.texs[index+8] = vec2<REAL>(1.0,1.0);
    newRope.norms[index+8] = Vector4(-m,0.0,m,0);
    newRope.pts[index+8] = Vector4(-0.5,(i+1)*h,0.5,1.0);

    newRope.texs[index+9] = vec2<REAL>(1.0,0.0);
    newRope.norms[index+9] = Vector4(-m, 0.0, m, 0);
    newRope.pts[index+9] = Vector4(-0.5, i*h, 0.5, 1.0);

    index += 10;
    }

    //*********************************************
    //  TRANSFORM THE VERTICES AND NORMALS
    for(int i=0; i<newRope.numVerts; i++)
    {
     newRope.pts[i] =   newRope.pts[i] * modelview ;
     newRope.norms[i] = newRope.norms[i] * normalmat;
    }
return newRope;
    //*********************************************
}
