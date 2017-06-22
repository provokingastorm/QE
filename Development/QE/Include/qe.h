// Copyright (C) 2006-2007 Joe Linhoff, All Rights Reserved
// qe.h
// this is the API to the engine
#ifndef QE_H
#define QE_H

#define QE_H_VERS 0x00000018

///////////////////////////////////////////////////////////////////////////////
// INCLUDES
//
// This section includes other header files that are required.
// Generally, it is not good practice to embed include files.
// However, they are included here to simplify using the engine.

#ifndef QEGLINCLUES_H
#define QEGLINCLUES_H 1
#if BUILD_MSVC
#include <windows.h> // included b/c GL needs it.. bummer
#endif // BUILD_MSVC
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif // ndef QEGLINCLUES_H

// C in C++ wrapper
#ifdef __cplusplus
extern "C" {
#endif

#ifndef QEBASE_H
#include "qebase.h"
#endif // ndef QEBASE_H

#ifndef QEFN_H
#include "qefn.h"
#endif // ndef QEFN_H

#ifndef QE_API
#ifdef QE_EXPORTS
#define QE_API __declspec(dllexport)
#else // ndef QE_EXPORTS
#define QE_API __declspec(dllimport)
#endif // ndef QE_EXPORTS
#endif // ndef QE_API

///////////////////////////////////////////////////////////////////////////////
// STRUCTURE DEFINITIONS

// non-numbered types for game created types
#define C4TYPE_MAT  C4('m','a','t','_')
#define C4TYPE_ERR  C4('e','r','r','_')
#define C4TYPE_ANI  C4('a','n','i','_')
#define C4TYPE_SKL  C4('s','k','l','_')
#define C4TYPE_SKN  C4('s','k','n','_')
#define C4TYPE_CAM  C4('c','a','m','_')
#define C4TYPE_LIT  C4('l','i','t','_')
#define C4TYPE_NODE C4('n','o','d','e')
#define C4TYPE_MARK C4('m','a','r','k')
// numbered types for lib types
#define C4TYPE_GEO1 C4('g','e','o','1')
#define C4TYPE_GEO2 C4('g','e','o','2')
#define C4TYPE_GEO3 C4('g','e','o','3')
#define C4TYPE_MAT1 C4('m','a','t','1')
#define C4TYPE_IMG1 C4('i','m','g','1')
#define C4TYPE_ANI1 C4('a','n','i','1')
#define C4TYPE_ANI2 C4('a','n','i','2')
#define C4TYPE_CAM1 C4('c','a','m','1')
#define C4TYPE_SKL1 C4('s','k','l','1')
#define C4TYPE_SKN1 C4('s','k','n','1')
#define C4TYPE_TAG1 C4('t','a','g','1')
#define C4TYPE_POS1 C4('p','o','s','1')
#define C4TYPE_MTR1 C4('m','t','r','1')
#define C4TYPE_LIT1 C4('l','i','t','1')

// light properties
typedef struct {
   float posdir[4]; // position or direction
   float ambient[4]; // color of ambient light
   float diffuse[4]; // colorof light hitting
   float specular[4]; // color reflected light
} qeLight1;

// material properties
typedef struct {
   float ambient[4]; // color percent of ambient light is given off
   float diffuse[4]; // color percent of light hitting the surface
   float specular[4]; // color percent of reflected light
   float shininess; // shininess exponent
} qeMaterial1;

// qeGeo1
// geometry definition 1
typedef struct {
   int numv; // number of vertices
   int numf; // number of faces
   float *verts; // pointer to verts
   int *faces; // pointer to faces
} qeGeo1;

// qeLibGeo2
// geometry definition 2
typedef struct {
   int numv; // number of vertices
   int numf; // number of faces
   float *verts; // pointer to verts
   float *vNormals; // pointer to vertex normals
   int *faces; // pointer to faces
   float *fUVs; // pointer to face texture coords
} qeLibGeo2;

#define M_QELIBGEO3_NOTVISIBLE 0x0001
#define M_QELIBGEO3_DONTLIGHT  0x0002
typedef struct {
   uns32 libType; // C4
   uns16 libFlags; // library flags
   uns16 libSize; // size of this record
   int32 numv; // number of verts
   int32 numf; // number of faces
   float *verts; // pointer to verts
   float *normals; // pointer to normals
   int   *faces; // pointer to face data
   float *uvs; // pointer to uv data
   chr   *imgname; // image name
   flt32 xyzrCull[4]; // culling xyzr
   chr* note; // note field
} qeLibGeo3;

#define M_QELIBMAT1_DONTSTART   0x1000
#define M_QELIBMAT1_TRANSFORMED 0x2000
typedef struct {
   uns32 libType;
   uns16 libFlags;
   uns16 libSize;
   chr   *name;
   flt32 trans[3];
   flt32 rot[3]; // radians
   flt32 scale[3];
   flt32 mat12[12];
} qeLibMat1;

typedef struct {
   uns32 libType;
   uns16 libFlags;
   uns16 libSize;
   chr   *name;
   chr   *path;
   uns8  flags;
   uns8  bytesPerPixel;
   uns16 texFlags;
   uns32 width;
   uns32 height;
   uns32 rowBytes;
   uns32 glFmt;
   uns32 glBind;
   uns8  *pix;
} qeLibImg1;

typedef struct {
   uns32 libType;
   uns16 libFlags;
   uns16 libSize;
   chr   *name;
   chr   *copyofName;
   flt32 trans[3];
   flt32 rot[3];
   flt32 scale[3];
} qeLibPos1;

typedef struct {
   float diffuse;
   float transparency[3];
   float ambientColor[3];
   float incandescence[3];
} qeMtrPhong;

#define QELIBMTRTYPE_PHONG C4('p','h','n','g')
typedef struct {
   uns32 libType;
   uns16 libFlags;
   uns16 libSize;
   off32 name;
   uns32 mtrType;
   union {
      qeMtrPhong phong;
   } u;
} qeLibMtr1;

typedef struct {
   flt32 ambientShade;
} qeLibLitAmb;

typedef struct {
   flt32 rot[3];
} qeLibLitDir;

typedef struct {
   flt32 trans[3];
   chr decay; // '0'=none 'L'=linear 'Q'=quadratic 'C'=cubic
   chr xx1[3]; // reserved
} qeLibLitPnt;

typedef struct {
   flt32 rot[3];
   flt32 trans[3];
   chr decay; // '0'=none 'L'=linear 'Q'=quadratic 'C'=cubic
   chr xx1[3]; // reserved
   flt32 coneAng;
   flt32 penumbraAng;
   flt32 dropoff;
} qeLibLitSpt;

#define M_QELIBLIT1_DONTSTART 0x0001
#define M_QELIBLIT1_AMBIENT   0x0010
#define M_QELIBLIT1_DIFFUSE   0x0020
#define M_QELIBLIT1_SPECULAR  0x0040
typedef struct {
   uns32 libType;
   uns16 libFlags;
   uns16 libSize;
   chr*  name;
   chr type; // 'a'=ambient 'd'=directional 'p'=point 's'=spot
   chr xx1[3];
   flt32 color4[4];
   flt32 intensity;
   union {
      qeLibLitAmb amb;
      qeLibLitDir dir;
      qeLibLitPnt pnt;
      qeLibLitSpt spt;
   } u;
} qeLibLit1;

#define M_QELIBKEY_TRS 0x0007
#define M_QELIBKEY_V   0x8000
#define M_QELIBKEY_TX  0x0010
#define M_QELIBKEY_TY  0x0020
#define M_QELIBKEY_TZ  0x0040
#define M_QELIBKEY_T   (M_QELIBKEY_TX|M_QELIBKEY_TY|M_QELIBKEY_TZ)
#define M_QELIBKEY_RX  0x0100
#define M_QELIBKEY_RY  0x0200
#define M_QELIBKEY_RZ  0x0400
#define M_QELIBKEY_R   (M_QELIBKEY_RX|M_QELIBKEY_RY|M_QELIBKEY_RZ)
#define M_QELIBKEY_SX  0x1000
#define M_QELIBKEY_SY  0x2000
#define M_QELIBKEY_SZ  0x4000
#define M_QELIBKEY_S   (M_QELIBKEY_SX|M_QELIBKEY_SY|M_QELIBKEY_SZ)
typedef struct {
   flt32 time;
} qeLibKeyHead;

typedef struct {
   flt32 a[3];
} qeLibKeyFA;

typedef struct {
   flt32 a[3];
   flt32 b[3];
} qeLibKeyFB;

typedef struct {
   flt32 a[3];
   flt32 b[3];
   flt32 c[3];
} qeLibKeyFC;

#define M_QELIBANI1_LOOP     0x0001 // loop the animation
#define M_QELIBANI1_DONTLOOP 0x0002 // dont loop the animation
typedef struct {
   uns32 libType;
   uns16 libFlags;
   uns16 libSize;
   chr   *name;
   chr   *jname;
   uns32 numk;
   uns16 keyFlags;
   uns16 xx1;
   qeLibKeyHead *keys;
   qeLibKeyHead *keyx;
} qeLibAni1;

typedef struct {
   uns32 libType;
   uns16 libFlags;
   uns16 libSize;
   chr *name;
   uns32 num;
   chr **animNames;
} qeLibAni2;

#define M_QELIBCAM1_ORTHO     0x0001
#define M_QELIBCAM1_DONTSTART 0x0002
typedef struct {
   uns32 libType;
   uns16 libFlags;
   uns16 libSize;
   chr*  name;
   flt32 orthoWidth;
   flt32 clipNear;
   flt32 clipFar;
   flt32 fovy;
   flt32 hdivw;
   flt32 centerOfInterest;
   flt32 trans[3];
   flt32 rot[3];
} qeLibCam1;

typedef struct {
   chr* name;
   uns16 parentNum;
   uns16 xx1;
   uns32 parentM12Off;
   flt32 m12Bind[12];
   flt32 orient[3];
   flt32 trans[3];
   flt32 rot[3];
   flt32 scale[3];
} qeLibJoint1;

typedef struct {
   uns32 libType;
   uns16 libFlags;
   uns16 libSize;
   chr* name;
   uns32 numj;
   qeLibJoint1 *joints;
   chr* notes;
} qeLibSkl1;

typedef struct {
   chr *name; // joint name -- match with skel
   uns16 matIndex; // skeleton matrix index
   uns16 xx1;
   flt32 m12BindPre[12];
} qeLibBind1;

typedef struct {
   uns32 libType;
   uns16 libFlags;
   uns16 libSize;
   chr* name;
   uns32 numb; // number of binds
   uns32 numv; // number of verts
   qeLibBind1 *binds;
   flt32 *weights;
} qeLibSkn1;

typedef struct {
   uns32 libType;
   uns16 libFlags;
   uns16 libSize;
   chr* name;
   uns32 type;
   chr* tags;
} qeLibTag1;

#define M_QEINSTVISIT_POPVISIT 0x0001
typedef struct {
   uns32 type;
   int (*visit)(ptr inst,uns flags);
} qeInstVisitRec;

///////////////////////////////////////////////////////////////////////////////
// FUNCTIONS

//
// INITIALIZATION, FINALIZATION, CONTROL
//

// extern for user's main function
extern int qeMain(int argc,chr *argv[]);

// open & close the main window
QE_API int qeOpen(void);
QE_API void qeClose(void);
QE_API int qeObjAddInitial(uns op); // setup engine object list

// two ways to run the engine
QE_API int qeForever(void); // does not return until closed
QE_API int qeOneLoop(void); // executes one engine loop & returns

// other
QE_API int qeAbort(int code); // signal engine to abort
QE_API int qePauseOnExit(void);
QE_API int qeWindowTitle(chr *name); // window title
QE_API int qeWindowSize(int width,int height); // window size
QE_API int qeFullscreen(int op); // 1=full 0=window 2=flip -1=cur
QE_API int qeGetWindowSize(float *w,float *h);
QE_API chr *qeVersion(void); // get version
QE_API uns qeHVers(void); // get QE_H_VERS lib was built with
QE_API int qeSwapBuffers(void); // swap screen buffers
QE_API int qeConsRGB(float r,float g,float b);
QE_API int qeConsClear(void);
QE_API int qeFontPrintf(chr *fmt,...);
QE_API int qeFontHeight(void);
QE_API int qeFontStringWidth(chr *str,chr *strx);
QE_API void qeFontPos(float x,float y);
QE_API void qeFontRGB(float r,float g,float b);

//
// OBJECTS
//

enum {
	QECPROP_RUN,
	QECPROP_INIT,
	QECPROP_FINAL
};
#define M_QECPROP 0xff

enum {
	QESYSFNCOP_NONE,
	QESYSFNCOP_INIT,
	QESYSFNCOP_FINAL,
};
#define M_QESYSFNCOP 0xff

// qeObjScan
// scanning record
typedef struct {
   qeObj *o;
   uns32 priv[32];
   chr *buf;
   uns bufSize;
} qeObjScan;

// engine objects
QE_API qeObj* qeObjAddFnc(fnc_iv fnc); // add C function obj
QE_API qeObj* qeObjAddFncSP(ptr adr,chr *sp,...); // add C func w/sp
QE_API qeObj* qeObjAddCFnc(ptr classptr,...); // add C++ function obj
QE_API qeObj* qeObjAddCFncSP(ptr classptr,...); // add C++ func obj w/sp
QE_API qeObj* qeObjAddCpr(fnc_ii fnc); // add co-process routine obj
QE_API qeObj* qeObjAddCprSP(ptr fnc,chr *sp,...); // add cpr obj w/sp
QE_API qeObj* qeObjAddInst(ptr adr,uns type); // add instance
QE_API qeObj* qeObjAddInstSP(ptr adr,uns type,chr *sp,...); // add inst w/sp
QE_API int qeObjRename(qeObj *o,chr *name,chr *namex);
QE_API qeObj *qeObjFindName(chr *name,chr *namex);
QE_API int qeObjRemove(qeObj *o);
#define M_QEOBJSETLINK_BEFORE 0x01
#define M_QEOBJSETLINK_AFTER  0x02
QE_API void qeObjSetLink(qeObj *o,uns op);
QE_API qeObj *qeObjScanNext(qeObjScan *scn,
   chr *nameBuf,uns nameBufSize,int *depthp);
QE_API qeObj *qeObjScanN(qeObjScan *scn);
QE_API qeObj *qeObjFirst(void);
QE_API qeObj *qeObjListHead(void);
QE_API qeObj* qeObjFindPath(ptr scanrec,qeObj *o,chr *path,chr *pathx);

//
// INST
//

QE_API int qeInstLoad(ptr adr,uns type);
QE_API int qeInstStartSP(ptr adr,uns type,chr *sp,...);
#define qeInstStart(_a_,_t_) qeInstStartSP(_a_,_t_,NUL)
QE_API int qeInstVisitTableSet(qeInstVisitRec *vr);

//
// CPR-SCRIPTING
//

QE_API int qeCprSleep(float t); // sleep in seconds
QE_API int qeCprSloop(int t); // sleep in loops

//
// DRAW
//

QE_API int qeDrawFaces(float *verts,int *faces,int numf);
QE_API int qeDrawUVFaces(float *verts,int *faces,float *uvs,int numf);
QE_API int qeDrawScaledFaces(float *scalexyz,float *verts,
   int *faces,int numf);
QE_API int qeDrawScaledUVFaces(float *scalexyz,float *verts,
   int *faces,float *uvs,int numf);
QE_API int qeDrawLitFaces(float *verts,float *vnormals,
   int *faces,int numf);
QE_API int qeDrawLitUVFaces(float *verts,float *vnormals,
   int *faces,float *uvs,int numf);
QE_API int qeDrawScaledLitFaces(float *scalexyz,float *verts,
   float *vnormals,int *faces,int numf);
QE_API int qeDrawScaledLitUVFaces(float *scalexyz,float *verts,
   float *vnormals,int *faces,float *uvs,int numf);
QE_API int qeDrawNormals(float *verts,int *faces,int numf);
QE_API int qeDrawVertVecs(float *verts,int numv,float *vecs,float mul);
QE_API int qeDrawLitUVTangentFaces(int tangentloc,float *verts,
   float *vnormals,float *vtangents,int *faces,float *uvs,int numf);

QE_API int qeBuildSmoothVertNormals(float *vnormals,float *verts,int numv,
   int *faces,int numf);
QE_API int qeBuildTangents(float *vtangents,float *verts,int numv,
   int *faces,float *uvs,int numf);
QE_API int qeOrthonormalize(float *vdst,float *vsrc,float *vnormals,int numv);

//
// CAMERA
// * all qeCam* functions are defunct and will go away soon
// * use qeGL* or qeM12* or qeM9* commands instead
// * the whole idea of a camera is being removed from the engine
//

enum {
   QECAM_XYZROT,
   QECAM_M12,
};

// qeCam
typedef struct {
   // -- depricated -- don't use
   uns8 type;
   uns8 flags;
   uns16 xx1;
   float xyz[3]; // world coordinate: x, y, z
   float rot[3]; // in radians: xrot, yrot, zrot
   float m12[12]; // matrix for camera
} qeCam;

// -- defunct, don't use these --
#define QECAM_CUR ((qeCam*)NUL)
QE_API qeCam* qeCamGet(chr *name,chr *namex);
QE_API int qeCamFly(qeCam *cam,float fw,float up,float rt,
   float xa,float ya);
QE_API int qeCamLookAt(qeCam *cam,float eyex,float eyey,float eyez,
   float centerx,float centery,float centerz,float upx,float upy,float upz);
QE_API int qeCamGetPDUR(qeCam *cam,
   float *pos,float *dir,float *up,float *right);
QE_API int qeCamGetFacingM12f(qeCam *cam,float *mat12);
QE_API int qeCamGetFrontM12f(qeCam *cam,float *mat12);
#define qeCamLookAtM12f qeM12LookAt

// -- use these --
QE_API void qeCamM12f(float *m12,float *rot,float *xyz);
QE_API int qeM12LookAt(float *m12,float *xyzEye,float *xyzCenter,float *xyzUp);

//
// OPTIONS
//

// options for qeOption*()
#define M_QEOPTION_CLEARSCREEN  0x000001
#define M_QEOPTION_FIRSTCAM     0x000002
#define M_QEOPTION_MOUSECAM     0x000004
#define M_QEOPTION_DRAWCONSOLE  0x000100
#define M_QEOPTION_INITGL       0x000200
#define M_QEOPTION_DRAWMODE     0x000400
#define M_QEOPTION_FIRSTLIGHT   0x000800
#define M_QEOPTION_FULLSCREEN   0x001000
#define M_QEOPTION_LOGPRINTF    0x002000
#define M_QEOPTION_CHECKLOGSIZE 0x004000
#define M_QEOPTION_INERNALSLEEP 0x008000

// Option control
QE_API int qeOptionTurnOff(uns flags);
QE_API int qeOptionTurnOn(uns flags);
QE_API uns qeOptionCurrent(void);

// Engine flags
#define M_QEDRAWFLAG_WIREFRAME 0x0001
#define M_QEDRAWFLAG_JOINTS    0x0002
#define M_QEDRAWFLAG_NORMALS   0x0004
QE_API uns qeDrawFlags(void);

enum { // qeAbort() codes
   QEABORT_USERQUIT=1,
}; // qeAbort() codes

//
// INPUT
//

enum { // button definitions
   QEINPBUTTON_NONE,
   QEINPBUTTON_UP,
   QEINPBUTTON_DOWN,
   QEINPBUTTON_LEFT,
   QEINPBUTTON_RIGHT,
   QEINPBUTTON_MOUSELB,
   QEINPBUTTON_MOUSERB,
   QEINPBUTTON_MOUSECB,
   QEINPBUTTON_UP2,
   QEINPBUTTON_DOWN2,
   QEINPBUTTON_A,
   QEINPBUTTON_B,
   QEINPBUTTON_X,
   QEINPBUTTON_Y,
   QEINPBUTTON_START,
   QEINPBUTTON_DRAWMODE,
   QEINPBUTTON_FIRSTGAME=32, // first game defined button 
   QEINPBUTTON_NUM=64 // number of buttons in this list -- not a button
}; // button definitions

#define M_QEINPSTATE_ALT   0x0001
#define M_QEINPSTATE_SHIFT 0x0002
#define M_QEINPSTATE_CTL   0x0004

#define QEINP_JOYAXIS_MAX 32767

QE_API uns qeInpButton(uns inpb);
QE_API uns qeInpMouseXY(int *xy);
QE_API int qeInpMouseSetXY(int *xy);
QE_API uns qeInpState(void);
QE_API int qeInpMouseWheel(void);
QE_API int qeInpKeyBuf(uns *keyBuf,uns keyBufSize);

enum {
   QEINPOP_NONE,
   QEINPOP_GETNUM, // -- get number of entries in keyBuf
   QEINPOP_CHECKOVERFLOW, // -- return overflow flag
   QEINPOP_CPYCHRBUF, // chr *dst,uns dstSize -- copy out
   QEINPOP_RESET, // -- reset keyBuf
   QEINPOP_KEYBOARDMAPPINGS_OFF, // -- turn keyboard mappings off
   QEINPOP_KEYBOARDMAPPINGS_ON, //-- turn keyboard mappings on

   QEINPOP_JOYGETNUM_HATS, // uns joynum -- return number of hats for joynum
   QEINPOP_CURSOFF, // turn cursor off
   QEINPOP_CURSON, // turn cursor on
   QEINPOP_SETKEYMAP, // int *mapTable -- QEINPBUTTON_,key,0 .. 0
   QEINPOP_DEFAULTKEYMAP, // -- reset to default mappings
};

#define M_INPJOYHAT_UP 0x01 // dont use this
#define M_INPJOYHAT_RIGHT 0x02  // dont use this
#define M_INPJOYHAT_DOWN 0x04 // dont use this
#define M_INPJOYHAT_LEFT 0x08 // dont use this

#define M_QEINPJOYHAT_UP 0x01
#define M_QEINPJOYHAT_RIGHT 0x02
#define M_QEINPJOYHAT_DOWN 0x04
#define M_QEINPJOYHAT_LEFT 0x08

#define QEJOYAXIS_LEFT_X 0
#define QEJOYAXIS_LEFT_Y 1
#define QEJOYAXIS_RIGHT_X 2
#define QEJOYAXIS_RIGHT_Y 3
#define QEJOYAXIS_LEFT_TRIGGER 4
#define QEJOYAXIS_RIGHT_TRIGGER 5

#define QEJOYBUTTON_A 0
#define QEJOYBUTTON_B 1
#define QEJOYBUTTON_X 2
#define QEJOYBUTTON_Y 3
#define QEJOYBUTTON_LSHOULDER 4
#define QEJOYBUTTON_RSHOULDER 5
#define QEJOYBUTTON_DPAD_LEFT 6
#define QEJOYBUTTON_DPAD_RIGHT 7
#define QEJOYBUTTON_DPAD_UP 8
#define QEJOYBUTTON_DPAD_DOWN 9
#define QEJOYBUTTON_START 10
#define QEJOYBUTTON_BACK 11
#define QEJOYBUTTON_LTHUMB 12
#define QEJOYBUTTON_RTHUMB 13

QE_API int qeInpOp(uns op,...);

QE_API uns qeInpJoyNum(void);
QE_API int qeInpJoyInfo(uns num,uns *axisp,uns *buttonp);
QE_API int qeInpJoyAxis(uns num,uns axis);
QE_API uns qeInpJoyButton(uns num,uns button);
QE_API uns qeInpJoyHat(uns num,uns hat); // depricated -- don't use

//
// LIB
//

#define qeLibScanRec qeObjScan

typedef struct {
   chr *name;
   uns32 type;
   ptr data;
   qeObj* libObj;
} qeLibChunk;

#define M_QELIBNEW_LOAD        0x0001
#define M_QELIBNEW_START       0x0002
#define M_QELIBNEW_STARTIMAGES 0x0004
QE_API int qeLibNew(chr *name,uns flags,chr *path);
QE_API int qeLibDelete(chr *name);
QE_API int qeLibLoad(chr *name);
QE_API int qeLibUnload(chr *name);
#define qeLibStart(_name_) qeLibStartSP(_name_,NUL)
QE_API int qeLibStartSP(chr *name,chr *sp,...);
// qeLibScanFirst -- doesn't return a value anymore
#define qeLibScanFirst(_lsr_) MEMZ(*(_lsr_))
QE_API int qeLibScanNext(qeObjScan *sr,qeLibChunk *lib,chr *name,chr *namex);
QE_API chr *qeLibScanLibName(qeObj *o);
QE_API int qeLibFind(qeLibChunk *dst,chr *name,chr *namex,uns32 type);
QE_API int qeLibFindPathless(qeLibChunk *dst,
   chr *name,chr *namex,uns32 type);
#define qeLibInstFind qeLibFind // depricated
#define qeLibInstFindPathless qeLibFindPathless // depricated
QE_API int qeLibFindSibling(qeLibChunk *dst,
   qeObj *o,chr *name,chr *namex,uns32 type);
QE_API qeObj *qeLibFirst(void);
QE_API qeObj *qeLibListHead(void);
QE_API qeObj *qeLibLoaded(chr *name);

//
// SOUND
//

// sound defines & masks for qeSndNew()
#define M_SNDNEW_LOOPING    0x00100000 // loop the sound
#define M_SNDNEW_STREAM     0x00200000 // stream from file
#define M_SNDNEW_DONTSTREAM 0x00400000 // don't stream (load into RAM)
#define M_SNDNEW_PREDEFINED 0x00800000 // special named sound
#define M_SNDNEW_PRI        0x0000000f // priority bits
#define S_SNDNEW_PRI        0          // priority shift (priority 0..15)
#define M_SNDNEW_CHMASK     0x00000ff0 // channel bits
#define S_SNDNEW_CHMASK     4          // channel shift (channel mask 0x00..0xff)

// expanded bit fields
#define M_SNDNEW_CH_1       0x00000010  // channel 1
#define M_SNDNEW_CH_2       0x00000020  // channel 2
#define M_SNDNEW_CH_3       0x00000040  // channel 3
#define M_SNDNEW_CH_4       0x00000080  // channel 4
#define M_SNDNEW_CH_5       0x00000100  // channel 5
#define M_SNDNEW_CH_6       0x00000200  // channel 6
#define M_SNDNEW_CH_7       0x00000400  // channel 7
#define M_SNDNEW_CH_8       0x00000800  // channel 8
#define M_SNDNEW_PRI_0      0x00000000  // lowest priority (no priority)
#define M_SNDNEW_PRI_1      0x00000001  // priority
#define M_SNDNEW_PRI_2      0x00000002  // priority
#define M_SNDNEW_PRI_3      0x00000003  // priority
#define M_SNDNEW_PRI_4      0x00000004  // priority
#define M_SNDNEW_PRI_5      0x00000005  // priority
#define M_SNDNEW_PRI_6      0x00000006  // priority
#define M_SNDNEW_PRI_7      0x00000007  // priority
#define M_SNDNEW_PRI_8      0x00000008  // priority
#define M_SNDNEW_PRI_9      0x00000009  // priority
#define M_SNDNEW_PRI_10     0x0000000a  // priority
#define M_SNDNEW_PRI_11     0x0000000b  // priority
#define M_SNDNEW_PRI_12     0x0000000c  // priority
#define M_SNDNEW_PRI_13     0x0000000d  // priority
#define M_SNDNEW_PRI_14     0x0000000e  // priority
#define M_SNDNEW_PRI_15     0x0000000f  // highest

#define QESNDVOL_MAX 255  // loudest sound, volume 0 is silent
#define SNDVOL_MAX QESNDVOL_MAX // depricated

QE_API int qeSndNew(chr *name,uns flags,uns tProtect,chr *path);
QE_API int qeSndPlay(chr *name); // returns play id on success
QE_API int qeSndStopAll(void);
QE_API int qeSndMasterVol(int vol);

//
// IMAGES
//

#define M_QEIMGNEW_LOAD 0x0001
QE_API int qeImgNew(chr *imgName,uns flags,chr *path);
QE_API int qeImgNewRGBA(chr *imgName,uns width,uns height,uns8 *rgba);
QE_API int qeImgNewSP(chr *name,chr *sp,...);
QE_API int qeImgLoad(chr *imgName);
QE_API int qeImgUnload(chr *imgName);
QE_API int qeImgDelete(chr *imgName);
#define M_QEIMGINFO_FMT_GLFMT 0x00ffff
#define M_QEIMGINFO_FMT_ALPHA 0x010000 // has alpha
QE_API int qeImgInfo(chr *imgName,uns *widthp,uns *heightp,uns *fmt,void *pixp);
QE_API int qeImgPixGet(uns8 *rgba,chr *name,int x,int y);

//
// TEXTURES
//

#define M_QETEX_S_CLAMP    0x0001
#define M_QETEX_T_CLAMP    0x0002
#define M_QETEX_MAG_LINEAR 0x0004
#define M_QETEX_MIN_LINEAR 0x0008

enum {
   QETEXOP_NONE,
   QETEXOP_SETFLAGS,
   QETEXOP_GETFLAGS,
};

QE_API int qeTexOp(chr *imgName,uns op,...);

QE_API int qeTexLoad(chr *imgName);
QE_API int qeTexUnload(chr *imgName);
#define M_QETEXBIND_ALPHA 0x0001
QE_API int qeTexBind(chr *imgName);

//
// SHADERS
//

#define M_QESHADERNEW_PREDEFINED 0x0001
#define M_QESHADERNEW_COMPILE    0x0002
#define M_QESHADERNEW_STRING     0x0004

QE_API int qeShaderNew(chr *name,uns flags,chr *path);
QE_API int qeShaderCompile(chr *name);
QE_API int qeShaderUse(chr *name);

//
// LIGHTS
//

enum {
   QELIGHTOP_NEXT,
   QELIGHTOP_CURRENT,
   QELIGHTOP_NEXTAMBIENT,
   QELIGHTOP_CURRENTAMBIENT,
};

QE_API int qeLightOp(uns op,...);

//
// TIMERS
//

typedef struct {
   uns64 priv[1];
} qeTimerRec;

enum {
   QETIMEROP_NONE,
   QETIMEROP_START,
   QETIMEROP_STOP,
   QETIMEROP_TICKS,
   QETIMEROP_TICK_RESTART,
   QETIMEROP_TICKSPERSECOND,
};

QE_API void qeTimeReset(void); // reset frame timer
QE_API int qeTimeFakeFrameRate(float sec); // force time per frame or 0
QE_API float qeTimeFrame(void); // locked in time for the frame
QE_API float qeTimeDelta(void); // locked in delta for the frame
QE_API void qeLoopReset(void); // reset loop count
QE_API uns qeLoopFrame(void); // loop count since reset
QE_API int qeTimerOp(qeTimerRec *timerec,uns op,...); // loop count since reset

//
// COLLISIONS
//

QE_API int qeColRayPoly(float *cxyz,float *cnrm,
   float *xyzStart,float *xyzEnd,
   float *m12,float *verts,int *faces,int numFaces);

//
// RANDOM
//

#define QERAND_MAX 32767
QE_API int qeRand(void); // 0..QERAND_MAX
QE_API float qeRand01(void); // 0..1

//
// GL FUNCTIONS
//

#define M_QEGLSUPPORT_BASICSHADERS 0x00000100

QE_API uns qeGLSupport(void); // support bits
QE_API void qeGLM9f(float *m9); // concatenate with current OpenGL matrix
QE_API void qeGLM12f(float *m12); // concatenate with current OpenGL matrix
QE_API void qeGLM9GetFacingf(float *m9); // get flat-to-view rotation matrix
QE_API void qeGLM12GetFacingf(float *m12); // get flat-to-view rotation matrix

//
// DEBUGGING
//

QE_API int qePrintf(chr *fmt,...); // print a message
QE_API int qeLogf(chr *fmt,...); // log a message
QE_API int qeErrCount(void);
QE_API int qeErrNext(void);
QE_API chr *qeErrString(int err);

//
// OBJECT NAMES
//

#define QENAME_CAMLENS     "f-camlens"
#define QENAME_CAMSYS      "f-camsys"
#define QENAME_CLEARSCREEN "f-clrscr"
#define QENAME_DRAWCONS    "f-drwcons"
#define QENAME_DRAWMODE    "f-drwmode"
#define QENAME_FIRSTLIGHT  "f-lighta"

// C in C++ wrapper
#ifdef __cplusplus
}
#endif

#endif // ndef QE_H
