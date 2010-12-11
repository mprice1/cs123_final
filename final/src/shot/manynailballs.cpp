#include "manynailballs.h"
#include "drawengine.h"

/*
struct nailcell
{
    Vector4 pos;
    Vector4 rotvec;
    float rot;
    float scale;
};
*/

/*
    nailcell* cells;
    int numCells;
 */

manyNailBalls::manyNailBalls(DrawEngine* parent,QHash<QString, QGLShaderProgram *>* shad, QHash<QString, GLuint>* tex, QHash<QString, Model>* mod):Shot(parent,shad,tex,mod)
{
    m_fadespan = 50;
    m_lifespan = 350;
    numCells = 25;
    //initialize cells
    cells = new nailcell[numCells];

    srand(time(NULL));

    for(int i =0; i<5;i++)
    {
        for(int j=0;j<5;j++)
        {

        int k = rand() % 50 + 1;    //1 to 50
        cells[5*i + j].pos = Vector4(2*i,0,2*j,1);
        cells[5*i + j].scale = 0.6 + .4 * (k-25.0)/50.0;
        cells[5*i + j].amp = 0.5*k/50.0 + 0.5;
        cells[5*i + j].freq = .05 * k/50.0 + .01;
        }
    }
}

manyNailBalls::~manyNailBalls()
    {
        delete[] cells;
    }


void manyNailBalls::begin()
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

void manyNailBalls::update()
    {
        m_framesElapsed++;
        if(m_framesElapsed>=m_lifespan)
        {
            m_engine->endShot();
        }

        int frames = m_lifespan - m_fadespan;

        if(m_framesElapsed == frames)
        {
            m_engine->fadeShots(m_lifespan - frames);
        }
    }

void manyNailBalls::draw()
    {
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);



    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(1-m_framesElapsed*.01,-1,3 - m_framesElapsed * 0.05);
    glRotatef(m_framesElapsed/2.0,0,0,1);

        //BIND NAIL SHADER AT SOME POINT FOOL
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textures_->value("cube_map_1"));
    shader_programs_->value(NAIL_SHADER)->bind();
    shader_programs_->value(NAIL_SHADER)->setUniformValue("CubeMap",GL_TEXTURE0);
    shader_programs_->value(NAIL_SHADER)->setUniformValue("eyept",m_engine->camera_.eye.x, m_engine->camera_.eye.y, m_engine->camera_.eye.z);
        for(int i=0;i<numCells;i++)
        {
            glPushMatrix();
            glTranslatef(cells[i].pos.x,cells[i].pos.y + cells[i].amp*sin(cells[i].freq*m_framesElapsed),cells[i].pos.z);
            glRotatef(m_framesElapsed,1,0,0);
            //drawSphere();
            glScalef(.3,.3,.3);
            drawNailBall();
            glPopMatrix();
        }

        for(int i=0;i<numCells;i++)
        {
            glPushMatrix();
            glTranslatef(cells[i].pos.x,cells[i].pos.y+ cells[i].amp*sin(cells[i].freq*m_framesElapsed),cells[i].pos.z);
            glRotatef(m_framesElapsed,1,0,0);
            float sc = (cells[i].scale *sin(cells[i].freq*m_framesElapsed + 50*cells[i].freq))*0.5 + 0.6;
            glScalef(sc,sc,sc);
            drawSphere();
            glPopMatrix();
        }
    }
