#include "nmsphere.h"

NMSphere::NMSphere(int stacks, int slices)
{
    makeGeometry(stacks, slices);
}

NMSphere::~NMSphere(){
    if (verts){
        delete[] verts;
        verts = NULL;
    }
    if (norms){
        delete[] norms;
        norms = NULL;
    }
    if(tans){
        delete[] tans;
        tans = NULL;
    }
    if (texs){
        delete[] texs;
        texs = NULL;
    }
}

void NMSphere::makeGeometry(int stacks, int slices)
{
    int numFaces = stacks * slices * 2;
    numVerts = 3 * numFaces;
    verts = new Vector4[numVerts];
    norms = new Vector4[numVerts];
    tans = new Vector4[numVerts];
    texs = new vec2<REAL>[numVerts];

    Vector4 north = Vector4(0,0.5,0,0);
    Vector4 toNorth;
    Vector4 bitan;

    int writePtr = 0;

    for(int i=0;i<stacks;i++)
    {
        float stackangleup = (M_PI / 180)*(i*(180.0/stacks));
        float stackangledown = (M_PI / 180)*((i+1)*(180.0/stacks));

        for(int j=0; j<slices;j++)
        {
            float sliceanglelow = (M_PI/180)*(j*(360.0/slices));
            float sliceanglehigh = (M_PI/180)*((j+1)*(360.0/slices));



            float normMag;



            //**************************************************************************************************************
            //Add our two triangles
                //Normal
            normMag = sqrt(
                            (0.5 * sin(stackangledown) * cos(sliceanglelow))*(0.5 * sin(stackangledown) * cos(sliceanglelow))
                            +  (0.5 * cos(stackangledown)) *  (0.5 * cos(stackangledown))
                            + (0.5 * sin(stackangledown) * sin(sliceanglelow))*(0.5 * sin(stackangledown) * sin(sliceanglelow)));


            norms[writePtr].x = (0.5 * sin(stackangledown) * cos(sliceanglelow))/normMag;
            norms[writePtr].y = (0.5 * cos(stackangledown))/normMag;
            norms[writePtr].z = (0.5 * sin(stackangledown) * sin(sliceanglelow))/normMag;
            norms[writePtr].w = 0;



                verts[writePtr].x = 0.5 * sin(stackangledown) * cos(sliceanglelow);
                verts[writePtr].y = 0.5 * cos(stackangledown);
                verts[writePtr].z = 0.5 * sin(stackangledown) * sin(sliceanglelow);
                verts[writePtr].w = 1;

                texs[writePtr].x = sliceanglelow / 2 / M_PI;
                texs[writePtr].y = stackangledown / M_PI;


                toNorth = north - verts[writePtr];
                bitan = norms[writePtr].cross(toNorth);
                tans[writePtr] = norms[writePtr].cross(bitan);
                tans[writePtr].normalize();

          //**************************************************************************************************************

           //NORMAL (new)
                normMag = sqrt(
                                (0.5 * sin(stackangleup) * cos(sliceanglelow))*(0.5 * sin(stackangleup) * cos(sliceanglelow))
                                +  (0.5 * cos(stackangleup)) *  (0.5 * cos(stackangleup))
                                + (0.5 * sin(stackangleup) * sin(sliceanglelow))*(0.5 * sin(stackangleup) * sin(sliceanglelow)));

                writePtr++;
                norms[writePtr].x = (0.5 * sin(stackangleup) * cos(sliceanglelow))/normMag;
                norms[writePtr].y = (0.5 * cos(stackangleup))/normMag;
                norms[writePtr].z = (0.5 * sin(stackangleup) * sin(sliceanglelow))/normMag;
                norms[writePtr].w = 0;



                verts[writePtr].x = 0.5 * sin(stackangleup) * cos(sliceanglelow);
                verts[writePtr].y = 0.5 * cos(stackangleup);
                verts[writePtr].z = 0.5 * sin(stackangleup) * sin(sliceanglelow);
                verts[writePtr].w = 1;

                texs[writePtr].x = sliceanglelow / 2 / M_PI;
                texs[writePtr].y = stackangleup / M_PI;

                toNorth = north - verts[writePtr];
                bitan = norms[writePtr].cross(toNorth);
                tans[writePtr] = norms[writePtr].cross(bitan);
                tans[writePtr].normalize();
          //**************************************************************************************************************

           //NORMAL (new)
                normMag = sqrt(
                                (0.5 * sin(stackangledown) * cos(sliceanglehigh))*(0.5 * sin(stackangledown) * cos(sliceanglehigh))
                                +  (0.5 * cos(stackangledown)) *  (0.5 * cos(stackangledown))
                                + (0.5 * sin(stackangledown) * sin(sliceanglehigh))*(0.5 * sin(stackangledown) * sin(sliceanglehigh)));

                writePtr++;
                norms[writePtr].x = (0.5 * sin(stackangledown) * cos(sliceanglehigh))/normMag;
                norms[writePtr].y = (0.5 * cos(stackangledown))/normMag;
                norms[writePtr].z = (0.5 * sin(stackangledown) * sin(sliceanglehigh))/normMag;
                norms[writePtr].w = 0;


                verts[writePtr].x = 0.5 * sin(stackangledown) * cos(sliceanglehigh);
                verts[writePtr].y = 0.5 * cos(stackangledown);
                verts[writePtr].z = 0.5 * sin(stackangledown) * sin(sliceanglehigh);
                verts[writePtr].w = 1;

                texs[writePtr].x = sliceanglehigh / 2 / M_PI;
                texs[writePtr].y = stackangledown / M_PI;

                toNorth = north - verts[writePtr];
                bitan = norms[writePtr].cross(toNorth);
                tans[writePtr] = norms[writePtr].cross(bitan);
                tans[writePtr].normalize();

                //**************************************************************************************************************

                //normal again (old)
                normMag = sqrt(
                                (0.5 * sin(stackangledown) * cos(sliceanglehigh))*(0.5 * sin(stackangledown) * cos(sliceanglehigh))
                                +  (0.5 * cos(stackangledown)) *  (0.5 * cos(stackangledown))
                                + (0.5 * sin(stackangledown) * sin(sliceanglehigh))*(0.5 * sin(stackangledown) * sin(sliceanglehigh)));

                writePtr++;
                norms[writePtr].x = (0.5 * sin(stackangledown) * cos(sliceanglehigh))/normMag;
                norms[writePtr].y = (0.5 * cos(stackangledown))/normMag;
                norms[writePtr].z = (0.5 * sin(stackangledown) * sin(sliceanglehigh))/normMag;
                norms[writePtr].w = 0;


                verts[writePtr].x = 0.5 * sin(stackangledown) * cos(sliceanglehigh);
                verts[writePtr].y = 0.5 * cos(stackangledown);
                verts[writePtr].z = 0.5 * sin(stackangledown) * sin(sliceanglehigh);
                verts[writePtr].w = 1;

                texs[writePtr].x = sliceanglehigh / 2 / M_PI;
                texs[writePtr].y = stackangledown / M_PI;

                toNorth = north - verts[writePtr];
                bitan = norms[writePtr].cross(toNorth);
                tans[writePtr] = norms[writePtr].cross(bitan);
                tans[writePtr].normalize();
                //**************************************************************************************************************

            //NORMAL (new)
                normMag = sqrt(
                                (0.5 * sin(stackangleup) * cos(sliceanglelow))*(0.5 * sin(stackangleup) * cos(sliceanglelow))
                                +  (0.5 * cos(stackangleup)) *  (0.5 * cos(stackangleup))
                                + (0.5 * sin(stackangleup) * sin(sliceanglelow))*(0.5 * sin(stackangleup) * sin(sliceanglelow)));

                writePtr++;
                norms[writePtr].x = (0.5 * sin(stackangleup) * cos(sliceanglelow))/normMag;
                norms[writePtr].y = (0.5 * cos(stackangleup))/normMag;
                norms[writePtr].z = (0.5 * sin(stackangleup) * sin(sliceanglelow))/normMag;
                norms[writePtr].w = 0;


                verts[writePtr].x = 0.5 * sin(stackangleup) * cos(sliceanglelow);
                verts[writePtr].y = 0.5 * cos(stackangleup);
                verts[writePtr].z = 0.5 * sin(stackangleup) * sin(sliceanglelow);
                verts[writePtr].w = 1;

                texs[writePtr].x = sliceanglelow / 2 / M_PI;
                texs[writePtr].y = stackangleup / M_PI;

                toNorth = north - verts[writePtr];
                bitan = norms[writePtr].cross(toNorth);
                tans[writePtr] = norms[writePtr].cross(bitan);
                tans[writePtr].normalize();
                //**************************************************************************************************************

            //NORMAL (new)
                normMag = sqrt(
                                (0.5 * sin(stackangleup) * cos(sliceanglehigh))*(0.5 * sin(stackangleup) * cos(sliceanglehigh))
                                +  (0.5 * cos(stackangleup)) *  (0.5 * cos(stackangleup))
                                + (0.5 * sin(stackangleup) * sin(sliceanglehigh))*(0.5 * sin(stackangleup) * sin(sliceanglehigh)));

                writePtr++;
                norms[writePtr].x = (0.5 * sin(stackangleup) * cos(sliceanglehigh))/normMag;
                norms[writePtr].y = (0.5 * cos(stackangleup))/normMag;
                norms[writePtr].z = (0.5 * sin(stackangleup) * sin(sliceanglehigh))/normMag;
                norms[writePtr].w = 0;


                verts[writePtr].x = 0.5 * sin(stackangleup) * cos(sliceanglehigh);
                verts[writePtr].y = 0.5 * cos(stackangleup);
                verts[writePtr].z = 0.5 * sin(stackangleup) * sin(sliceanglehigh);
                verts[writePtr].w = 1;

                texs[writePtr].x = sliceanglehigh / 2 / M_PI;
                texs[writePtr].y = stackangleup / M_PI;

                toNorth = north - verts[writePtr];
                bitan = norms[writePtr].cross(toNorth);
                tans[writePtr] = norms[writePtr].cross(bitan);
                tans[writePtr].normalize();

                writePtr++;


        }
    }


}


/*
class NMSphere
{
public:
    NMSphere(int stacks, int slices);
    void makeGeometry(int stacks, int slices);
    void draw;

protected:
    Vector4* verts;
    Vector4* norms;
    Vector4* tans;
    vec2<REAL>* texs;

    int numVerts;
};*/
