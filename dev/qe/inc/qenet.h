// Copyright (C) 2007 Joe Linhoff, All Rights Reserved
// qenet.h
#ifndef QENET_H
#define QENET_H

// C in C++ wrapper
#ifdef __cplusplus
extern "C" {
#endif

// qe notes:
// this file defines OpenGL functionality beyond the basics in gl.h
// GL_GLEXT_PROTOTYPES -- keep this undefined.  I would like to set this to enable
// proper tool-tips, but haven't worked through the build and linking issues

#ifndef QEGL_API
#define QEGL_API 1
#endif // QEGL_API

#ifdef QE_EXPORTS
#define GLAPI __declspec(dllexport)
#else // !QE_EXPORTS
#define GLAPI _declspec(dllimport)
#endif // !QE_EXPORTS

QE_API int qeSocketNewSP(chr *name,chr *sp,...);
QE_API int qeSocketOpen(chr *name,uns flags);
QE_API int qeSocketClose(chr *name,uns flags);
QE_API int qeSocketSendSP(chr *name,chr *sp,...);
QE_API int qeNetObjNameOfSender(qeObj *netObj,uns flags,chr *buf,uns bufSize);

typedef int (*qeNetCallback)(qeObj* netObj,uns op,...);
#define M_NETCALLBACK 0xff
enum {
   NETCALLBACK_NONE,
   NETCALLBACK_PACKET_RECEIVED, // uns8* buf,int len
};

// C in C++ wrapper
#ifdef __cplusplus
}
#endif

#endif // QEGL_H

