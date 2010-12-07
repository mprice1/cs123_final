#ifndef TEMPSHOT_H
#define TEMPSHOT_H

#include <shot.h>
#include <nmplane.h>

class TempShot : public Shot
{
public:
    TempShot(DrawEngine* parent,QHash<QString, QGLShaderProgram *>* shad, QHash<QString, GLuint>* tex, QHash<QString, Model>* mod);

    ~TempShot();

    //In begin, initialize things that could not have been initialized beforehand
    //(gl state)
    void begin();

    //called every frame before draw.
    void update();

    //draw!
    void draw();

    NMPlane* m_plane;
};

#endif // TEMPSHOT_H
