// Copyright (C) 2006-2007 Joe Linhoff, All Rights Reserved
// qebase.h
#ifndef QEBASE_H
#define QEBASE_H

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

// base types
typedef unsigned char bit; // 1 or 0
typedef char chr; // character
#define S_CHR_SIZE 0 // shift for character size
typedef signed char int8;
typedef unsigned char uns8;
typedef signed short int16;
typedef unsigned short uns16;
typedef int int32;
typedef int32 off32; // all offsets are from their adr base
typedef unsigned int uns32;
typedef unsigned int uns;
typedef float flt32;
typedef double flt64;
typedef void* ptr;

#if BUILD_MSVC
typedef __int64 int64;
typedef unsigned __int64 uns64;
#endif // BUILD_MSVC

#if BUILD_GCC
typedef signed long long int64;
typedef unsigned long long uns64;
#endif // BUILD_GCC

#define MAX_UNS32 0xffffffff
#define MAX_UNS ((uns)~0)
#define MAX_INT 0x7fffffff
#define MIN_INT -MAX_INT
#define FLT_NAN 0x7FFFFFFF // not a number
#define DBL_NAN 0x7FFFFFFFE0000000 // not a number
#define FLT_INF 0x7F800000 // infinity
#define FLT_NINF 0xFF800000 // negative infinity
#define DBL_INF 0x000000007FF00000 // infinity
#define DBL_NINF 0x00000000FFF00000 // negative infinity
#define IS_FLT_NAN(_f_) (*((uns*)&(_f_))==FLT_NAN)
#define IS_DBL_NAN(_f_) (*((uns64*)&(_f_))==DBL_NAN)
#define EPSILON 0.00001

#if DEBUG
   #if BUILD_MSVC
   #define BRK() qe_nobrk?0:1,__debugbreak()
   #else // !BUILD_MSVC
   #define BRK()
   #endif // !BUILD_MSVC
#else // !DEBUG
   #define BRK()
#endif // !DEBUG

QE_API int qe_nobrk;

// macros
#define ret(_r_) {r=(_r_);goto BAIL;}
#define bret(_r_) {BRK();r=(_r_);goto BAIL;}
#define bbail() {BRK();goto BAIL;}
#define NUM(_a_) (sizeof(_a_)/sizeof(_a_[0]))
#define NUL 0L
#define PI 3.14159265358979323846264338327950288419716939937510
#define ALIGN(_n_,_m_) (((_n_)+(_m_))&~(_m_))
#define M_ALIGN_DEFAULT 7

#define QE_DEGTORAD(_deg_) (((_deg_)*PI)/180.0)
#define QE_RADTODEG(_rad_) (((_rad_)*180.0)/PI)

#define PTR_ADD(_p_,_i_) (void*)(((uns8*)(_p_))+(_i_))
#define PTR_SUB(_p_,_i_) (void*)(((uns8*)(_p_))-(_i_))
#define PTR_DIFF(_p1_,_p2_) ((uns8*)(_p1_)-(uns8*)(_p2_))
#define PTR_ALIGN(_p_,_m_) (((uns8*)0)+(((uns)(_p_)+(_m_))&~(_m_)))

#if BUILD_MSVC
   #if _MSC_FULL_VER >= 140050320
   #define DEPRECATED __declspec(deprecated("Depricated."))
   #else // old MSVC
   #define DEPRECATED __declspec(deprecated())
   #endif // old MSVC
#else // !BUILD_MSVC
   #define DEPRECATED
#endif // !BUILD_MSVC

// function types
// void int uns flt ptr
typedef void (*fnc_v)(void);
typedef void (*fnc_vv)(void);
typedef void (*fnc_viiii)(int,int,int,int);
typedef void (*fnc_vp)(void*);
typedef void (*fnc_vpp)(void*,void*);
typedef void (*fnc_vpu)(void*,uns);
typedef void (*fnc_vpup)(void*,uns,void*);
typedef void (*fnc_vppu)(void*,void*,uns);
typedef void (*fnc_vpuu)(void*,uns,uns);
typedef void (*fnc_vppp)(void*,void*,void*);

typedef int (*fnc_iiiii)(int,int,int,int);

typedef int (*fnc_iv)(void);
typedef int (*fnc_ip)(void*);
typedef int (*fnc_iu)(uns);
typedef int (*fnc_ii)(int);
typedef int (*fnc_ipp)(void*,void*);
typedef int (*fnc_ippp)(void*,void*,void*);
typedef int (*fnc_ipppp)(void*,void*,void*,void*);

typedef int (*fnc_ipppi)(void*,void*,void*,int);
typedef int (*fnc_ipppu)(void*,void*,void*,uns);
typedef int (*fnc_ipppuu)(void*,void*,void*,uns,uns);

typedef int (*fnc_ipi)(void*,int);

typedef int (*fnc_ippu)(void*,void*,uns);

typedef int (*fnc_ipa)(void*,...);
typedef int (*fnc_ipu)(void*,uns);
typedef int (*fnc_ipua)(void*,uns,...);
typedef int (*fnc_ipup)(void*,uns,void*);
typedef int (*fnc_ipupp)(void*,uns,void*,void*);
typedef int (*fnc_ipuuu)(void*,uns,uns,uns);
typedef int (*fnc_ipuuup)(void*,uns,uns,uns,void*);

typedef int (*fnc_ipipp)(void*,int,void*,void*);
typedef int (*fnc_ipipa)(void*,int,void*,...);
typedef int (*fnc_ipipppp)(void*,int,void*,void*,void*,void*);

typedef int (*fnc_iia)(int,...);
typedef int (*fnc_iii)(int,int);
typedef int (*fnc_iip)(int,void*);
typedef int (*fnc_iiip)(int,int,void*);
typedef int (*fnc_iipp)(int,void*,void*);
typedef int (*fnc_iua)(uns,...);

typedef void* (*fnc_pu)(uns);

typedef void* (*fnc_pp)(void*);

///////////////////////////////////////////////////////////////////////////////
// basic characters

// see sz.h for more C_ definitions
#define C_BLANK  0x01 // blank
#define C_SELALL 0x01 // select all
#define C_SPEC   0x02 // special character
#define C_COPY   0x03 // ctl-c copy
#define C_BS     0x08 // backspace
#define C_LF     0x0A // line feed
#define C_CR     0x0D // carriage return
#define C_PASTE  0x16 // ctl-v paste
#define C_CUT    0x18 // ctl-x cut
#define C_ESC    0x1b // escape
#define C_DEL    0x7f // delete
#define M_C_ENC  0x80 // encoded bit
#define C_HOME   0x81 // home
#define C_END    0x82 // end
#define C_LEFT   0x83 // left arrow
#define C_RIGHT  0x84 // right arrow
#define C_UP     0x85 // up arrow
#define C_DOWN   0x86 // down arrow
#define C_DECODE(_c_) (_c_)
#define C_ENCODE(_c_) (_c_)

#define C1(_a_) (_a_)
#define C2(_a_,_b_) ((_b_)<<8|(_a_))
#define C3(_a_,_b_,_c_) ((_c_)<<16|(_b_)<<8|(_a_))
#define C4(_a_,_b_,_c_,_d_) ((_d_)<<24|(_c_)<<16|(_b_)<<8|(_a_))

#define C4_CHR0(_c4_) ((_c4_)&0xff)
#define C4_CHR1(_c4_) (((_c4_)>>8)&0xff)
#define C4_CHR2(_c4_) (((_c4_)>>16)&0xff)
#define C4_CHR3(_c4_) (((_c4_)>>24)&0xff)

#if BUILD_MSVC
#pragma warning(disable:4311) // type cast pointer truncation
#pragma warning(disable:4018) // '>' : signed/unsigned mismatch
#pragma warning(disable:4133) // incompatible types - from 'int *__w64 '
#pragma warning(disable:4047) // differs in levels of indirection
#pragma warning(disable:4244) // conversion from double to float
#pragma warning(disable:4305) // truncation from 'double' to 'float'
#pragma warning(disable:4390) // ';' : empty controlled statement
#endif // BUILD_MSVC

#define SET2(_d_,_s1_,_s2_) (_d_)[0]=(_s1_),(_d_)[1]=(_s2_)
#define SET3(_d_,_s1_,_s2_,_s3_) (_d_)[0]=(_s1_), \
   (_d_)[1]=(_s2_),(_d_)[2]=(_s3_)
#define SET4(_d_,_s1_,_s2_,_s3_,_s4_) (_d_)[0]=(_s1_), \
   (_d_)[1]=(_s2_),(_d_)[2]=(_s3_),(_d_)[3]=(_s4_)
#define SET12(_d_,_s1_,_s2_,_s3_,_s4_,_s5_,_s6_,_s7_,_s8_,_s9_,_s10_,_s11_,_s12_) \
   (_d_)[0]=(_s1_),(_d_)[1]=(_s2_),(_d_)[2]=(_s3_), \
   (_d_)[3]=(_s4_),(_d_)[4]=(_s5_),(_d_)[5]=(_s6_), \
   (_d_)[6]=(_s7_),(_d_)[7]=(_s8_),(_d_)[8]=(_s9_), \
   (_d_)[9]=(_s10_),(_d_)[10]=(_s11_),(_d_)[11]=(_s12_)

#define CPY2(_d_,_s_) (_d_)[0]=(_s_)[0],(_d_)[1]=(_s_)[1]
#define CPY3(_d_,_s_) (_d_)[0]=(_s_)[0],(_d_)[1]=(_s_)[1],(_d_)[2]=(_s_)[2]
#define CPY4(_d_,_s_) (_d_)[0]=(_s_)[0],(_d_)[1]=(_s_)[1],(_d_)[2]=(_s_)[2],(_d_)[3]=(_s_)[3]

///////////////////////////////////////////////////////////////////////////////

//
// MEMORY
//

#define MEMZ(_a_) MemZero(&(_a_),sizeof(_a_))

QE_API void MemZero(ptr dst,uns size);
QE_API void MemCpy(ptr dst,ptr src,uns size);
QE_API int MemAlloc(ptr memp,uns size);
QE_API int MemAlloz(ptr memp,uns size);
QE_API void MemFree(ptr mem);
QE_API int MemAllocCount(void);

///////////////////////////////////////////////////////////////////////////////

//
// VECTORS, MATRICES, MATH
//

typedef struct {
   float rot[3];
   float trans[3];
   float scale[3];
} qeRTS;

enum {
   // on paper, the matrix is:
   //  11 21 31
   //  12 22 32
   //  13 23 33
   // in memory, the matrix order is:
   //  11 12 13 21 22 23 31 32 33
   M9_11,
   M9_12,
   M9_13,
   M9_21,
   M9_22,
   M9_23,
   M9_31,
   M9_32,
   M9_33,
   M9_SIZE
};

enum {
   // on paper, the matrix is:
   //  11 21 31 41
   //  12 22 32 42
   //  13 23 33 43
   //  14 24 34 44
   // in memory, the matrix order is:
   //  11 12 13 14 21 22 23 24 31 32 33 34 41 42 43 44
   M16_11,
   M16_12,
   M16_13,
   M16_14,
   M16_21,
   M16_22,
   M16_23,
   M16_24,
   M16_31,
   M16_32,
   M16_33,
   M16_34,
   M16_41, // X
   M16_42, // Y
   M16_43, // Z
   M16_44,
   M16_SIZE
};

#define M16_X M16_41
#define M16_Y M16_42
#define M16_Z M16_43

enum {
   // on paper:
   //   X  Y  Z
   //  11 21 31
   //  12 22 32
   //  13 23 33
   // in memory:
   //  X Y Z 11 12 13 21 22 23 31 32 33
   M12_X,
   M12_Y,
   M12_Z,
   M12_11,
   M12_12,
   M12_13,
   M12_21,
   M12_22,
   M12_23,
   M12_31,
   M12_32,
   M12_33,
   M12_SIZE
};

QE_API void M9SetRotXd(double *mat,double rad);
QE_API void M9SetRotYd(double *mat,double rad);
QE_API void M9SetRotZd(double *mat,double rad);
QE_API void M9SetRotXf(float *mat,float rad);
QE_API void M9SetRotYf(float *mat,float rad);
QE_API void M9SetRotZf(float *mat,float rad);
QE_API void M9Mulf(float *dst,float *cur,float *cat);
QE_API void M9Muld(double *dst,double *cur,double *cat);
QE_API int M9TestOnef(float *m9);
QE_API void M12Mulf(float *dst,float *cur,float *cat);
QE_API void M12Muld(double *dst,double *cur,double *cat);
QE_API void M16Mulf(float *dst,float *cur,float *cat);
QE_API float MathSqrtf(float b);
QE_API float MathSqrtFastf(float b);
QE_API float MathInvSqrtf(float b);
QE_API float MathInvSqrtFastf(float b);
QE_API float MathATan2f(float y,float x);
QE_API double MathATan2d(double y,double x);
QE_API double MathTand(double x);
QE_API float MathTanf(float x);
QE_API void M9Invf(float *dst,float *src);
QE_API void M9Invd(double *dst,double *src);
QE_API void M12Invf(float *dst,float *src);
QE_API void M12Invd(double *dst,double *src);
QE_API void M16Invf(float *dst,float *src);
QE_API int VecNormalized(double *vec);
QE_API int VecNormalizef(float *vec);
QE_API void M12BuildTf(float *mat,float x,float y,float z);
QE_API void M12BuildRTf(float *mat,float xa,float ya,float za,
   float x,float y,float z);
QE_API void M12BuildRTSf(float *mat,float xa,float ya,float za,
   float x,float y,float z,float sx,float sy,float sz);

// OpenGL format
QE_API void M9Buildf(float *m9,float *scale,float *rot);
QE_API void M12Buildf(float *m12,float *scale,float *rot,float *trans);

QE_API float MathCosSinf(float rad,float *sinp);
QE_API double MathCosSind(double rad,double *sinp);
QE_API float MathDistFastf(float *xyz0,float *xyz1);
QE_API float MathDistf(float *xyz0,float *xyz1);
QE_API float VecLenf(float *xyz);
QE_API float VecLenFastf(float *xyz);
QE_API float MathLineMinDist(float *xyzA1,float *xyzA2,
   float *xyzB1,float *xyzB2);

// find dst given parameter t (0..1) between fr and to
// pass pointers to 4 xyz: bf=before, fr=from, to=to, af=after
QE_API int MathSplineCatmullRom(float *dst,float t,
   float *bf,float *fr,float *to,float *af);

QE_API void QuatFromAngf(float *dst,float rad,float x,float y,float z);
QE_API void M9FromQuatf(float *dst,float *quat);
QE_API void M16FromQuatf(float *dst,float *quat);
QE_API void QuatMulf(float *dst,float *qCur,float *qCat);

QE_API void VecRotXAf(float *dst,float rad,float *src);
QE_API void VecRotXAd(double *dst,double rad,double *src);
QE_API void VecRotYAf(float *dst,float rad,float *src);
QE_API void VecRotYAd(double *dst,double rad,double *src);
QE_API void VecRotZAf(float *dst,float rad,float *src);
QE_API void VecRotZAd(double *dst,double rad,double *src);

QE_API int PlaneEqf(float *nrmk,float *xyz0,float *xyz1,float *xyz2);
QE_API int VertNormalf(float *nrm,float *xyz0,float *xyz1,float *xyz2);

#define VecRotM9(_d_,_v_,_m_) \
   (_d_)[0]=(_v_)[0]*(_m_)[M9_11]+(_v_)[1]*(_m_)[M9_21] \
   +(_v_)[2]*(_m_)[M9_31], \
   (_d_)[1]=(_v_)[0]*(_m_)[M9_12]+(_v_)[1]*(_m_)[M9_22] \
   +(_v_)[2]*(_m_)[M9_32], \
   (_d_)[2]=(_v_)[0]*(_m_)[M9_13]+(_v_)[1]*(_m_)[M9_23] \
   +(_v_)[2]*(_m_)[M9_33]

#define VecRotM12(_d_,_v_,_m_) \
   (_d_)[0]=(_v_)[0]*(_m_)[M12_11]+(_v_)[1]*(_m_)[M12_21] \
   +(_v_)[2]*(_m_)[M12_31], \
   (_d_)[1]=(_v_)[0]*(_m_)[M12_12]+(_v_)[1]*(_m_)[M12_22] \
   +(_v_)[2]*(_m_)[M12_32], \
   (_d_)[2]=(_v_)[0]*(_m_)[M12_13]+(_v_)[1]*(_m_)[M12_23] \
   +(_v_)[2]*(_m_)[M12_33]

#define VecwRotM12(_d_,_v_,_w_,_m_) \
   (_d_)[0]=(_w_)*((_v_)[0]*(_m_)[M12_11]+(_v_)[1]*(_m_)[M12_21] \
   +(_v_)[2]*(_m_)[M12_31]), \
   (_d_)[1]=(_w_)*((_v_)[0]*(_m_)[M12_12]+(_v_)[1]*(_m_)[M12_22] \
   +(_v_)[2]*(_m_)[M12_32]), \
   (_d_)[2]=(_w_)*((_v_)[0]*(_m_)[M12_13]+(_v_)[1]*(_m_)[M12_23] \
   +(_v_)[2]*(_m_)[M12_33])

#define VecwAddRotM12(_d_,_v_,_w_,_m_) \
   (_d_)[0]+=(_w_)*((_v_)[0]*(_m_)[M12_11]+(_v_)[1]*(_m_)[M12_21] \
   +(_v_)[2]*(_m_)[M12_31]), \
   (_d_)[1]+=(_w_)*((_v_)[0]*(_m_)[M12_12]+(_v_)[1]*(_m_)[M12_22] \
   +(_v_)[2]*(_m_)[M12_32]), \
   (_d_)[2]+=(_w_)*((_v_)[0]*(_m_)[M12_13]+(_v_)[1]*(_m_)[M12_23] \
   +(_v_)[2]*(_m_)[M12_33])

#define VecRotM16(_d_,_v_,_m_) \
   (_d_)[0]=(_v_)[0]*(_m_)[M16_11]+(_v_)[1]*(_m_)[M16_21] \
   +(_v_)[2]*(_m_)[M16_31], \
   (_d_)[1]=(_v_)[0]*(_m_)[M16_12]+(_v_)[1]*(_m_)[M16_22] \
   +(_v_)[2]*(_m_)[M16_32], \
   (_d_)[2]=(_v_)[0]*(_m_)[M16_13]+(_v_)[1]*(_m_)[M16_23] \
   +(_v_)[2]*(_m_)[M16_33]

#define VecTransformM12(_d_,_v_,_m_) \
   (_d_)[0]=(_v_)[0]*(_m_)[M12_11]+(_v_)[1]*(_m_)[M12_21] \
   +(_v_)[2]*(_m_)[M12_31]+(_m_)[M12_X], \
   (_d_)[1]=(_v_)[0]*(_m_)[M12_12]+(_v_)[1]*(_m_)[M12_22] \
   +(_v_)[2]*(_m_)[M12_32]+(_m_)[M12_Y], \
   (_d_)[2]=(_v_)[0]*(_m_)[M12_13]+(_v_)[1]*(_m_)[M12_23] \
   +(_v_)[2]*(_m_)[M12_33]+(_m_)[M12_Z]

#define VecwTransformM12(_d_,_v_,_w_,_m_) \
   (_d_)[0]=(_w_)*((_v_)[0]*(_m_)[M12_11]+(_v_)[1]*(_m_)[M12_21] \
   +(_v_)[2]*(_m_)[M12_31]+(_m_)[M12_X]), \
   (_d_)[1]=(_w_)*((_v_)[0]*(_m_)[M12_12]+(_v_)[1]*(_m_)[M12_22] \
   +(_v_)[2]*(_m_)[M12_32]+(_m_)[M12_Y]), \
   (_d_)[2]=(_w_)*((_v_)[0]*(_m_)[M12_13]+(_v_)[1]*(_m_)[M12_23] \
   +(_v_)[2]*(_m_)[M12_33]+(_m_)[M12_Z])

#define VecwAddTransformM12(_d_,_v_,_w_,_m_) \
   (_d_)[0]+=(_w_)*((_v_)[0]*(_m_)[M12_11]+(_v_)[1]*(_m_)[M12_21] \
   +(_v_)[2]*(_m_)[M12_31]+(_m_)[M12_X]), \
   (_d_)[1]+=(_w_)*((_v_)[0]*(_m_)[M12_12]+(_v_)[1]*(_m_)[M12_22] \
   +(_v_)[2]*(_m_)[M12_32]+(_m_)[M12_Y]), \
   (_d_)[2]+=(_w_)*((_v_)[0]*(_m_)[M12_13]+(_v_)[1]*(_m_)[M12_23] \
   +(_v_)[2]*(_m_)[M12_33]+(_m_)[M12_Z])

#define VecTransformM16(_d_,_v_,_m_) \
   (_d_)[0]=(_v_)[0]*(_m_)[M16_11]+(_v_)[1]*(_m_)[M16_21] \
   +(_v_)[2]*(_m_)[M16_31]+(_m_)[M16_X], \
   (_d_)[1]=(_v_)[0]*(_m_)[M16_12]+(_v_)[1]*(_m_)[M16_22] \
   +(_v_)[2]*(_m_)[M16_32]+(_m_)[M16_Y], \
   (_d_)[2]=(_v_)[0]*(_m_)[M16_13]+(_v_)[1]*(_m_)[M16_23] \
   +(_v_)[2]*(_m_)[M16_33]+(_m_)[M16_Z]

#define VecCross(_d_,_a_,_b_) \
   (_d_)[0]=(_a_)[1]*(_b_)[2]-(_a_)[2]*(_b_)[1], \
   (_d_)[1]=(_a_)[2]*(_b_)[0]-(_a_)[0]*(_b_)[2], \
   (_d_)[2]=(_a_)[0]*(_b_)[1]-(_a_)[1]*(_b_)[0]

#define VecDot(_a_,_b_) \
   (_a_)[0]*(_b_)[0]+(_a_)[1]*(_b_)[1]+(_a_)[2]*(_b_)[2]

#define VecSub(_d_,_a_,_b_) \
   (_d_)[0]=(_a_)[0]-(_b_)[0],(_d_)[1]=(_a_)[1]-(_b_)[1],(_d_)[2]=(_a_)[2]-(_b_)[2]

#define XYSub(_d_,_a_,_b_) \
   (_d_)[0]=(_a_)[0]-(_b_)[0],(_d_)[1]=(_a_)[1]-(_b_)[1]

#define VecAdd(_d_,_a_,_b_) \
   (_d_)[0]=(_a_)[0]+(_b_)[0],(_d_)[1]=(_a_)[1]+(_b_)[1],(_d_)[2]=(_a_)[2]+(_b_)[2]

#define XYAdd(_d_,_a_,_b_) \
   (_d_)[0]=(_a_)[0]+(_b_)[0],(_d_)[1]=(_a_)[1]+(_b_)[1]

#define M16FromM12(_dst_,_m12_) (_dst_)[M16_14]=(_dst_)[M16_24]= \
   (_dst_)[M16_34]=0,(_dst_)[M16_44]=1,(_dst_)[M16_11]=(_m12_)[M12_11], \
   (_dst_)[M16_21]=(_m12_)[M12_21],(_dst_)[M16_31]=(_m12_)[M12_31], \
   (_dst_)[M16_12]=(_m12_)[M12_12],(_dst_)[M16_22]=(_m12_)[M12_22], \
   (_dst_)[M16_32]=(_m12_)[M12_32],(_dst_)[M16_13]=(_m12_)[M12_13], \
   (_dst_)[M16_23]=(_m12_)[M12_23],(_dst_)[M16_33]=(_m12_)[M12_33], \
   (_dst_)[M16_X]=(_m12_)[M12_X],(_dst_)[M16_Y]=(_m12_)[M12_Y], \
   (_dst_)[M16_Z]=(_m12_)[M12_Z]

#define M16FromM9(_dst_,_src_) (_dst_)[M16_14]=(_dst_)[M16_24]= \
   (_dst_)[M16_34]=0,(_dst_)[M16_44]=1,(_dst_)[M16_11]=(_src_)[M9_11], \
   (_dst_)[M16_21]=(_src_)[M9_21],(_dst_)[M16_31]=(_src_)[M9_31], \
   (_dst_)[M16_12]=(_src_)[M9_12],(_dst_)[M16_22]=(_src_)[M9_22], \
   (_dst_)[M16_32]=(_src_)[M9_32],(_dst_)[M16_13]=(_src_)[M9_13], \
   (_dst_)[M16_23]=(_src_)[M9_23],(_dst_)[M16_33]=(_src_)[M9_33], \
   (_dst_)[M16_X]=(_dst_)[M16_Y]=(_dst_)[M16_Z]=0

#define M16PartM9(_dst_,_src_) \
   (_dst_)[M16_11]=(_src_)[M9_11], \
   (_dst_)[M16_21]=(_src_)[M9_21],(_dst_)[M16_31]=(_src_)[M9_31], \
   (_dst_)[M16_12]=(_src_)[M9_12],(_dst_)[M16_22]=(_src_)[M9_22], \
   (_dst_)[M16_32]=(_src_)[M9_32],(_dst_)[M16_13]=(_src_)[M9_13], \
   (_dst_)[M16_23]=(_src_)[M9_23],(_dst_)[M16_33]=(_src_)[M9_33]

#define M9FromM16(_dst_,_src_) \
   (_dst_)[M9_11]=(_src_)[M16_11], \
   (_dst_)[M9_21]=(_src_)[M16_21],(_dst_)[M9_31]=(_src_)[M16_31], \
   (_dst_)[M9_12]=(_src_)[M16_12],(_dst_)[M9_22]=(_src_)[M16_22], \
   (_dst_)[M9_32]=(_src_)[M16_32],(_dst_)[M9_13]=(_src_)[M16_13], \
   (_dst_)[M9_23]=(_src_)[M16_23],(_dst_)[M9_33]=(_src_)[M16_33]

#define M12FromM16(_vm_,_m_) (_vm_)[M12_X]=(_m_)[M16_X], \
   (_vm_)[M12_Y]=(_m_)[M16_Y],(_vm_)[M12_Z]=(_m_)[M16_Z], \
   (_vm_)[M12_11]=(_m_)[M16_11],(_vm_)[M12_21]=(_m_)[M16_21], \
   (_vm_)[M12_31]=(_m_)[M16_31],(_vm_)[M12_12]=(_m_)[M16_12], \
   (_vm_)[M12_22]=(_m_)[M16_22],(_vm_)[M12_32]=(_m_)[M16_32], \
   (_vm_)[M12_13]=(_m_)[M16_13],(_vm_)[M12_23]=(_m_)[M16_23], \
   (_vm_)[M12_33]=(_m_)[M16_33]

#define M9Id(_m_) (_m_)[M9_11]=(_m_)[M9_22]= \
   (_m_)[M9_33]=1,(_m_)[M9_21]=(_m_)[M9_31]= \
   (_m_)[M9_12]=(_m_)[M9_32]=(_m_)[M9_13]=(_m_)[M9_23]=0

#define M12Id(_m_) (_m_)[M12_11]=(_m_)[M12_22]= \
   (_m_)[M12_33]=1,(_m_)[M12_21]=(_m_)[M12_31]= \
   (_m_)[M12_12]=(_m_)[M12_32]=(_m_)[M12_13]=(_m_)[M12_23]= \
   (_m_)[M12_X]=(_m_)[M12_Y]=(_m_)[M12_Z]=0

#define M12BuildT(_m_,_x_,_y_,_z_) (_m_)[M12_11]=(_m_)[M12_22]= \
   (_m_)[M12_33]=1,(_m_)[M12_21]=(_m_)[M12_31]= \
   (_m_)[M12_12]=(_m_)[M12_32]=(_m_)[M12_13]=(_m_)[M12_23]= \
   (_m_)[M12_X]=(_x_),(_m_)[M12_Y]=(_y_),(_m_)[M12_Z]=(_z_)

#define M16Id(_m_) (_m_)[M16_11]=(_m_)[M16_22]= \
   (_m_)[M16_33]=(_m_)[M16_44]=1,(_m_)[M16_21]=(_m_)[M16_31]= \
   (_m_)[M16_14]=(_m_)[M16_12]=(_m_)[M16_32]=(_m_)[M16_24]= \
   (_m_)[M16_13]=(_m_)[M16_23]=(_m_)[M16_34]=(_m_)[M16_41]= \
   (_m_)[M16_42]=(_m_)[M16_43]=0

// JFL 06 Sep 06
#define SWAPT(_a_,_b_,_t_) (_t_)=(_a_),(_a_)=(_b_),(_b_)=(_t_)

// transpose, use a temp var
#define M9Transpose(_m_,_t_) \
   SWAPT((_m_)[M9_21],(_m_)[M9_12],_t_), \
   SWAPT((_m_)[M9_32],(_m_)[M9_23],_t_), \
   SWAPT((_m_)[M9_31],(_m_)[M9_13],_t_)

#define M12Transpose(_m_,_t_) \
   SWAPT((_m_)[M12_21],(_m_)[M12_12],_t_), \
   SWAPT((_m_)[M12_32],(_m_)[M12_23],_t_), \
   SWAPT((_m_)[M12_31],(_m_)[M12_13],_t_)

typedef struct {float m[9];} qeM9f;
#define M9Cpyf(_d_,_s_) *((qeM9f*)(_d_))=*((qeM9f*)(_s_))

typedef struct {double m[9];} qeM9d;
#define M9Cpyd(_d_,_s_) *((qeM9d*)(_d_))=*((qeM9d*)(_s_))

typedef struct {float m[12];} qeM12f;
#define M12Cpyf(_d_,_s_) *((qeM12f*)(_d_))=*((qeM12f*)(_s_))

typedef struct {double m[12];} qeM12d;
#define M12Cpyd(_d_,_s_) *((qeM12d*)(_d_))=*((qeM12d*)(_s_))

typedef struct {float m[16];} qeM16f;
#define M16Cpyf(_d_,_s_) *((qeM16f*)(_d_))=*((qeM16f*)(_s_))

typedef struct {double m[16];} qeM16d;
#define M16Cpyd(_d_,_s_) *((qeM16d*)(_d_))=*((qeM16d*)(_s_))

typedef struct {float m[3];} qeVecf;
#define VecCpyf(_d_,_s_) *((qeVecf*)(_d_))=*((qeVecf*)(_s_))

typedef struct {double m[3];} qeVecd;
#define VecCpyd(_d_,_s_) *((qeVecd*)(_d_))=*((qeVecd*)(_s_))

#define QuatId(_d_) (_d_)[0]=(_d_)[1]=(_d_)[2]=0,(_d_)[3]=1
QE_API void QuatFromAngf(float *dst,float rad,float x,float y,float z);
QE_API void QuatAngf(float *dst,float xa,float ya,float za);
QE_API void M9FromQuatf(float *m,float *quat);
QE_API void QuatMulf(float *dst,float *qCur,float *qCat);
QE_API void QuatSlerpf(float *dst,float *from, float *to, float t);

///////////////////////////////////////////////////////////////////////////////
// VARIABLE ARGUMENT

typedef uns8 *vargs;
#define VARG_SIZEOF(n) ((sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1))
#define varg_start(ap,v) (ap = (vargs)&v + VARG_SIZEOF(v))
#define varg_local(ap,v) (ap = (vargs)&v)
#define varg_next(ap,t) (*(t *)((ap += VARG_SIZEOF(t)) - VARG_SIZEOF(t)))
#define varg_end(x)

///////////////////////////////////////////////////////////////////////////////
// ERROR CODES

enum {
   // - add entry to qeErrStrTable[]

	ERR_MEM_ALLOC=-1099,

	ERR_FS_OPEN=-1199,
	ERR_FS_READ,
	ERR_FS_WRITE,
	ERR_FS_INVALID,
	ERR_FS_SEEK,
	ERR_FS_NOTFOUND,
	ERR_FS_EOF,

	// general errors
	ERR_SYNTAX=-1299,
	ERR_LOAD,
	ERR_INIT,
	ERR_OVERFLOW,
	ERR_UNDERFLOW,
	ERR_ROOT,
	ERR_LINK,
	ERR_PARAM,
	ERR_SIZE,
	ERR_ABORT,
	ERR_STEP,
	ERR_ONLYIFABORT,
   ERR_ERR_OVERFLOW,

	ERR_APP_NOTHANDLED=-1399,

   ERR_SND_FORMAT=-1499,
   ERR_SND_FILENOTFOUND,
   ERR_SND_FILESCANNING,
   ERR_SND_SYSTEMBUFFER,
   ERR_SND_NOCHANNELAVAIL,

   ERR_IMG_FILENOTFOUND=-1599,
   ERR_IMG_FORMAT,
   ERR_IMG_FILESCANNING,

   ERR_TEX_NOTLOADED=-1550,

   ERR_LIB_FILENOTFOUND=-1699,
   ERR_LIB_FORMAT,
   ERR_LIB_BUFFER,

   ERR_OBJ_OPTIONNOTHANDLED=-1799,

   ERR_INP_INTERNALBUF_OVERLOW=-1899,

   ERR_SHADER_FILENOTFOUND=-1999,

   ERR_GL_UNSUPPORTEDFUNCTION=-2099,

	ERR_SZ_UNDEF,
};


///////////////////////////////////////////////////////////////////////////////
// LNKD

// linked list
typedef struct _dlnode {
   struct _dlnode *next;
   struct _dlnode *prev;
} DLNode;

// a list head
typedef struct {
   DLNode first;
   DLNode last;
} DLHead;

QE_API void DLSetHead(DLHead *h);
QE_API void DLNLinkAfter(DLNode *h,DLNode *n);
QE_API void DLNLinkBefore(DLNode *h,DLNode *n);
QE_API void DLNUnlink(DLNode *n);

#define DLEmpty(_n_) (!((DLNode*)_n_)->next||!((DLNode*)_n_)->prev)
#define DLFirst(_h_) !((DLNode*)_h_)->next||!((DLNode*)_h_)->next->next? \
   (void*)0:(void*)((DLNode*)_h_)->next
#define DLNext(_n_) !((DLNode*)_n_)->next||!((DLNode*)_n_)->next->next? \
   (void*)0:(void*)((DLNode*)_n_)->next
#define DLUnlink(_n_) DLNUnlink((DLNode*)_n_)
#define DLLinkBefore(_h_,_n_) DLNLinkBefore((DLNode*)_h_,(DLNode*)_n_)
#define DLLinkAfter(_h_,_n_) DLNLinkAfter((DLNode*)_h_,(DLNode*)_n_)
#define DLLinkTail(_h_,_n_) DLNLinkBefore(((DLNode*)_h_)+1,(DLNode*)_n_)

///////////////////////////////////////////////////////////////////////////////

enum {
   QEOBJTYPE_NONE,
   QEOBJTYPE_TMP,
   QEOBJTYPE_LIB,
   QEOBJTYPE_INST,
}; // QEOBJTYPE_

typedef struct {
   uns32 typeC4; // type
   chr *name; // name
   uns16 nameSize; // size of name buffer
   uns16 flags; // flags
   ptr p; // lib/def data
} qeObjInst;

typedef struct {
   chr *name; // name
   uns16 nameSize; // size of name buffer
   uns16 flags; // flags
   ptr p; // lib/def data
} qeObjLib;

#define M_QEOBJ_HASCHILD 0x0001 // has a child
#define M_QEOBJ_SKIP     0x2000 // don't visit

typedef uns qeObj; // dummy type for qeObj
#define QEOBJ_REC(_o_) qeObjNode(_o_) // depricated, use qeObjNode() directly
#define QEOBJ_INST(_o_) qeObjNode(_o_) // depricated, use qeObjNode() directly

QE_API uns qeObjType(qeObj *o); // get type
QE_API uns qeObjFlags(qeObj *o); // get flags
QE_API chr* qeObjName(qeObj *o); // get name

QE_API int qeObjNew(qeObj **op,uns type,uns size,uns flags,chr *name);
QE_API void qeObjZap(qeObj *o); // immediately free all resources
QE_API void qeObjDelete(qeObj *o); // free all resources
QE_API qeObj* qeObjEngineCur(uns op); // get current engine object
#define qeObjCur qeObjEngineCur // depricated, use qeObjEngineCur()

QE_API void qeObjHead(qeObj *o);
QE_API void qeObjLinkAfter(qeObj *h,qeObj *o);
QE_API void qeObjLinkBefore(qeObj *h,qeObj *o);
QE_API void qeObjUnlink(qeObj *o);

QE_API qeObj* qeObjNext(qeObj *o); // next
QE_API qeObj* qeObjPrev(qeObj *o); // prev
QE_API qeObj *qeObjNextLoop(qeObj *o); // next, loop
QE_API qeObj *qeObjPrevLoop(qeObj *o); // prev, loop
QE_API qeObj* qeObjChild(qeObj *o); // child
QE_API qeObj* qeObjParent(qeObj *o); // parent

QE_API ptr qeObjExt(qeObj *o); // get ext or inst data area (type dependent)
QE_API ptr qeObjNode(qeObj *o); // get node (normally, use qeObjExt())
QE_API qeObj *qeInstObj(ptr inst);

///////////////////////////////////////////////////////////////////////////////
// STRINGZ

// string routines follow these rules:
// - buffer building functions guarantee a zero-terminator (size > 0)
//
// parameter naming conventions
// - 'ss' points to the string start, i.e. the first chr in the string
// - 'sx' points to the string terminator, i.e. one past the last chr
//   a null pointer can always be passed in for zero-terminated strings
// - 'ts' and 'tx' are used for the second string
// - pass the byte size of the whole buffer
//
// flag table:
// uns32 flagtable[0] = number of entries in table
//       flagtable[1] = flags for character 1 ..

#define M_SZF_PREINC 0x10000000  // not in table -- signals pre inc
QE_API int szsize(chr *ss); // byte size of string and terminator 
QE_API int szsize2(chr *ss,chr *sx);
QE_API int szlen(chr *ss); // number of chrs in string
QE_API int szlen2(chr *ss,chr *sx);
QE_API int szeos(chr *ss,chr *sx); // end of string test, >0 = eos
QE_API int szcpy(chr *dst,int dstsize,chr *ss,chr *sx);
QE_API int szcat(chr *dst,int dstsize,chr *ss,chr *sx);
QE_API int szcpycat(chr *dst,int dstsize,chr *ss,chr *sx,chr *ts,chr *tx);
QE_API int szcmp(chr *ss,chr *ts,chr *tx);
QE_API int szcmp2(chr *ss,chr *sx,chr *ts,chr *tx);
QE_API int szicmp(chr *ss,chr *ts,chr *tx); // case insensitive
QE_API int szicmp2(chr *ss,chr *sx,chr *ts,chr *tx);
QE_API int szstart(chr *ss,chr *ts,chr *tx); // does ss start with ts?
QE_API int szstart2(chr *ss,chr *sx,chr *ts,chr *tx);
QE_API int szistart(chr *ss,chr *ts,chr *tx); // case insensitive
QE_API int szistart2(chr *ss,chr *sx,chr *ts,chr *tx);
QE_API chr* szchr(chr *ss,chr *sx,chr c);
QE_API chr* szrchr(chr *ss,chr *sx,chr c);
QE_API chr* szsub(chr *sub,chr *subx,chr *ts,chr *tx); // find first sub
QE_API chr* szisub(chr *sub,chr *subx,chr *ts,chr *tx); // case insensitive
QE_API chr* szfskip(int *flagtable,chr *ss,chr *sx,int flags);
QE_API chr* szftill(int *flagtable,chr *ss,chr *sx,int flags);
QE_API chr* sztobin(chr *ss,chr *sx,int base,chr dsttype,void *dst);
QE_API int szmap(chr *table[],chr *ss,chr *sx);
QE_API int szimap(chr *table[],chr *ss,chr *sx);
QE_API int szfmt(chr *dst,int dstsize,chr *fmt,...);
QE_API int szfmt2(chr *dst,int dstsize,chr *fmt,chr *fmtx,...);


#if 0 // strings
// SAFE STRINGS
// guaranteed 0 string termination
// pass in buffer size
// string start & stop or 0 terminated
// flag table:
// uns32 flagtable[0] = number of entries in table
//       flagtable[1] = flags for character 1 ..
#define M_SZF_PREINC 0x10000000  // not in table -- signals pre inc
QE_API int szsize(chr *ss); // byte size of string and terminator 
QE_API int szsize2(chr *ss,chr *sx);
QE_API int szlen(chr *ss); // number of chrs in string
QE_API int szlen2(chr *ss,chr *sx);
QE_API int szeos(chr *ss,chr *sx); // end of string test, >0 = eos
QE_API int szcpy(chr *dst,int dstsize,chr *ss,chr *sx);
QE_API int szcat(chr *dst,int dstsize,chr *ss,chr *sx);
QE_API int szcpycat(chr *dst,int dstsize,chr *ss,chr *sx,chr *ts,chr *tx);
QE_API int szcmp(chr *ss,chr *ts,chr *tx);
QE_API int szcmp2(chr *ss,chr *sx,chr *ts,chr *tx);
QE_API int szicmp(chr *ss,chr *ts,chr *tx); // case insensitive
QE_API int szicmp2(chr *ss,chr *sx,chr *ts,chr *tx);
QE_API int szstart(chr *ss,chr *ts,chr *tx); // does ss start with ts?
QE_API int szstart2(chr *ss,chr *sx,chr *ts,chr *tx);
QE_API int szistart(chr *ss,chr *ts,chr *tx); // case insensitive
QE_API int szistart2(chr *ss,chr *sx,chr *ts,chr *tx);
QE_API chr* szchr(chr *ss,chr *sx,chr c);
QE_API chr* szrchr(chr *ss,chr *sx,chr c);
QE_API chr* szsub(chr *sub,chr *subx,chr *ts,chr *tx); // find first sub
QE_API chr* szisub(chr *sub,chr *subx,chr *ts,chr *tx); // case insensitive
QE_API chr* szfskip(uns32 *flagtable,chr *ss,chr *sx,uns32 flags);
QE_API chr* szftill(uns32 *flagtable,chr *ss,chr *sx,uns32 flags);
QE_API chr* sztobin(chr *ss,chr *sx,int base,chr dsttype,void *dst);
QE_API int szmap(chr *table[],chr *ss,chr *sx);
QE_API int szimap(chr *table[],chr *ss,chr *sx);
QE_API int szfmt(chr *dst,int dstsize,chr *fmt,...);
QE_API int szfmt2(chr *dst,int dstsize,chr *fmt,chr *fmtx,...);
#endif // stringz

QE_API int C4Cmp(uns match,uns test);

///////////////////////////////////////////////////////////////////////////////
// FILE

#define M_FILEREC_OPEN        0x0001
#define M_FILEREC_ENDIAN_SWAP 0x1000
#define M_FILEREC_ERROR       0x8000
typedef struct {
   uns flags;
   ptr fh;
   uns priv[4];
} FileRec;

QE_API int FilePathFind(chr *fpath,chr *bigBuf,uns bigBufSize);
QE_API int FileLogOutput(chr *buf,uns len);
QE_API int FileLogCheckToDelete(uns toobig);
QE_API int FileSize(chr *path);
QE_API int FileOpen(FileRec *fr,chr *fpath,uns flags);
QE_API void FileClose(FileRec *fr);
QE_API int FileRead(FileRec *fr,ptr buf,uns size);
QE_API int FileSeek(FileRec *fr,uns off);
QE_API int FileSkip(FileRec *fr,int off);
QE_API int FileTell(FileRec *fr,uns *offp);
QE_API uns32 FileR32U(FileRec *fr);
QE_API uns16 FileR16U(FileRec *fr);
QE_API int FileDelete(chr *fpath);


#ifdef __cplusplus
}
#endif

#endif // QEBASE_H
