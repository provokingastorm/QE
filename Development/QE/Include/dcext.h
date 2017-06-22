#ifndef DCEXT_H
#define DCEXT_H
// Copyright (C) 2006-2007, Joe Linhoff
// dcext.h

#if !BUILD_MSVC && !BUILD_GCC
#error -- set BUILD_MSVC=1 in proprocessor defines
#endif // BUILD_

#ifndef JOECO_H
#define JOECO_H

/////////////
// BASE TYPES

typedef unsigned char chr;
typedef signed char int8;
typedef unsigned char uns8;
typedef signed short int16;
typedef unsigned short uns16;
typedef signed int int32;
typedef unsigned int uns32;
typedef unsigned int uns;
typedef float flt;
typedef float flt32;
typedef double dbl;
typedef void* vpt;
typedef void* ptr;
typedef unsigned char bit;
typedef int32 off32;

#if BUILD_MSVC
typedef __int64 int64;
typedef unsigned __int64 uns64;
#endif // BUILD_MSVC

#if BUILD_GCC
typedef signed long long int64;
typedef unsigned long long uns64;
#endif // BUILD_GCC

typedef uns64 sto; // large enough to hold: int,uns,flt,dbl,vpt,crd

///////////////
// BASE DEFINES

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

//////////////////////////
// SYSTEM MACROS & DEFINES

#if DEBUG

	#if BUILD_MSVC
	extern bit main_brk; // add to your main.c
	#define BRK() {if(main_brk) __asm{int 3};}
	#endif // BUILD_MSVC

	#if BUILD_GCC
	#define BRK() 
	#endif // BUILD_GCC

#else // !DEBUG

	#define BRK()

#endif // !DEBUG

#define PI 3.14159265358979323846264338327950288419716939937510

#define DEGTORAD(_deg_) (((_deg_)*PI)/180.0)
#define RADTODEG(_rad_) (((_rad_)*180.0)/PI)

#define NUL ((void*)0)
#define ret(_x_) {r=_x_;goto BAIL;}
#define bret(_x_) {BRK();r=_x_;goto BAIL;}
#define NUM(_x_) (sizeof(_x_)/sizeof(_x_[0]))
#define ALIGN(_n_,_m_) (((_n_)+(_m_))&~(_m_))

#define PTR_ADD(_p_,_i_) (void*)(((uns8*)(_p_))+(_i_))
#define PTR_SUB(_p_,_i_) (void*)(((uns8*)(_p_))-(_i_))
#define PTR_DIFF(_p1_,_p2_) ((uns8*)(_p1_)-(uns8*)(_p2_))
#define PTR_ALIGN(_p_,_m_) (((uns8*)0)+(((uns)(_p_)+(_m_))&~(_m_)))

#define C1(_a_) (_a_)
#define C2(_a_,_b_) ((_b_)<<8|(_a_))
#define C3(_a_,_b_,_c_) ((_c_)<<16|(_b_)<<8|(_a_))
#define C4(_a_,_b_,_c_,_d_) ((_d_)<<24|(_c_)<<16|(_b_)<<8|(_a_))

#if BUILD_MSVC
#pragma warning(disable:4018) // '==' : signed/unsigned mismatch
#pragma warning(disable:4133) // incompatible types - from 'uno *__w64 ' to 'xx *'
#pragma warning(disable:4244) // '=' : conversion from '__w64 int' to 'int', possible loss of data
#pragma warning(disable:4305) // '=' : truncation from 'double' to
#pragma warning(disable:4311) // 'type cast' : pointer truncation from 'vpt' to 'int'
#pragma warning(disable:4761) // integral size mismatch in argument; conversion supplied
#endif // BUILD_MSVC

#define SET3(_d_,_s1_,_s2_,_s3_) (_d_)[0]=(_s1_), \
   (_d_)[1]=(_s2_),(_d_)[2]=(_s3_)
#define CPY3(_d_,_s_) (_d_)[0]=(_s_)[0],(_d_)[1]=(_s_)[1],(_d_)[2]=(_s_)[2]
#define CPY4(_d_,_s_) (_d_)[0]=(_s_)[0],(_d_)[1]=(_s_)[1],(_d_)[2]=(_s_)[2],(_d_)[3]=(_s_)[3]

#endif // ndef JOECO_H

typedef struct UNO_struct 
{
	struct UNO_struct *p; // prev
	struct UNO_struct *n; // next
	chr sname[12]; // short name
	uns16 t; // type
	uns16 f; // flags
} UNO;

///////////////////////////////////////////////////////////////////////////////
// SZ

#define M_SZ_ALPHA    0x00000001
#define M_SZ_NUM      0x00000002
#define M_SZ_WHITE    0x00000004
#define M_SZ_MODNUM   0x00000008
#define M_SZ_ID1      0x00000010

///////////////////////////////////////////////////////////////////////////////
// DB

#define DBTYPE_NONE 0
#define DBTYPE_FILE C4('f','l','l','e')
#define DBTYPE_MSH1 C4('m','s','h','1')
#define DBTYPE_ANI1 C4('a','n','i','1')
#define DBTYPE_ANI2 C4('a','n','i','2')
#define DBTYPE_CAM1 C4('c','a','m','1')
#define DBTYPE_LIT1 C4('l','i','t','1')
#define DBTYPE_SKL1 C4('s','k','l','1')
#define DBTYPE_SKN1 C4('s','k','n','1')
#define DBTYPE_POS1 C4('p','o','s','1')
#define DBTYPE_MTR1 C4('m','t','r','1')

typedef struct {
   UNO l;
   uns type;
   uns id;
   chr *name; // ptr to long name
   ptr rec; // ptr to record
} DBObj;

#define M_DBMESH1_NOTVISIBLE 0x0002
#define M_DBMESH1_DONTLIGHT  0x0004
#define M_DBMESH1_HARDEDGES  0x0008
#define M_DBMESH1_SOFTEDGES  0x0010
typedef struct {
   chr *name;
   uns flags;
   uns numv;
   uns numf;
   uns numfv;
   float *verts;
   int *faces;
   float *normals;
   float *uvs;
   chr *imgPath;
   uns imgPathSize;
   float trans[3];
   float rot[3]; // radians
   float scale[3];
   float mat12[12];
   chr *note;
   uns noteSize;
} DBMesh1;

#define M_DBNEWMESH1_UVS 0x0001
#define M_DBNEWMESH1_NRM 0x0002

typedef struct {
   ptr priv[64];
} DBNextRec;

enum {
   DBIMGTYPE_NONE,
   DBIMGTYPE_RGB,
   DBIMGTYPE_RGBA,
};

typedef struct {
   UNO lnk;
   chr *name;
   chr *path;
   uns16 flags;
   uns16 texFlags;
   uns width;
   uns height;
   uns rowBytes;
   uns8 bytesPerPixel;
   uns8 *pix;
} DBImg;

#define M_DBONEKEY_V  0x1000
#define M_DBONEKEY_TX 0x0001
#define M_DBONEKEY_TY 0x0002
#define M_DBONEKEY_TZ 0x0004
#define M_DBONEKEY_RX 0x0010
#define M_DBONEKEY_RY 0x0020
#define M_DBONEKEY_RZ 0x0040
#define M_DBONEKEY_SX 0x0100
#define M_DBONEKEY_SY 0x0200
#define M_DBONEKEY_SZ 0x0400
typedef struct {
   uns flags;
   float time;
   float trans[3];
   float rot[3];
   float scale[3];
} DBOneKey;

#define M_DBANIM1_LOOP      0x0001
#define M_DBANIM1_DONTLOOP  0x0002
#define M_DBANIM1_WRITTEN 0x1000
typedef struct {
   chr *name;
   uns flags;
   uns numk;
   uns keyFlags;
   DBOneKey *keys;
} DBAnim1;

typedef struct {
   chr *name;
   uns flags;
   uns num;
   chr **animNames;
   ptr extra;
} DBAnim2;

#define M_DBCAM1_ORTHO 0x0001
typedef struct {
   chr *name;
   uns flags;
   float focalLen;
   float centerOfInterest;
   float nearClip;
   float farClip;
   float orthoWidth;
   float wdivh;
   float trans[3];
   float rot[3];
   chr *note;
   uns noteSize;
} DBCam1;

#define DBLIGHTTYPE_AMBIENT C4('a','m','b',' ')
#define DBLIGHTTYPE_DIRECTIONAL C4('d','i','r',' ')
#define DBLIGHTTYPE_POINT C4('p','n','t',' ')
#define DBLIGHTTYPE_SPOT C4('s','p','t',' ')

enum {
   DBLIGHT_DECAY_NONE,
   DBLIGHT_DECAY_LINEAR,
   DBLIGHT_DECAY_QUADRATIC,
   DBLIGHT_DECAY_CUBIC,
};

typedef struct {
   float ambientShade;
} DBLightAmbient;

typedef struct {
   float rot[3];
} DBLightDirectional;

typedef struct {
   float trans[3];
   int decay;
} DBLightPoint;

typedef struct {
   float rot[3];
   float trans[3];
   int decay;
   float coneAng;
   float penumbraAng;
   float dropoff;
} DBLightSpot;

#define M_DBLIGHT1_DIFFUSE  0x0001
#define M_DBLIGHT1_SPECULAR 0x0002
typedef struct {
   chr *name;
   uns flags;
   uns type;
   float color4[4];
   float intensity;
   union {
      DBLightAmbient amb;
      DBLightDirectional dir;
      DBLightPoint pnt;
      DBLightSpot spt;
   } u;
   chr *note;
   uns noteSize;
} DBLight1;

typedef struct _dbonejoint {
   chr *name;
   uns nameSize;
   struct _dbonejoint *parent;
   float orient[3];
   float trans[3];
   float rot[3];
   float scale[3];
   float m12Bind[12];
   float m12Rel[12];
} DBOneJoint;

typedef struct {
   chr *name;
   uns numj;
   DBOneJoint *joints;
   uns jointNameSize;
   chr *note;
   uns noteSize;
} DBSkel1;

typedef struct {
   chr *name;
   uns nameSize;
   float m12BindPre[12];
   int16 matIndex;
   uns16 xx1;
   chr *jointName;
   uns jointNameSize;
} DBOneBind;

typedef struct {
   chr *name;
   uns numb;
   uns numv;
   DBOneBind *binds;
   float *weights;
   uns jointNameSize;
} DBSkin1;

typedef struct {
   chr *name;
   chr *copyofName;
   uns copyofNameSize;
   float trans[3];
   float rot[3];
   float scale[3];
   chr *note;
   uns noteSize;
} DBPos1;

typedef struct {
   float diffuse;
   float ambientColor[3];
   float transparency[3];
   float incandescence[3];
} DBMaterialPhong;

#define DBMATERIALTYPE_PHONG C4('p','h','n','g')
#define DBMATERIALTYPE_LAMBERT C4('l','m','b','t')
#define DBMATERIALTYPE_BLINN C4('b','l','n','n')
typedef struct {
   chr *name;
   uns flags;
   uns mtrType;
   union {
      DBMaterialPhong phong;
   } u;
   chr *note;
   uns noteSize;
} DBMaterial1;

///////////////////////////////////////////////////////////////////////////////
//

#define DCINTREC_MAGIC   0x5267C293

typedef struct {
   int magic;
   int DCIntSize;
   int vers;

   // SZ
   int (*szlen)(chr *s1);
   int (*szcpy)(chr *dst,int dstsize,chr *s1,chr *s1x);
   int (*szcmp)(chr *s1,chr *s2,chr *s2x);
   int (*szicmp)(chr *s1,chr *s2,chr *s2x);
   chr* (*szsub)(chr *s1,chr *s1x,chr *sub,chr *subx);
   int (*szmap)(chr *table[],chr *s1,chr *s1x);
   int (*szimap)(chr *table[],chr *s1,chr *s1x);
   chr* (*szchr)(chr *s1,chr *s1x,chr c);
   chr* (*szrchr)(chr *s1,chr *s1x,chr c);
   chr* (*szskip)(chr *s1,chr *s1x,chr *skip);
   chr* (*sztill)(chr *s1,chr *s1x,chr *till);
   chr* (*sztskip)(chr *s1,chr *s1x,uns32 flags);
   chr* (*szttill)(chr *s1,chr *s1x,uns32 flags);
   chr* (*sztobin)(chr *s1,chr *s1x,int base,chr dsttype,void *dst);
   int (*szfmt)(chr *dst,int dstsize,chr *fmt,...);
   int szMagic;

   // MEM
   int (*MemAlloc)(void *memp,uns size);
   int (*MemAlloz)(void *memp,uns size);
   void (*MemFree)(void *mem);
   void (*MemZ)(void *buf,uns size); // zero
   void (*MemC)(void *dst,uns size,void *src);
   void (*Mem8)(void *dst,uns size,uns8 x);
   void (*MemChr)(void *dst,uns size,chr x);
   int memMagic;

   // DATE
   int (*DateUnpackSz)(chr *dst,uns dstSize,uns op,vpt datep);
   int dateMagic;

   // DB
   DBObj* (*DBListHead)(void);
   int dbMagic;

   // UNO
   void (*UNOHead)(UNO *o);
   void (*UNOLinkAfter)(UNO *head,UNO *o);
   void (*UNOLinkBefore)(UNO *head,UNO *o);
   void (*UNOUnlink)(UNO *o);
   int unoMagic;

   // UTIL
   int (*GeoSoftVertNormals)(float *vnormals,float *verts,int numv,
      int *faces,int numf);
   int (*GeoHardVertNormals)(float *vnormals,float *verts,int numv,
      int *faces,int numf);
   int (*GeoHardSoftVertNormals)(float *vnormals,float *verts,int numv,
      int *faces,int numf);
   int (*GeoBreakoutVerts)(float *vnormals,int maxv,float *verts,int numv,
      int *faces,int numf);
   int (*GeoCullXYZR)(float *verts,uns numv,float *xyzr);
   int (*ImgLoad)(DBImg **imgp,chr *name,chr *namex,chr *path,chr *pathx);
   int (*ImgFree)(DBImg *img);
   int (*ToolPrintf)(chr *fmt,...);
   int utilMagic;

   // SCAN
   int (*ScanNextBlock)(chr **dstp,chr **dstxp,chr **outp,chr **outxp,
      chr *src,chr *srcx,chr *tag,chr *tagx);
   int (*ScanRemoveBlocks)(chr *dst,int dstSize,chr *src,chr *srcx,
      chr *tag,chr *tagx);
   int scanMagic;

} DCIntRec;

extern DCIntRec *dci; // pointer to internal record
#define DCI_SZMAP(_t_,_s1_,_sx_) (*dci->szmap)(_t_,_s1_,_sx_)
#define DCI_SZSUB(_s1_,_s1x_,_sub_,_subx_) (*dci->szsub)(_s1_,_s1x_,_sub_,_subx_)
#define DCI_SZLEN(_s1_) (*dci->szlen)(_s1_)
#define DCI_SZLN(_s1_,_sx_) ((_sx_)?PTR_DIFF(_sx_,_s1_):DCI_SZLEN(_s1_))
#define DCI_SZCMP(_s1_,_s2_,_s2x_) (*dci->szcmp)(_s1_,_s2_,_s2x_)
#define DCI_SZICMP(_s1_,_s2_,_s2x_) (*dci->szicmp)(_s1_,_s2_,_s2x_)
#define DCI_SZTSKIP(_s1_,_sx_,_f_) (*dci->sztskip)(_s1_,_sx_,_f_)
#define DCI_SZCHR(_s1_,_sx_,_c_) (*dci->szchr)(_s1_,_sx_,_c_)
#define DCI_SZRCHR(_s1_,_sx_,_c_) (*dci->szrchr)(_s1_,_sx_,_c_)
#define DCI_SZCPY(_d_,_ds_,_s1_,_sx_) (*dci->szcpy)(_d_,_ds_,_s1_,_sx_)

#define DCI_MEMALLOC(_memp_,_size_) (*dci->MemAlloc)(_memp_,_size_)
#define DCI_MEMALLOZ(_memp_,_size_) (*dci->MemAlloz)(_memp_,_size_)
#define DCI_MEMFREE(_mem_) (*dci->MemFree)(_mem_)
#define DCI_MEMZ(_mem_,_size_) (*dci->MemZ)(_mem_,_size_)
#define DCI_MEMC(_dst_,_size_,_src_) (*dci->MemC)(_dst_,_size_,_src_)
#define DCI_MEM8(_dst_,_size_,_val_) (*dci->Mem8)(_dst_,_size_,_val_)
#define DCI_MEMCHR(_dst_,_size_,_val_) (*dci->MemChr)(_dst_,_size_,_val_)

#define DCI_DATEUNPACKSZ (*dci->DateUnpackSz)

#define DCI_DBLISTHEAD() (*dci->DBListHead)()
#define DCI_DBLISTFIRST() (DCI_DBLISTHEAD())->l.n

#define DCI_UNOHEAD(_o_) (*dci->UNOHead)(_o_)
#define DCI_UNOLINKAFTER(_h_,_o_) (*dci->UNOLinkAfter)(_h_,_o_)
#define DCI_UNOLINKBEFORE(_h_,_o_) (*dci->UNOLinkBefore)(_h_,_o_)
#define DCI_UNOUNLINK(_o_) (*dci->UNOUnlink)(_o_)

#define DCI_GEOSOFTNORMALS(_dstnormals_,_verts_,_numv_,_faces_,_numf_) \
   (*dci->GeoSoftVertNormals)(_dstnormals_,_verts_,_numv_,_faces_,_numf_)
#define DCI_GEOHARDNORMALS(_dstnormals_,_verts_,_numv_,_faces_,_numf_) \
   (*dci->GeoHardVertNormals)(_dstnormals_,_verts_,_numv_,_faces_,_numf_)
#define DCI_GEOHARDSOFTNORMALS(_dstnormals_,_verts_,_numv_,_faces_,_numf_) \
   (*dci->GeoHardSoftVertNormals)(_dstnormals_,_verts_,_numv_,_faces_,_numf_)
#define DCI_GEOBREAKOUTVERTS(_dstverts_,_dstnumv_,_verts_,_numv_,_faces_,_numf_) \
   (*dci->GeoBreakoutVerts)(_dstverts_,_dstnumv_,_verts_,_numv_,_faces_,_numf_)
#define DCI_IMGLOAD(_imgp_,_name_,_namex_,_path_,_pathx_) \
   (*dci->ImgLoad)(_imgp_,_name_,_namex_,_path_,_pathx_)
#define DCI_IMGFREE(_img_) (*dci->ImgFree)(_img_)
#define DCI_GEOCULLXYZR(_verts_,_numv_,_xyzr_) \
   (*dci->GeoCullXYZR)(_verts_,_numv_,_xyzr_)

#define DCI_SCANNEXTBLOCK(_dstp_,_dstxp_,_outp_,_outxp_,_src_,_srcx_, \
   _tag_,_tagx_) \
   (*dci->ScanNextBlock)(_dstp_,_dstxp_,_src_,_srcx_,_tag_,_tagx_)
#define DCI_SCANREMOVEBLOCKS(_dst_,_dstSize_,_src_,_srcx_,_tag_,_tagx_) \
   (*dci->ScanRemoveBlocks)(_dst_,_dstSize_,_src_,_srcx_,_tag_,_tagx_)

#define DCEXTREC_MAGIC 0x7359A241
typedef struct {
   int  magic;
   int  DCExtSize;
   int  vers;
   int  (*open)(int flags,DCIntRec *ir);
   void (*close)(void);
   int  (*scan)(chr *s1,chr *s2,chr *sx);
} DCExtRec;

#define DCEXT_OPEN(_ex_,_flags_,_ir_) (*(_ex_)->open)(_flags_,_ir_)
#define DCEXT_CLOSE(_ex_) (*(_ex_)->close)()
#define DCEXT_SCAN(_ex_,_s1_,_s2_,_sx_) (*(_ex_)->scan)(_s1_,_s2_,_sx_)

enum {
   DCEXTERR_NONE,
   DCEXTERR_INTERNAL=-2500,
   DCEXTERR_SCAN_NOTFOUND=-2599,
   DCEXTERR_FILE_WRITE=-2699,
};

#ifdef DCEXT_EXPORTS
#define DCEXT_API __declspec(dllexport)
#else // ndef DCEXT_EXPORTS
#define DCEXT_API __declspec(dllimport)
#endif // ndef DCEXT_EXPORTS

DCEXT_API void* DCExtAdr(int flags);

#endif // DCEXT_H
