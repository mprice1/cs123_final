#include "shot.h"
#include "drawengine.h"

Shot::Shot(DrawEngine* parent,QHash<QString, QGLShaderProgram *>* shad, QHash<QString, GLuint>* tex, QHash<QString, Model>* mod)
{
    m_framesElapsed = 0;
m_engine = parent;
textures_ = tex;
models_ = mod;
shader_programs_ = shad;
}




Shot::~Shot()
{

}

//In begin, initialize things that could not have been initialized beforehand
//(gl state)
void Shot::begin()
{

}

//called every frame before draw.
void Shot::update()
{

}

//draw!
void Shot::draw()
{

}

//draws the unit NMsphere at the origin
void Shot::drawSphere()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,textures_->value(CRACK_COLOR));
    shader_programs_->value(CRACK_SHADER)->setUniformValue("colormap",0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,textures_->value(CRACK_NORM));
    shader_programs_->value(CRACK_SHADER)->setUniformValue("normalmap",1);

    shader_programs_->value(CRACK_SHADER)->bind();

    NMSphere * sph = m_engine->nm_sphere;
    glBegin(GL_TRIANGLES);
    for(int i=0; i<sph->numVerts; i++)
    {

    //set tangent attribute
      //  shader_programs_->value(CRACK_SHADER)->setAttributeValue("intan",(float)sph->tans[i].x,(float)sph->tans[i].y,(float)sph->tans[i].z);
    //set normal
        glNormal3f(sph->norms[i].x , sph->norms[i].y , sph->norms[i].z );
    //set tex coord
        glTexCoord2f(sph->texs[i].x,sph->texs[i].y);

    //set vertex
        glVertex3d(sph->verts[i].x,sph->verts[i].y,sph->verts[i].z);
    }
    glEnd();

    shader_programs_->value(CRACK_SHADER)->release();

}

void Shot::drawNailBall()
{
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
}

void Shot::drawRope(rope myrope, bool useSag)
{
    if(useSag)
    {
        shader_programs_->value(ROPE_SHADER)->setUniformValue("sag",myrope.sag);
    }
    shader_programs_->value(ROPE_SHADER)->setUniformValue("start",myrope.start.x, myrope.start.y, myrope.start.z, myrope.start.w);
    shader_programs_->value(ROPE_SHADER)->setUniformValue("end",myrope.end.x,myrope.end.y,myrope.end.z,myrope.end.w);
    glBegin(GL_QUAD_STRIP);
    for(int i=0; i<myrope.numVerts;i++)
    {
        glTexCoord2f(myrope.texs[i].x, myrope.texs[i].y);
        glNormal3f(myrope.norms[i].x,myrope.norms[i].y,myrope.norms[i].z);
        glVertex4f(myrope.pts[i].x,myrope.pts[i].y,myrope.pts[i].z,myrope.pts[i].w);
    }
    glEnd();
}

rope Shot::makeRopeLine(Vector4 pt1, Vector4 pt2, float radius, float sag)
{
    //Same math as ropeLine()
    float stringRadius = radius;
    Vector4 line = pt2-pt1;
    float distance = (line).getMagnitude();
    int tess = (int)(8*distance);
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
    newRope.sag = sag;
    newRope.numVerts = tess * 18;
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
    REAL low = (i)*h;
    REAL high = (i+1)*h;

    for(int ang=0; ang<=360; ang+=45)
    {
        int offset = 2 * (ang/45);
        double radians =  ((double)ang) * M_PI/ 180.0;

        newRope.texs[index + offset] = vec2<REAL>(((double)ang)/360.0 , 1.0);
        newRope.norms[index + offset] = Vector4(cos(radians), 0.0, sin(radians), 0.0);
        newRope.pts[index + offset] = Vector4(0.5*cos(radians), high, 0.5*sin(radians), 1.0);

        offset++;

        newRope.texs[index + offset ] = vec2<REAL>(((double)ang)/360.0 , 0.0);
        newRope.norms[index + offset ] = Vector4(cos(radians), 0.0, sin(radians), 0.0);
        newRope.pts[index + offset ] = Vector4(0.5*cos(radians), low, 0.5*sin(radians), 1.0);

    }
    index += 18;

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


