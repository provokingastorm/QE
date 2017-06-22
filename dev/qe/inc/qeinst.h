// Copyright (C) 2006-2007 Joe Linhoff, All Rights Reserved
// qeinst.h
#ifndef QEINST_H
#define QEINST_H

// C in C++ wrapper
#ifdef __cplusplus
extern "C" {
#endif

// structure note:
// the structures in this file may change with future implementations

//
// C4TYPE_NODE
//

#define M_QEINSTNODE1_PUSHMAT 0x0001
#define M_QEINSTNODE1_POPMAT  0x0002

typedef struct { // see structure note at top of file
   uns flags;
} qeInstNode1;

//
// C4TYPE_MAT
//

typedef struct { // see structure note at top of file
   uns libType;
   ptr libData;
   float trans[3];
   float rot[3];
   float scale[3];
   float mat12[12];
} qeInstNodeMat;

//
// C4TYPE_ANI
//

// flags for qeInstNodeAni
#define M_QEINSTANI_AUTODRIVE   0x000001 // search siblings for first drivable
#define M_QEINSTANI_DRIVERTSF   0x000002 // qeRTS follows
#define M_QEINSTANI_DRIVEM12F   0x000004 // qeM12f follows
#define M_QEINSTANI_LOOP        0x000008 // loop this animation (override anim)
#define M_QEINSTANI_DONTLOOP    0x000010 // dont loop this anim (override anim)
#define M_QEINSTANI_LINEAR      0x000020 // linear interpolation
#define M_QEINSTANI_FIXEDVEL    0x000040 // fixed velocity
#define M_QEINSTANI_DONTSCALE   0x000080 // don't scale
#define M_QEINSTANI_DONE        0x001000 // anim finished, cleared on start
#define M_QEINSTANI_LOOPED      0x002000 // anim looped, sys doesn't clear
#define M_QEINSTANI_SYSTEM      0xff0000 // reserved for system
#define M_QEINSTANI_RESTART     0x100000 // restart

typedef struct { // see structure note at top of file
   ptr lib0Data; // key0
   ptr lib1Data; // key1
   uns flags; // flags
   int16 keyCurrent; // current keyframe
   int16 keyCount; // total keys run through
   float timeStart; // last key-frame-zero start
   float timeMul; // multiplier to change speed of animation
   float time0; // start of key0
   float time1; // end of key1
   float timeDiv; // 1/(end-start)
   ptr kh0; // key0
   ptr kh1; // key1
   float frac;
   uns priv[10];
} qeInstNodeAni;

//
// C4TYPE_SKL
//

typedef struct { // see structure note at top of file
   uns libType;
   ptr libData;
   qeObj *oSelf;
   uns numMats;
   float *m12Inputs; // input transform matrix
   float *m12User; // user matrix (NUL for no anim, m12 space is allocated)
   float *m12Outputs; // output total transform for joint
   float *m12Joints; // output partial (orientation only) transform
} qeInstNodeSkl;

//
// C4TYPE_SKN
//

#define M_QEINSTSKN_DONTVISIT 0x0001
#define M_QEINSTSKN_DONTDRAW  0x0002
typedef struct { // see structure note at top of file
   uns libType;
   ptr libData;
   uns geoType;
   ptr geoLib;
   uns flags;
   float *verts;
   float *normals;
} qeInstNodeSkn;

//
// C4TYPE_CAM
//

#define M_QEINSTCAM_ARCBALL 0x0001
#define M_QEINSTCAM1_ORTHO  0x0002
typedef struct { // see structure note at top of file
   uns libType;
   ptr libData;
   uns flags;
   float rot[3];
   float trans[3];
   ptr p1;
} qeInstNodeCam;

typedef struct { // see structure note at top of file
   uns libType;
   ptr libData;
   uns flags;
} qeInstNodeLit;

#define M_QEINSTVISITSKN_DONTDRAW 0x0001

QE_API int qeInstVisitGEO2(ptr inst,uns flags);
QE_API int qeInstVisitGEO3(ptr inst,uns flags);
QE_API int qeInstVisit_MAT(ptr inst,uns flags);
QE_API int qeInstVisit_NODE(ptr inst,uns flags);
QE_API int qeInstVisit_ANI(ptr inst,uns flags);
QE_API int qeInstVisit_SKL(ptr inst,uns flags);
QE_API int qeInstVisit_SKN(ptr inst,uns flags);
QE_API int qeInstVisit_CAM(ptr inst,uns flags);
QE_API int qeInstVisit_LIT(ptr inst,uns flags);

//
// ANIMATION
//

// use these flags with qeObjAniOp() -- these are 'safe' flags
#define M_QEOBJANI_LOOP      M_QEINSTANI_LOOP // loop this animation
#define M_QEOBJANI_DONTLOOP  M_QEINSTANI_DONTLOOP // dont loop
#define M_QEOBJANI_DRIVERTS  M_QEINSTANI_DRIVERTSF // qeRTS imm following
#define M_QEOBJANI_DRIVEM12  M_QEINSTANI_DRIVEM12F // qeM12 imm following
#define M_QEOBJANI_HASLOOPED M_QEINSTANI_LOOPED // looped / ended, not cleared
#define M_QEOBJANI_DONE      M_QEINSTANI_DONE // ended, cleared on start
#define M_QEOBJANI_FIXEDVEL  M_QEINSTANI_FIXEDVEL // fixed velocity spline
#define M_QEOBJANI_DONTSCALE M_QEINSTANI_DONTSCALE // dont scale

enum {
   QEOBJANIOP_NONE,
   QEOBJANIOP_SETFLAGS, // uns M_QEOBJANI_
   QEOBJANIOP_CLEARFLAGS, // uns M_QEOBJANI_
   QEOBJANIOP_GETFLAGS, // get flags
   QEOBJANIOP_GETKEYCOUNT, // total keys animated
   QEOBJANIOP_SETKEYCOUNT, // int count
   QEOBJANIOP_GETKEYCURRENT, // current key in animation
   QEOBJANIOP_GETSTARTTIME, // float *resultp
   QEOBJANIOP_GETTIMEMUL, // float *resultp
   QEOBJANIOP_SETTIMEMUL, // float timemul
   QEOBJANIOP_GETVEL, // float *resultp
   QEOBJANIOP_SETVEL, // float vel
   QEOBJANIOP_GETTOTALKEYS, // total keys in animation
   QEOBJANIOP_GETTOTALTIME, // float *resultp
   QEOBJANIOP_GETDIST, // float *resultp
   QEOBJANIOP_SETDIST, // dist
   QEOBJANIOP_GETDISTTOTAL, // float *resultp
};

// animation instances
QE_API qeInstNodeAni* qeObjAni(qeObj *o);
QE_API int qeObjAniVisit(qeObj *o,uns flags);
#define qeObjAniStart(_oani_,_libAni_) \
   qeObjAniStartSP(_oani_,_libAni_,NUL)
QE_API int qeObjAniStartSP(qeObj *o,qeObj *libAni,chr *sp,...);
#define qeObjAniInterp(_oani_,_libani_,_time_) \
   qeObjAniStartSP(_oani_,_libani_,"i",((float)_time_))
QE_API int qeObjAniRestart(qeObj *o);
QE_API int qeObjAniSnapFrac(qeObj *o,float keyFrameFrac);
QE_API int qeObjAniSnapTime(qeObj *o,float timeFromAnimStart);
QE_API qeRTS* qeObjAniRTS(qeObj *o);
QE_API float* qeObjAniM12f(qeObj *o);
QE_API int qeObjAniOp(qeObj *o,uns op,...);
#define qeObjAniSetFlags(_oani_,_flags_) \
   qeObjAniOp(_oani_,QEOBJANIOP_SETFLAGS,_flags_)
#define qeObjAniClearFlags(_oani_,_flags_) \
   qeObjAniOp(_oani_,QEOBJANIOP_CLEARFLAGS,_flags_)
#define qeObjAniKeyCountGet(_oani_) \
   qeObjAniOp(_oani_,QEOBJANIOP_GETKEYCOUNT)
#define qeObjAniKeyCountSet(_oani_,_n_) \
   qeObjAniOp(_oani_,QEOBJANIOP_SETKEYCOUNT,_n_)
#define qeObjAniKeyCurrentGet(_oani_) \
   qeObjAniOp(_oani_,QEOBJANIOP_GETKEYCURRENT)
#define qeObjAniSetVel(_oani_,_vel_) \
   qeObjAniOp(_oani_,QEOBJANIOP_SETVEL,((float)(_vel_)))

QE_API int qeLibScaleAnims(qeObj *olib,float sx,float sy,float sz);

//
// SKELETON ANIMATIONS
//

enum {
   QEJOINTEDOP_NONE,
   QEJOINTEDOP_DRAW,
   QEJOINTEDOP_DONTDRAW,
   QEJOINTEDOP_VISIT,
   QEJOINTEDOP_DONTVISIT,
};

QE_API qeObj* qeObjJointedAddSP(ptr libSkl,chr *sp,...);
QE_API void qeObjJointedFree(qeObj *job);
QE_API qeObj* qeObjJointedSkel(qeObj *job);
QE_API int qeObjJointedOp(qeObj *job,uns op,...);

// C in C++ wrapper
#ifdef __cplusplus
}
#endif

#endif // QEINST_H

