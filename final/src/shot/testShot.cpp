#include "testShot.h"
#include "drawengine.h"
using std::cout;
using std::endl;

testShot::testShot(DrawEngine* parent,QHash<QString, QGLShaderProgram *>* shad, QHash<QString, GLuint>* tex, QHash<QString, Model>* mod) : Shot(parent,shad,tex,mod)
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
lightpos[1]=2.f;
lightpos[2]=1.f;
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
  /*
    GLuint filter;						// Which Filter To Use
    GLuint fogMode[]= { GL_EXP, GL_EXP2, GL_LINEAR };	// Storage For Three Types Of Fog
    GLuint fogfilter= 0;					// Which Fog To Use
    GLfloat fogColor[4]= {1.0f, 0.5f, 0.5f, 1.0f};		// Fog Color

    glFogi(GL_FOG_MODE, fogMode[fogfilter]);		// Fog Mode
    glFogfv(GL_FOG_COLOR, fogColor);			// Set Fog Color
    glFogf(GL_FOG_DENSITY, 0.35f);				// How Dense Will The Fog Be
    glHint(GL_FOG_HINT, GL_DONT_CARE);			// Fog Hint Value
    glFogf(GL_FOG_START, 1.0f);				// Fog Start Depth
    glFogf(GL_FOG_END, 5.0f);				// Fog End Depth
    glEnable(GL_FOG);					// Enables GL_FOG

    */

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_MODELVIEW);

   shader_programs_->value(ROPE_SHADER)->bind();
    glBindTexture(GL_TEXTURE_2D,textures_->value(ROPE_OCC));
    glActiveTexture(GL_TEXTURE0);
    shader_programs_->value(ROPE_SHADER)->setUniformValue("colormap",GL_TEXTURE0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,textures_->value(ROPE_NORM));

    shader_programs_->value(ROPE_SHADER)->setUniformValue("normalmap",GL_TEXTURE1);

    shader_programs_->value(ROPE_SHADER)->setUniformValue("sag",(GLfloat)(0.5 + 0.01*(m_framesElapsed%100)));



    /*
     WHAT THE FUCK

     HOW DO I GET BOTH MAPS IN???
     */





   // drawUnitRope();
    Vector4 start = Vector4(0,0,0,1);

    Vector4 end = Vector4(0,0,2,1);
    rope myrope = makeRopeLine(start,end);
    drawRope(myrope);


    end = Vector4(-2,0,0,1);
    rope myrope2 = makeRopeLine(start,end);
    drawRope(myrope2);

    end = Vector4(2,0,0,1);
    rope myrope3 = makeRopeLine(start,end);
    drawRope(myrope3);


 shader_programs_->value(ROPE_SHADER)->release();

 glActiveTexture(GL_TEXTURE0);
 glBindTexture(GL_TEXTURE_CUBE_MAP, textures_->value("cube_map_1"));
 shader_programs_->value("reflect")->bind();
 shader_programs_->value("reflect")->setUniformValue("CubeMap",GL_TEXTURE0);

for(int i=0; i<5; i++)
 {
    for(int j=0;j<5;j++)
    {

        glPushMatrix();
            glTranslatef((float)i,0,(float)j);
            glScalef(.5,.5,.5);
            if((i+j)%3==2)
            {
                glCallList(models_->value(NAIL_MODEL).idx);
            }
            else
            {
                glCallList(models_->value(BRAD_MODEL).idx);
            }
        glPopMatrix();
    }
 }

//make a ball of nails!
glPushMatrix();
glTranslatef(0,1.5,0);

for(int i=0; i<8; i++)
{
    glPushMatrix();
    glRotatef(i*45,0,1.0,0);
    for(int j=0; j<4; j++)
    {
        glPushMatrix();
        glRotatef(45*j,0,0,1);
        glTranslatef(0,1.0,0);
        if((i+j)%3==2)
        {
            glCallList(models_->value(NAIL_MODEL).idx);
        }
        else
        {
            glCallList(models_->value(BRAD_MODEL).idx);
        }
        glPopMatrix();
    }
    glPopMatrix();
}

glPopMatrix();

shader_programs_->value("reflect")->release();


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

