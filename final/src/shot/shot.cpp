#include "shot.h"
#include "drawengine.h"

Shot::Shot(DrawEngine* parent,QHash<QString, QGLShaderProgram *>* shad, QHash<QString, GLuint>* tex)
{
    m_framesElapsed = 0;
m_engine = parent;
textures_ = tex;
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


