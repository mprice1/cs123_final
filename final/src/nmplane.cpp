#include "nmplane.h"
#include <GL/glu.h>

NMPlane::NMPlane(int xtess, int ytess)
{
    this->xtess = xtess;
    this->ytess = ytess;
    int arraysize = xtess * ytess * 4;
    verts = new Vector4[arraysize];
    norms = new Vector4[arraysize];
    tans = new Vector4[arraysize];
    texs = new vec2<REAL>[arraysize];

    makeGeometry();

}

NMPlane::~NMPlane(){
    if (verts){
        delete[] verts;
        verts = NULL;
    }
    if (norms){
        delete[] norms;
        norms = NULL;
    }
    if (tans){
        delete[] tans;
        tans = NULL;
    }
    if (texs){
        delete[] texs;
        texs = NULL;
    }
}

/*
 Tesselate a unit plane bounded by -0.5, 0.5 in the XZ plane at the origin.
 Texture coordinates in texs
 Vertices in verts
 Normals in norms (all equal (0,1,0,0))
 Tangents in tans (all equal (0,0,-1,0) ... i think  (towards positive V in uv space))
 */

/*
 IF UNSURE ABOUT FORMAT USE NMSphere AS REFERENCE
 */
void NMPlane::makeGeometry()
{
    int index = 0;
    for (int x = 0; x < xtess; x++){
        for (int y = 0; y < ytess; y++){
            Vector4& norm1 = norms[index];
            Vector4& vert1 = verts[index];
            Vector4& tan1 = tans[index];
            vec2<REAL>& tex1 = texs[index];

            Vector4& norm2 = norms[index + 1];
            Vector4& vert2 = verts[index + 1];
            Vector4& tan2 = tans[index + 1];
            vec2<REAL>& tex2 = texs[index + 1];

            Vector4& norm3 = norms[index + 2];
            Vector4& vert3 = verts[index + 2];
            Vector4& tan3 = tans[index + 2];
            vec2<REAL>& tex3 = texs[index + 2];

            Vector4& norm4 = norms[index + 3];
            Vector4& vert4 = verts[index + 3];
            Vector4& tan4 = tans[index + 3];
            vec2<REAL>& tex4 = texs[index + 3];

            tex1 = vec2<REAL>(0,0);
            tex2 = vec2<REAL>(1,0);
            tex3 = vec2<REAL>(1,1);
            tex4 = vec2<REAL>(0,1);

            norm1 = Vector4(0,1,0,0);
            norm2 = Vector4(0,1,0,0);
            norm3 = Vector4(0,1,0,0);
            norm4 = Vector4(0,1,0,0);

            double xfrac = ((double) x) / ((double) xtess);
            double yfrac = ((double) y) / ((double) ytess);
            double xfracplus = ((double) x + 1) / ((double) xtess);
            double yfracplus = ((double) y + 1) / ((double) ytess);

            vert1 = Vector4(-.5 + xfrac,0,-.5 + yfrac,0);
            vert2 = Vector4(-.5 + xfracplus,0,-.5 + yfrac,0);
            vert3 = Vector4(-.5 + xfracplus,0,-.5 + yfracplus,0);
            vert4 = Vector4(-.5 + xfrac,0,-.5 + yfracplus,0);

            tan1 = Vector4(0,0,-1,0);
            tan2 = Vector4(0,0,-1,0);
            tan3 = Vector4(0,0,-1,0);
            tan4 = Vector4(0,0,-1,0);

            index += 4;
        }
    }
}

void NMPlane::draw(){
    glBegin(GL_QUADS);

    int index = 0;
    for (int x = 0; x < xtess; x++){
        for (int y = 0; y < ytess; y++){
            Vector4& norm1 = norms[index];
            Vector4& vert1 = verts[index];
            Vector4& tan1 = tans[index];
            vec2<REAL>& tex1 = texs[index];

            Vector4& norm2 = norms[index + 1];
            Vector4& vert2 = verts[index + 1];
            Vector4& tan2 = tans[index + 1];
            vec2<REAL>& tex2 = texs[index + 1];

            Vector4& norm3 = norms[index + 2];
            Vector4& vert3 = verts[index + 2];
            Vector4& tan3 = tans[index + 2];
            vec2<REAL>& tex3 = texs[index + 2];

            Vector4& norm4 = norms[index + 3];
            Vector4& vert4 = verts[index + 3];
            Vector4& tan4 = tans[index + 3];
            vec2<REAL>& tex4 = texs[index + 3];

            glNormal3f(norm1.x, norm1.y, norm1.z);
            glTexCoord2f(tex1.x,tex1.y);
            glVertex3f(vert1.x, vert1.y, vert1.z);
            glNormal3f(norm2.x, norm2.y, norm2.z);
            glTexCoord2f(tex2.x,tex2.y);
            glVertex3f(vert2.x, vert2.y, vert2.z);
            glNormal3f(norm3.x, norm3.y, norm3.z);
            glTexCoord2f(tex3.x,tex3.y);
            glVertex3f(vert3.x, vert3.y, vert3.z);
            glNormal3f(norm4.x, norm4.y, norm4.z);
            glTexCoord2f(tex4.x,tex4.y);
            glVertex3f(vert4.x, vert4.y, vert4.z);

            index += 4;
        }
    }

    glEnd();
}

