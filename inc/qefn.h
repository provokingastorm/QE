// Copyright (C) 2006 Joe Linhoff, All Rights Reserved
// qefn.h
#ifndef QEFN_H
#define QEFN_H

#ifndef QE_API
#ifdef QE_EXPORTS
#define QE_API __declspec(dllexport)
#else // ndef QE_EXPORTS
#define QE_API __declspec(dllimport)
#endif // ndef QE_EXPORTS
#endif // ndef QE_API

#ifdef __cplusplus
extern "C" {
#endif

/// @note This file provides some commonly used, optional routines.
/// The arcball camera provides a simple way
/// to view a scene using the mouse.  Drag with left button to rotate the camera
/// as if a ball were on the screen.

#define M_QE_ARCBALL_VALID       0x0001
#define M_QE_ARCBALL_AB_SETUP    0x0002
#define M_QE_ARCBALL_CAM_SETUP   0x0004
#define M_QE_ARCBALL_INP         0x0008
#define M_QE_ARCBALL_INP_LBUTTON 0x0010
#define M_QE_ARCBALL_INP_RBUTTON 0x0020
#define M_QE_ARCBALL_INP_ALT     0x0040
#define M_QE_ARCBALL_NOWHEEL     0x0080
#define M_QE_ARCBALL_RESETVALID  0x0100
#define M_QE_ARCBALL_DRAGGING    0x1000
   typedef struct {
   uns flags; // flags
   uns wSetup; // width as setup
   uns hSetup; // height as setup
   float inpXY[2]; // input
   int   inpWheel; // input
   float inpPrevXY[2]; // input
   float aWidth; // inv-adj width
   float aHeight; // inv-adj height
   float start[3];
   float end[3];
   float m9base[9];
   float m9cur[9];
   float resetDist; // reset dist
   float resetM9[9]; // reset cur m9
   float resetLookAt[3]; // camera look at
   float camFOV; // camera field of view
   float camNearClip; // camera near clip
   float camFarClip; // camera far clip
   float camDist; // camera distance
   float camLookAt[3]; // camera look at
   float xfmCamPos[3]; // transformed camera pos
   float xfmCamUp[3]; // transformed camera up
} qefnArcBallRec;

/// Set buffer clear parameters.
QE_API int qefnClearScreenRGBADepth(float r,float g,float b,float a,float d);
QE_API int qefnClearScreen(void); // swap buffers, clear, reset lighting
QE_API int qefnSysCamAuto(void); // trackball cam
QE_API int qefnSysCamFlat(void); // ortho: (0,0) top left (1,1) bottom right
QE_API int qefnSysCam2D(void); // ortho: (0,0) bottom left (1,1) top right
QE_API int qefnSysCamBitmap(void); // ortho: (0,0) top left (w-1,h-1) bot right
QE_API int qefnSysCamPerspectiveId(void);
QE_API int qefnSysCamTransform(void);
QE_API int qefnCamArcBall(qefnArcBallRec *ab); // persp: arcball
/// Setup arcball camera's record
QE_API int qefnCamArcBallSetCam(qefnArcBallRec *ab,float fovRad,
   float clipNear,float clipFar,
   float *xyzLookAt,float *xyzPos,float *m9);
QE_API int qefnMouseCamMove(void);
QE_API int qefnSysLight(void);
QE_API int qefnSysMaterial(void);
QE_API int qefnConsDraw(void);
QE_API int qefnDrawCube(float scale);
/// Draw an XYZ axes in red green blue
QE_API int qefnDrawAxes(float scale);
QE_API int qefnDrawRoot(float scale);
QE_API int qefnDrawGrid(uns num,float size); // draw num x num grid
QE_API int qefnDrawRay(float x,float y,float z);
QE_API int qefnDrawBox(float sx,float sy,float sz);
QE_API int qefnDrawBoxOnGnd(float sx,float sy,float sz);
QE_API int qefnDrawPyramid(float sx,float sy,float sz);
QE_API int qefnDrawModel(uns ptrid,float sx,float sy,float sz);
QE_API int qefnDrawModelLit(uns ptrid,float sx,float sy,float sz);
/// Scan verts to compute the culling center and radius.
/// @param[in] verts vertices
/// @param[in] numv number of vertices
/// @param[out] xyzr pointer to 4 floats set
QE_API int qefnCullXYZR(float *verts,uns numv,float *xyzr);
/// Using currently set camera (modelview matrix) find the ray at the window
/// coordinates
QE_API int qefnWinPickRay(float *xyzRay,float *dirRay,int winx,int winy);
/// Given all the camera information, find the ray at the window coordinates
QE_API int qefnWinPickRayCam(float *xyzRay,float *dirRay,int winx,int winy,
   float winW,float winH,float camAspect,float camNearHeight,float camNearZ,
   float *camM12);
QE_API int qefnCheckForCleanExit(void);

enum {
   QE_PTRID_NONE,
   QE_PTRID_BOXONGND_GEO1, ///< box on the ground
   QE_PTRID_PYRAMID_GEO1, ///< pyramid on the ground
   QE_PTRID_JET01_GEO1, ///< created by Vadim Flaks
};

enum {
   QE_PTRTYPE_NONE,
   QE_PTRTYPE_GEO1,
};

enum {
   QE_PTROP_NONE,
   QE_PTROP_GETTYPE,
};

QE_API ptr qefnPtrGet(uns qePtrId); ///< Get pointer to QE_PTRID_ item
QE_API int qefnPtrOp(uns qePtrId,uns op,...); ///< Get type of pointer

QE_API float qefnBlack4f[];
QE_API float qefnWhite4f[];
QE_API float qefnDarkBlue4f[];
QE_API float qefnDarkGray4f[];
QE_API float qefnRed4f[];
QE_API float qefnGreen4f[];
QE_API float qefnBlue4f[];
QE_API float qefnYellow4f[];
QE_API float qefnM12Identity[]; ///< Identity
QE_API float qefnVec0000[]; ///< zeros

typedef struct {
   // shadow definition
   uns flags;
   int numFaces; // number of faces
   int numEdges; // number of edges
   int numFaceEdges; // number of face-edges
   int *faceEdges; // every edge in every face has one
   int *edgePool; // two indices per edge
} qeShadow;

QE_API void* qeShadowAlloc(int preSize,float *verts,int *faces,int numf);
QE_API int qeShadowCalcPlaneEqs(float *dstPlaneEqs,
   float *verts,int *faces,int numf);
QE_API int qeShadowDraw(qeShadow *shd,float *locallight,float skirtlen,
   float *verts,float *normalk,int *faces);

#define M_QESHADOW_CIRCUITS 0x001
QE_API int qeShadowTmpNeeded(qeShadow *shd,uns flags);
QE_API int qeShadowDrawSP(qeShadow *shd,chr *sp,...);

#ifdef __cplusplus
}
#endif

#endif // ndef QEFN_H
