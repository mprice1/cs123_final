#include "polarshapes.h"
#include "drawengine.h"

PolarShapes::PolarShapes(DrawEngine* parent,QHash<QString, QGLShaderProgram *>* shad, QHash<QString, GLuint>* tex, QHash<QString, Model>* mod) : Shot(parent,shad,tex, mod), m_quadric(NULL)
{
    //lasts 150 frames
    m_lifespan = 150000;
    //m_increment = 0;
    //mFPS = 30;
    m_framecount = 0;
    m_shapes = new QList<Shapes>();
}

PolarShapes::~PolarShapes(){


    if(m_shapes){
        m_shapes->clear();
        delete m_shapes;
        m_shapes = NULL;
    }
    gluDeleteQuadric(m_quadric);
}
extern "C"{
    extern void APIENTRY glActiveTexture(GLenum);
}

void PolarShapes::begin(){
    srand(time(NULL));

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
    m_quadric = gluNewQuadric();


    m_shapes->clear();
    QList<Shapes>* temp = NULL;

    temp = PolarShapes::makeShapes(100,40);
    m_shapes->append(*temp);
    temp->clear();
    delete temp;

    temp = PolarShapes::makeRectShapes(5,5,-50,50,-50,50);
    m_shapes->append(*temp);
    temp->clear();
    delete temp;

}

//called every frame before draw.
void PolarShapes::update(){

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

}

//draw!
void PolarShapes::draw(){
    glMatrixMode(GL_MODELVIEW);

    m_framecount++;


    glPushMatrix();
    glScaled(.1,.1,.1);
    glRotated(90,1,0,0);


    glPushMatrix();
    glTranslated(0,0,10);
//    gluDisk(m_quadric,0,100,4,4);
    gluDisk(m_quadric,0,100,4,4);
    glPopMatrix();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textures_->value("cube_map_1"));
    shader_programs_->value("reflect")->bind();
    shader_programs_->value("reflect")->setUniformValue("CubeMap",GL_TEXTURE0);

    for(int i = 0; i < m_shapes->length(); i++){
        Shapes s = m_shapes->at(i);

        glPushMatrix();
        glRotated(s.r.angle, s.r.x,s.r.y,s.r.z);
        //glTranslated(s.t.x,s.t.y,5 * sin(m_framecount / 20 + s.polt));
        glTranslated(s.t.x,s.t.y,5 * sin(m_framecount / 20 + s.polr / 15));
        //gluCylinder(m_quadric,.25,0,10,10,10);

        glRotated(-90,1,0,0);
        glScaled(5,5,5);
        glCallList(models_->value(BRAD_MODEL).idx);
        glPopMatrix();
    }

    shader_programs_->value(NAIL_SHADER)->release();

    glPopMatrix();
}

QList<Shapes>* PolarShapes::makeShapes(int numshapes, double radius){
    QList<Shapes>* toReturn = new QList<Shapes>();

    srand(time(NULL));

    for (int i = 0; i < numshapes; i++){

        double r1 = (double) rand() / (double) RAND_MAX;
        //r1 *= 2;
        //r1 -= 1;

        double r2 = (double) rand() / (double) RAND_MAX;
        //r2 *= 2;
        //r2 -= 1;

        double r3 = (double) rand() / (double) RAND_MAX;
        r3 *= 2;
        r3 -= 1;

        double r4 = (double) rand() / (double) RAND_MAX;
        r4 *= 2;
        r4 -= 1;

        double r5 = (double) rand() / (double) RAND_MAX;
        r5 *= 2;
        r5 -= 1;

        double r6 = (double) rand() / (double) RAND_MAX;
        r6 *= 2;
        r6 -= 1;

        double rloc = r1 * radius;
        double tloc = r2 * 2 * M_PI;

        Shapes s;
        s.rectx = i;
        s.polr = rloc;
        s.polt = tloc;

        s.t.x = rloc * cos(tloc);
        s.t.y = rloc * sin(tloc);

        s.r.angle = r3 * 15;
        s.r.x = r4;
        s.r.y = r5;
        s.r.z = r6;

        toReturn->append(s);

    }

    return toReturn;
}


QList<Shapes>* PolarShapes::makeRectShapes(int numx, int numy, double xmin, double xmax, double ymin, double ymax){
    QList<Shapes>* toReturn = new QList<Shapes>();

    double xwidth = xmax - xmin;
    double ywidth = ymax - ymin;

    srand(time(NULL));

    for (int x = 0; x < numx; x++){
        for (int y = 0; y < numy; y++){
            double xloc = x;
            xloc = xloc / (double) numx;
            double yloc = y;
            yloc = yloc / (double) numy;

            double r1 = (double) rand() / (double) RAND_MAX;
            r1 *= 2;
            r1 -= 1;

            double r2 = (double) rand() / (double) RAND_MAX;
            r2 *= 2;
            r2 -= 1;

            double r3 = (double) rand() / (double) RAND_MAX;
            r3 *= 2;
            r3 -= 1;

            double r4 = (double) rand() / (double) RAND_MAX;
            r4 *= 2;
            r4 -= 1;

            double r5 = (double) rand() / (double) RAND_MAX;
            r5 *= 2;
            r5 -= 1;

            double r6 = (double) rand() / (double) RAND_MAX;
            r6 *= 2;
            r6 -= 1;

            Shapes s;
            s.rectx = x;
            s.recty = y;

            s.t.x = xmin * (1 - xloc) + xloc * xmax + r1 * xwidth / 10;
            s.t.y = ymin * (1 - yloc) + yloc * ymax + r2 * ywidth / 10;

            s.r.angle = r3 * 15;
            s.r.x = r4;
            s.r.y = r5;
            s.r.z = r6;

            toReturn->append(s);
        }
    }

    return toReturn;
}
