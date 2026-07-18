// Copyright (C) 2006-2008 Joe Linhoff, All Rights Reserved
// qe.h
// this is the API to the engine
#ifndef QE_H
#define QE_H

#define QE_H_VERS 0x00000055

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
#define C4TYPE_COL1 C4('c','o','l','1')

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
   float *normals; // pointer to vertex normals
   int *faces; // pointer to faces
   float *uvs; // pointer to face texture coords
} qeLibGeo2;

#define M_QELIBGEO3_NOTVISIBLE   0x0001
#define M_QELIBGEO3_DONTLIGHT    0x0002
#define M_QELIBGEO3_FACENORMALS  0x0004 // one normal for each face-vert
#define M_QELIBGEO3_VERTNORMALS  0x0008 // one normal for each vert
typedef struct {
   uns32 libType; // C4
   uns16 libFlags; // library flags
   uns16 libSize; // size of this record
   int32 numv; // number of verts
   int32 numf; // number of faces
   int32 numfv; // number of faces-verts
   float *verts; // pointer to verts
   float *normals; // pointer to normals
   int   *faces; // pointer to face data
   float *uvs; // pointer to uv data
   chr   *imgname; // image name
   flt32 xyzrCull[4]; // culling xyzr
   chr* note; // note field
} qeLibGeo3;

typedef struct {
   // -- experimental -- this will change --
   uns num; // number of elements (triangles)
   uns numv;
   float *verts;
   float *normals;
   float *uvs;
   ptr allocated;
   uns vboVertArray;
   uns vboNormalArray;
   uns vboUVArray;
} qeLibGeo4;

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

#define M_QELIBANI1_LOOP      0x0001 // loop the animation
#define M_QELIBANI1_DONTLOOP  0x0002 // dont loop the animation
#define M_QELIBANI1_FIRSTLAST 0x0004 // last==first frame of animation
typedef struct {
   uns32 libType;
   uns16 libFlags;
   uns16 libSize;
   chr   *name;
   chr   *jname;
   uns32 numk;
   uns16 keyFlags;
   uns16 xx1;
   qeLibKeyHead *keys; // first valid
   qeLibKeyHead *keyx; // first invalid
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
   uns32 libType; // type of this record
   uns16 libFlags; // M_QELIBCAM1_ flags
   uns16 libSize; // size of this record
   chr*  name; // name of camera
   flt32 orthoWidth; // orthographic width
   flt32 clipNear; // near clipping plane
   flt32 clipFar; // far clipping plane
   flt32 fovy; // field of view in vertical, radians
   flt32 hdivw; // aspect ratio, width dividedy by height
   flt32 centerOfInterest; // distance to center of interest
   flt32 trans[3]; // translation
   flt32 rot[3]; // rotation
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
   flt32 nrmk[4];
   int32 numv;
   // followed by numv offsets into verts
} qeLibCol1Face;

typedef struct {
   flt32 xyzMin[3];
   flt32 xyzMax[3];
   int32 numColFaceOffs;
   // followed by num uns32 offsets into colFaces (in qeLibCol1)
} qeLibCol1MMVol;

typedef struct {
   uns32 libType;
   uns16 libFlags;
   uns16 libSize;
   uns32 endian;
   int32 numf;
   qeLibCol1Face *colFaces; // qeLibCol1Face
   int32 numv;
   flt32 *verts; // vertices
   int32 numvol;
   qeLibCol1MMVol *mmvol; // qeLibCol1MMVol
} qeLibCol1;

typedef struct {
   uns32 libType;
   uns16 libFlags;
   uns16 libSize;
   chr* name;
   uns32 type;
   chr* tags;
} qeLibTag1;

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
QE_API int qeSleep(float time); // sleep in seconds

// other
QE_API int qeAbort(int code); // signal engine to abort
QE_API int qePauseOnExit(void); // pause for key on exit
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

// engine objects
QE_API qeObj* qeObjAddFnc(fnc_iv fnc); // add C function obj
QE_API qeObj* qeObjAddFncSP(ptr adr,chr *sp,...); // add C func w/sp
QE_API qeObj* qeObjAddCFnc(ptr classptr,...); // add C++ function obj
QE_API qeObj* qeObjAddCFncSP(ptr classptr,...); // add C++ func obj w/sp
QE_API qeObj* qeObjAddCpr(fnc_ii fnc); // add co-process routine obj
QE_API qeObj* qeObjAddCprSP(ptr fnc,chr *sp,...); // add cpr obj w/sp
QE_API qeObj* qeObjAddInst(ptr adr,uns type); // add instance
QE_API qeObj* qeObjAddInstSP(ptr adr,uns type,chr *sp,...); // add inst w/sp
QE_API qeObj* qeObjAddSharedSP(uns recsize,chr *sp,...); // add shared w/sp
QE_API qeObj* qeObjAddCSharedSP(ptr classptr,chr *sp,...); // add C++ shared w/sp

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
QE_API int qeDrawLitFaces(float *verts,float *normals,
   int *faces,int numf);
QE_API int qeDrawLitFacesVertNormals(float *verts,float *vnormals,
   int *faces,int numf);
//QE_API int qeDrawLitUVFaces(float *verts,float *fvnormals,
//   int *faces,float *uvs,int numf);
//                            _v_,_nrm_,_f_,_uv_,_numf_) /* defunct */ \
//   qeDrawLitUV(_v_,_f_,_uv_,_numf_)
QE_API int qeDrawLitUV(float *verts,int *faces,float *uvs,int numf);
QE_API int qeDrawFaceNormalsLitUV(float *verts,float *normals,
   int *faces,float *uvs,int numf);
QE_API int qeDrawVertNormalsLitUV(float *verts,float *normals,
   int *faces,float *uvs,int numf);
QE_API int qeDrawScaledLitFaces(float *scalexyz,float *verts,
   float *normals,int *faces,int numf);
QE_API int qeDrawScaledLitUVFaces(float *scalexyz,float *verts,
   float *normals,int *faces,float *uvs,int numf);
QE_API int qeDrawNormals(float *verts,int *faces,int numf);
QE_API int qeDrawFaceNormals(float *verts,float *normals,
   int *faces,int numf,float size);
QE_API int qeDrawVertVecs(float *verts,int numv,float *vecs,float mul);
QE_API int qeDrawLitUVTangentFaces(int tangentloc,float *verts,
   float *normals,float *tangents,int *faces,float *uvs,int numf);

QE_API int qeBuildSmoothVertNormals(float *normals,float *verts,int numv,
   int *faces,int numf);
QE_API int qeBuildTangents(float *vtangents,float *verts,int numv,
   int *faces,float *uvs,int numf);
QE_API int qeOrthonormalize(float *vdst,float *vsrc,float *vnormals,int numv);

//
// BUILD CAMERA MATRICES
//

QE_API void qeCamM12f(float *m12,float *rot,float *xyz);
QE_API int qeCamLookAtM12f(float *m12,float *xyzPos,float *xyzLookAt,float *xyzUp);

//
// OPTIONS
//

// options for qeOption*()
#define M_QEOPTION_CLEARSCREEN  0x000001 // clear screen every loop
#define M_QEOPTION_AUTOCAM      0x000002 // add first (arcball) camera
#define M_QEOPTION_DRAWCONSOLE  0x000100 // draw console
#define M_QEOPTION_INITGL       0x000200 // init OpenGL
#define M_QEOPTION_DRAWMODE     0x000400 // black on white
#define M_QEOPTION_FIRSTLIGHT   0x000800 // add first light
#define M_QEOPTION_LOGPRINTF    0x002000 // qePrintf() output also to log
#define M_QEOPTION_CHECKLOGSIZE 0x004000 // check & delete log if too big
#define M_QEOPTION_INERNALSLEEP 0x008000 // sleep inside the engine loop
#define M_QEOPTION_CLEARSTENCIL 0x010000 // clear stencil buffer every loop
#define M_QEOPTION_PAGEFLIP     0x010000 // flip pages

// Option control
QE_API int qeOptionTurnOff(uns flags); // turn options off
QE_API int qeOptionTurnOn(uns flags); // turn options on
QE_API uns qeOptionCurrent(void); // get current options

// Engine flags
#define M_QEFLAG_WIREFRAME 0x0001
#define M_QEFLAG_JOINTS    0x0002
#define M_QEFLAG_NORMALS   0x0004
#define M_QEFLAG_STEP      0x0008
QE_API uns qeGetFlags(void);

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
   QEINPBUTTON_LEFT2,
   QEINPBUTTON_RIGHT2,
   QEINPBUTTON_UP3,
   QEINPBUTTON_DOWN3,
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

enum { // raw key codes are used for key mappings
   QEINPRAWKEYCODE_SPECIAL=0x1000,
   QEINPRAWKEYCODE_UP,
   QEINPRAWKEYCODE_DOWN,
   QEINPRAWKEYCODE_LEFT,
   QEINPRAWKEYCODE_RIGHT,
   QEINPRAWKEYCODE_ALT,
   QEINPRAWKEYCODE_LSHIFT,
   QEINPRAWKEYCODE_RSHIFT,
   QEINPRAWKEYCODE_CTL,
   QEINPRAWKEYCODE_PAUSE,
   QEINPRAWKEYCODE_CAPSLOCK,
   QEINPRAWKEYCODE_UP2,
   QEINPRAWKEYCODE_DOWN2,
   QEINPRAWKEYCODE_F1,
   QEINPRAWKEYCODE_F2,
   QEINPRAWKEYCODE_F3,
   QEINPRAWKEYCODE_F4,
   QEINPRAWKEYCODE_F5,
   QEINPRAWKEYCODE_F6,
   QEINPRAWKEYCODE_F7,
   QEINPRAWKEYCODE_F8,
   QEINPRAWKEYCODE_F9,
}; // raw key codes

QE_API uns qeInpButton(uns inpb); // QEINPBUTTON_
QE_API uns qeInpMouseXY(int *xy); // fill in xy array with position
QE_API int qeInpMouseSetXY(int *xy); // set position from xy array
QE_API uns qeInpState(void); // get current M_QEINPSTATE_ flags
QE_API int qeInpMouseWheel(void); // read the wheel
QE_API int qeInpKeyBuf(uns *keyBuf,uns keyBufSize); // empty keyboard buffer

enum {
   QEINPOP_NONE,
   QEINPOP_KEYBUFGETNUM, // -- get number of entries in keyBuf
   QEINPOP_KEYBUFCHECKOVERFLOW, // -- return keyBuf overflow flag
   QEINPOP_KEYBUFCPYCHRBUF, // chr *dst,uns dstSize -- copy out of keyBuf
   QEINPOP_KEYBUFRESET, // -- reset keyBuf
   QEINPOP_KEYBOARDMAPPINGS_OFF, // -- turn keyboard mappings off
   QEINPOP_KEYBOARDMAPPINGS_ON, //-- turn keyboard mappings on
   QEINPOP_JOYGETNUM_HATS, // uns joy -- return number of hats
   QEINPOP_JOYGETNUM_MOTORS, // uns joy -- return number of motors
   QEINPOP_CURSOFF, // turn cursor off
   QEINPOP_CURSON, // turn cursor on
   QEINPOP_SETKEYMAP, // int *mapTable -- QEINPBUTTON_,key,0 .. 0
   QEINPOP_DEFAULTKEYMAP, // reset to default mappings
   QEINPOP_RECOUNTJOYSTICKS, // int n -- count the number of joysticks attached
   QEINPOP_JOYMAPSET, // uns joynum, uns joymap -- joymap must be > max joynum
   QEINPOP_JOYMAPGET, // uns joynum -- return mapping -- zero = unmapped
   QEINPOP_JOYMAPRESETALL, // reset (unmap) all mappings
   QEINPOP_JOYGETDEVICEINFO, // int n -- chr *buf, uns bufsize
};

#define QEINPOP_GETNUM QEINPOP_KEYBUFGETNUM // defunct
#define QEINPOP_CHECKOVERFLOW QEINPOP_KEYBUFCHECKOVERFLOW // defunct
#define QEINPOP_CPYCHRBUF QEINPOP_KEYBUFCPYCHRBUF // defunct
#define QEINPOP_RESET QEINPOP_KEYBUFRESET // defunct

#define QEINP_JOYAXIS_MAX 32767
#define QEINP_JOYAXIS_MIN (-QEINP_JOYAXIS_MAX)

enum {
   QEJOYAXIS_LEFT_X=0,
   QEJOYAXIS_LEFT_Y,
   QEJOYAXIS_RIGHT_X,
   QEJOYAXIS_RIGHT_Y,
   QEJOYAXIS_LEFT_TRIGGER,
   QEJOYAXIS_RIGHT_TRIGGER,
   QEJOYAXIS_LEFT_Z,
   QEJOYAXIS_RIGHT_Z,
   QEJOYAXIS_NUM
};

enum {
   QEJOYBUTTON_A=0,
   QEJOYBUTTON_B,
   QEJOYBUTTON_X,
   QEJOYBUTTON_Y,
   QEJOYBUTTON_LSHOULDER,
   QEJOYBUTTON_RSHOULDER,
   QEJOYBUTTON_DPAD_LEFT,
   QEJOYBUTTON_DPAD_RIGHT,
   QEJOYBUTTON_DPAD_UP,
   QEJOYBUTTON_DPAD_DOWN,
   QEJOYBUTTON_START,
   QEJOYBUTTON_BACK,
   QEJOYBUTTON_LTHUMB,
   QEJOYBUTTON_RTHUMB,
   QEJOYBUTTON_NUM
};

// default mappings to keys
#define QEINPBUTTON_KEY_Z QEINPBUTTON_A
#define QEINPBUTTON_KEY_X QEINPBUTTON_B
#define QEINPBUTTON_KEY_C QEINPBUTTON_X
#define QEINPBUTTON_KEY_V QEINPBUTTON_Y

#define QEJOYMOTOR_SPEED_MAX 32767 // max motor speed
#define QEJOYMOTOR_MOTOR_ALL 0xffff // all motors

QE_API int qeInpOp(uns op,...); // QEINPOP_

#define M_QEJOYFLAGS_XBOX360   0x00000001 // M_QEJOYFLAGS_DEVTYPE
#define M_QEJOYFLAGS_DIRECT    0x00000002 // M_QEJOYFLAGS_DEVTYPE
#define M_QEJOYFLAGS_WIIMOTE   0x00000003 // M_QEJOYFLAGS_DEVTYPE
#define M_QEJOYFLAGS_DEVTYPE   0x000000ff // M_QEJOYFLAGS_DEVTYPE
#define M_QEJOYFLAGS_ATTACHED  0x00001000 // currently attached
#define M_QEJOYFLAGS_LEFTAXIS  0x00002000 // has left axis
#define M_QEJOYFLAGS_RIGHTAXIS 0x00004000 // has right axis
#define M_QEJOYFLAGS_DPAD      0x00008000 // has dpad

// joy parameter can be either joynum or joymap
QE_API uns qeInpJoyNum(void); // joysticks attached -- valid after qeOpen()
QE_API int qeInpJoyInfo(uns joy,uns *axisp,uns *buttonp); // get num axes, buttons
QE_API int qeInpJoyAxis(uns joy,uns axis); // get, use QEJOYAXIS_
QE_API uns qeInpJoyButton(uns joy,uns button); // get, use QEJOYBUTTON_
QE_API int qeInpJoyMotor(uns joy,uns motor,uns speed); // set, use QEJOYMOTOR_
QE_API uns qeInpJoyFlags(uns joy); // return M_QEJOYFLAGS_ for joy

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
#define qeLibScanFirst(_lsr_) MEMZ(*(_lsr_))
QE_API int qeLibScanNext(qeObjScan *sr,qeLibChunk *lib,chr *name,chr *namex);
QE_API chr *qeLibScanLibName(qeObj *o);
QE_API int qeLibFind(qeLibChunk *dst,chr *name,chr *namex,uns32 type);
QE_API int qeLibFindPathless(qeLibChunk *dst,chr *name,chr *namex,uns32 type);
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
#define M_SNDNEW_DONTRECORDNOTFOUND 0x01000000 // don't record the error if not found
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

QE_API int qeSndNew(chr *name,uns flags,uns tProtect,chr *path);
QE_API int qeSndPlay(chr *name); // returns play id on success
QE_API int qeSndStopAll(void); // stop all sounds
QE_API int qeSndMasterVol(int vol); // set master volume

//
// IMAGES
//

#define M_QEIMGNEW_LOAD               0x0001 // load into memory
#define M_QEIMGNEW_DONTRECORDNOTFOUND 0x0002 // don't record the error if not found
#define M_QEIMGNEW_REPLACE            0x0004 // OK to replace
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
QE_API int qeImgRename(chr *name,chr *newname);

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

QE_API int qeLightOp(uns op,...); ///< light operation

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
   QETIMEROP_TICKSPERMILLISECOND,
};

QE_API void qeTimeReset(void); ///< reset game timer
QE_API int qeTimeFakeFrameRate(float sec); ///< force time per frame, 0 to run
QE_API float qeTimeFrame(void); ///< game time for the frame
QE_API float qeTimeDelta(void); ///< delta time for the frame
QE_API float qeTimeSinceReset(void); ///< use qeTimeFrame -- this free runs
QE_API void qeLoopReset(void); ///< reset loop count
QE_API uns qeLoopFrame(void); ///< loop count since reset
QE_API int qeTimeMul(float mul); ///< 0 for normal
QE_API int64 qeTimerOp(qeTimerRec *timerec,uns op,...); ///< high speed timer operations

//
// WINDOWS
//

#define M_QEWINNEW_OPEN 0x0001
QE_API int qeWinNewSP(chr *name,uns flags,int width,int height,chr *sp,...);
QE_API int qeWinOpen(chr *name);
QE_API int qeWinClose(chr *name);

//
// COLLISIONS
//

QE_API int qeColRayPoly(float *dstXYZ,float *dstNrm,float *dstFrac,
   float *xyzStart,float *xyzEnd,
   float *m12,float *verts,int *faces,int numFaces);
QE_API int qeColRayLibObj(float *dstXYZ,float *dstNrm,float *dstFrac,
   float *xyzStart,float *xyzEnd,
   float *m12,qeObj *libColObj);
QE_API int qeColLibObjDrawSP(qeObj *libColObj,chr *sp,...);

//
// RANDOM
//

#define QERAND_MAX 32767
QE_API uns qeRand(void); ///< get random 0..QERAND_MAX
QE_API float qeRand01(void); ///< get random 0..1
QE_API float qeRandMinMaxf(float min,float max); ///< get random min..max
QE_API int qeRandMinMaxi(int min,int max); ///< get random min..max
QE_API uns qeRandSeed(uns seed); ///< set random seed

//
// GL FUNCTIONS
//

#define M_QEGLSUPPORT_BASICSHADERS  0x00000100
#define M_QEGLSUPPORT_VERTEXBUFFERS 0x00000200

QE_API uns qeGLSupport(void); ///< get support bits
QE_API void qeGLM9f(float *m9); ///< concatenate with current OpenGL matrix
QE_API void qeGLM12f(float *m12); ///< concatenate with current OpenGL matrix
QE_API void qeGLXYZM9f(float *xyz,float *m9); ///< concat w/current OpenGL matrix
QE_API void qeGLM9GetFacingf(float *m9); ///< get flat-to-view rotation matrix
QE_API void qeGLM12GetFacingf(float *m12); ///< get flat-to-view rotation matrix

//
// DEBUGGING
//

enum {
   QEDEBUGOP_DUMP_ANIMATION_CHANNELS=1, // 0 or 1
};

QE_API int qePrintf(chr *fmt,...); ///< print a message
QE_API int qeLogf(chr *fmt,...); ///< log a message
QE_API int qeErrCount(void); ///< get current error count
QE_API int qeErrNext(void); ///< get next error
QE_API chr *qeErrString(int err); ///< turn error number into string
QE_API int qeDebugOp(uns op,...);

//
// SYSTEM OBJECT NAMES
//

enum {
   QENAMEID_NONE,
   QENAMEID_CAMAUTO,
   QENAMEID_CLEARSCREEN,
   QENAMEID_DRAWCONS,
   QENAMEID_DRAWMODE,
   QENAMEID_FIRSTLIGHT,
   QENAMEID_MARK_SHARED,
   QENAMEID_MARK_CONTROL,
   QENAMEID_MARK_UPDATE,
   QENAMEID_MARK_DRAW,
};

QE_API chr *qeNameGet(uns qenameid);

// PLATFORM OPERATIONS
// be careful when using these, they will likely make your application non-portable
enum {
   QEPLATFORMOP_NONE,
   QEPLATFORMOP_GETHWND, // HWND *pHwnd
   QEPLATFORMOP_GETHDC, // HDC *pHDC
   QEPLATFORMOP_GETHGLRC, // HGLRC *pHGLRC
   QEPLATFORMOP_WINPAINT_CALLBACK, // chr *sp,callback,this
   QEPLATFORMOP_WINDISPLAYCHANGE_CALLBACK, // chr *sp,callback,this
   QEPLATFORMOP_WINRESIZE_CALLBACK, // chr *sp,callback,this
};

QE_API int qePlatformOp(int op,...);

// C in C++ wrapper
#ifdef __cplusplus
}
#endif

#endif // ndef QE_H
