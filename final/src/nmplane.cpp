#include "nmplane.h"

NMPlane::NMPlane(int xtess, int ytess)
{
    makeGeometry(xtess, ytess);
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
void NMPlane::makeGeometry(int xtess, int ytess)
{

}


