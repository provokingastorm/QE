// prov_cloudfacetest.cpp

#include "e_config.h"
#if BUILD_FACETEST 
#include "qe.h"
#include "qesl.h"
#include "qebase.h"

#define CLOUD_NAME "cloud"
#define CLOUD_PATH "libs/cloud.lib"
#define CLOUD_FACE_NAME "cb_face"
#define CLOUD "cloud/cloud"
#define CLOUD_FACE "cb_face/cloud"

qefnArcBallRec arcCam;
qeLibScanRec librec;
qeLibChunk chunkrec;

qeLibMat1 *mat;
qeLibGeo3 *geo;
qeLibImg1 *img;

qeLibMat1 *facemat;
qeLibGeo3 *facegeo;
qeLibImg1 *faceimg;

struct FaceTextures
{
    char* name;
    char* path;
}  facePaths[] = {
{"cb_face", "characters/cloud/cb_face.tga"},
{"cb_good", "characters/cloud/cb_good.tga"},
{"cb_evil", "characters/cloud/cb_evil.tga"},
{"cb_rain", "characters/cloud/cb_rain.tga"},
{"cb_thun", "characters/cloud/cb_thun.tga"},
{"cb_light", "characters/cloud/cb_light.tga"},
{"cb_win", "characters/cloud/cb_win.tga"},
{"cb_lose", "characters/cloud/cb_lose.tga"},
};


// Scripting functions
int LoadFace(ptr sf, chr* sp,...);

// table of scripting functions to add
qeslFncRec gameFncTable[] =
{
   {"newface",NUL,LoadFace,0},
   //{"test",NUL,Test,0},
};

int fncCam()
{
    qefnCamArcBall(&arcCam);
    return 0;
}

int RenderCloud()
{
    int r;

    if(!geo)
        return 1;

    if(!facegeo)
        return 1;

	/**********************
	 * DRAW THE CLOUD
	 *********************/
   if(mat)
    {
       glPushMatrix();
       qeGLM12f(mat->mat12);
    }

    r=qeTexBind(geo->imgname);
    glEnable(GL_TEXTURE_2D); 
    glShadeModel(GL_SMOOTH);
   
    qeTexBind(geo->imgname);
    glPolygonMode(GL_FRONT,GL_FILL);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL); // RB416,444
    qeDrawLitUVFaces(geo->verts,geo->normals,geo->faces, geo->uvs,geo->numf);
    glDisable(GL_TEXTURE_2D);
    qeTexBind(NUL);


    if(mat)
       glPopMatrix();

    /**********************
	 * DRAW THE CLOUD FACE
	 *********************/

    if(facemat)
    {
        glPushMatrix();
        qeGLM12f(facemat->mat12);
    }

    r=qeTexBind(facegeo->imgname);
    glEnable(GL_TEXTURE_2D); 
    glShadeModel(GL_SMOOTH);

    if(!(r&M_QETEXBIND_ALPHA))
    {
        // setup to draw texture
        glColor3f(255,1,1);
        glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL); // RB416,444
    }
    else
    {
        // setup texture mode to blend
        glColor3f(0,0,0);
        glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_ADD); // RB416
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); // RB233
    }

    qeDrawLitUVFaces(facegeo->verts,facegeo->normals,
        facegeo->faces, facegeo->uvs,facegeo->numf);
    glDisable(GL_TEXTURE_2D);
    qeTexBind(NUL);

    if(facemat)
        glPopMatrix();

	return 0;
}

void Initialize();

int ScanLibs();

int PrintPaths();

int Process()
{
    Initialize();

    ScanLibs();

    qePrintf("\n");

    PrintPaths();

    return 0;
}

int qeMain(int argc, char *argv[])
{
    int r;
    qeOptionTurnOff(M_QEOPTION_FIRSTCAM|M_QEOPTION_FIRSTLIGHT);

    qeObjAddFncSP(fncCam,"n","f-cam"); 
    qeObjAddFncSP(RenderCloud,"n","cld-rend");
    
    // add the table of new functions
    if((r=qeslAddFncTable(gameFncTable,NUM(gameFncTable)))<0)
       BRK();

    qeObjAddCprSP(&Process,"nI","c-dispatch");

    qeForever();

    return 0;
}

//////////////////////////////
// SUPPORT FUNCTIONS
/////////////////////////////


int ScanLibs()
{
    int r;
    qeLibScanRec *lib=&librec;
    qeLibChunk *chunk=&chunkrec;

    if((r=qeLibNew(CLOUD_NAME, M_QELIBNEW_LOAD, CLOUD_PATH))<0)
       BRK();

    qeLibScanFirst(lib);
   
    qePrintf("Loaded Data:\n");
    
    while(qeLibScanNext(lib, chunk, NUL, NUL)>=0)
    {
        switch(chunk->type)
        {
        case C4TYPE_GEO3:
            if(!szcmp(chunk->name,CLOUD_NAME,NUL))
            {
                qePrintf("Geometry Transform Node: %s\n", chunk->name);
                geo=(qeLibGeo3*)chunk->data;
                break;
            }
            else if(!szcmp(chunk->name,CLOUD_FACE_NAME,NUL))
            {
                qePrintf("Geometry Transform Node: %s\n", chunk->name);
                facegeo=(qeLibGeo3*)chunk->data;
                break;
            }
        case C4TYPE_MAT1:
            if(!szcmp(chunk->name,CLOUD_NAME,NUL))
            {
                qePrintf("Matrix Transform Node: %s\n", chunk->name);
                mat=(qeLibMat1*)chunk->data;
                break;
            }
            else if(!szcmp(chunk->name,CLOUD_FACE_NAME,NUL))
            {
                qePrintf("Matrix Transform Node: %s\n", chunk->name);
                facemat=(qeLibMat1*)chunk->data;
                break;
            }
        case C4TYPE_IMG1:
            if(!szcmp(chunk->name,CLOUD_NAME,NUL))
            {
                qePrintf("Image Transform Node: %s\n", chunk->name);
                if(!(img=(qeLibImg1*)chunk->data))
                    break;
                // create image
                // p=pixel-pointer b=bytesPerPixel w=width,height f=glFmt
                if((r=qeImgNewSP(img->name,"pbwf",img->pix,img->bytesPerPixel,
                              img->width,img->height,img->glFmt))<0)
                    BRK();
                break;
            }
            else if(!szcmp(chunk->name,CLOUD_FACE_NAME,NUL))
            {
                qePrintf("Image Transform Node: %s\n", chunk->name);
                if(!(faceimg=(qeLibImg1*)chunk->data))
                    break;
                // create image
                // p=pixel-pointer b=bytesPerPixel w=width,height f=glFmt
                if((r=qeImgNewSP(faceimg->name,"pbwf",faceimg->pix,faceimg->bytesPerPixel,
                              faceimg->width,faceimg->height,faceimg->glFmt))<0)
                    BRK();
                break;
            }
        default:
            break;
        } // end switch statement that checks chunk type
    } // while loop that scans libs
    
    return 0;
}

void PrintLoadedData()
{
    qePrintf("Loaded Data:\n");

}

int PrintPaths()
{
    qePrintf("Face Texture Choices:\n");
    for(int counter = 0; counter < NUM(facePaths); counter++)
        qePrintf("%s %d\n", facePaths[counter].name, counter);
    qePrintf("\n");
    return 0;
}

int LoadFace(ptr sf, chr* strparam,...)
{
    chr type; // type: s=string, f=float, i=int
    vargs arguments;
    int i,r;

    varg_start(arguments, strparam);
    while(strparam && (type=*strparam++)) switch(type)
    {
    case 'i':
        i = varg_next(arguments,int);
        if(i >=0 && i < 8)
        {
            if((r=qeImgNew(facePaths[i].name,0,facePaths[i].path)<0))
            {BRK();qePrintf("That Image does not exist\n");break;}

            faceimg->name = facePaths[i].name;
            facegeo->imgname = faceimg->name;
            faceimg->path = facePaths[i].path;
            qePrintf("Current Face: %s\n", facegeo->imgname);
        }
        break;
    default:
        break;
    } // while switch

    return 0;
}


void Initialize()
{
   MEMZ(arcCam); 
   mat=NUL;
   geo=NUL;
   img=NUL;
   facegeo=NUL;
   facemat=NUL;
   faceimg=NUL;
}

#endif