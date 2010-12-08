#include "nailsinfield.h"
#include "polarshapes.h"
#include "drawengine.h"

nailSinField::nailSinField(DrawEngine* parent,QHash<QString, QGLShaderProgram *>* shad, QHash<QString, GLuint>* tex, QHash<QString, Model>* mod) : Shot(parent,shad,tex, mod)
{
    m_lifespan = 575;
    m_framecount = 0;
    m_shapes = new QList<Shapes>();
    nm_plane = new NMPlane(30,50);

    myrope = makeRopeLine(Vector4(0,0,0,1),Vector4(0,-10,0,1),.05,0.5);
}

nailSinField::~nailSinField()
{
delete nm_plane;
}

//(gl state)
void nailSinField::begin()
{
    glShadeModel(GL_SMOOTH);
    glClearColor(0.8f,0.8f,0.8f,0.0f);
    glEnable(GL_LIGHTING);
    //Make some lights!
    glEnable(GL_LIGHT0);
    float lightpos[4];
    lightpos[0]=0.f;
    lightpos[1]=0.f;
    lightpos[2]=4.f;
    lightpos[3]=0.f;
    glLightfv(GL_LIGHT0,GL_POSITION,lightpos);


    m_shapes->clear();
    QList<Shapes>* temp = NULL;

    //temp = PolarShapes::makeRectShapesNonRand(80,80,-160,160,-160,160);
    temp = PolarShapes::makeRectShapesNonRand(100,25,-160,160,5,-80);
    m_shapes->append(*temp);
    temp->clear();
    delete temp;

    //CAMERA SETTINGS
    m_engine->camera_.eye.y += 1.0;
    m_engine->camera_.center.y += 0.5;
    m_engine->camera_.center.x -= 0.5;
    //m_engine->camera_.eye.z += 0.5;
    m_engine->perspective_camera();
}

//called every frame before draw.
void nailSinField::update()
{
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    m_framecount++;
    if(m_framecount>=m_lifespan)
    {
        m_engine->endShot();
    }
}

//draw!
void nailSinField::draw()
{



    //CAMERA BEGINS ROTATING PART WAY INTO THE SHOT
    if(m_framecount<400)
    {
        m_engine->camera_.center.x += 0.0025;
        m_engine->perspective_camera();
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // MASTER TRANSLATE
    glPushMatrix();


        glTranslatef(5 - m_framecount/75.0,0,0);




    // GROUND PLANE
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
    nm_plane->draw();
shader_programs_->value(CRACK_SHADER)->release();

    glPopMatrix();

    //********* NAIL SHADER
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textures_->value("cube_map_1"));
    shader_programs_->value(NAIL_SHADER)->bind();
    shader_programs_->value(NAIL_SHADER)->setUniformValue("CubeMap",GL_TEXTURE0);
    shader_programs_->value(NAIL_SHADER)->setUniformValue("eyept",m_engine->camera_.eye.x, m_engine->camera_.eye.y, m_engine->camera_.eye.z);
    //******** NAIL FIELD
    glPushMatrix();
    glTranslatef(0,-1,0);
    glScaled(.1,.1,.1);
    glRotated(90,1,0,0);
    for(int i = 0; i < m_shapes->length(); i++){
        Shapes s = m_shapes->at(i);

        glPushMatrix();
        glTranslated(s.t.x,s.t.y,0);
        glRotated(s.r.angle, s.r.x,s.r.y,s.r.z);
        //glTranslated(s.t.x,s.t.y,5 * sin((float)m_framecount / 20 + s.polt));
        //glTranslated(s.t.x,s.t.y,(40 - s.polr) * sin(m_framecount / 20 - s.polr / 15) / (m_framecount / 20 - s.polr / 15));
        glTranslated(0,0,3.0 * sin((float)m_framecount / 30 - (double) s.rectx / 5 - (double) s.recty / 3));
        glRotated(-90,1,0,0);
        glScaled(5,5,5);
        glCallList(models_->value(BRAD_MODEL).idx);
        glPopMatrix();
    }
    glPopMatrix();
    //**************** END NAIL FIELD NOW IN (master translated) WORLD SPACE AGAIN
    shader_programs_->value(NAIL_SHADER)->release();



    //BIND ROPE SHADER
    shader_programs_->value(ROPE_SHADER)->bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,textures_->value(ROPE_OCC));
    shader_programs_->value(ROPE_SHADER)->setUniformValue("colormap",0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,textures_->value(ROPE_NORM));
    shader_programs_->value(ROPE_SHADER)->setUniformValue("normalmap",1);
    //shader_programs_->value(ROPE_SHADER)->setUniformValue("sag",(GLfloat)(r1.sag));
    shader_programs_->value(ROPE_SHADER)->setUniformValue("eyept",m_engine->camera_.eye.x, m_engine->camera_.eye.y, m_engine->camera_.eye.z);
    //DRAW ROPE
    //ROPE
    glPushMatrix();
        glTranslatef(6,10,-2);
        drawRope(myrope,true);
    glPopMatrix();

    // MASTER TRANSLATE POP
    glPopMatrix();
}

