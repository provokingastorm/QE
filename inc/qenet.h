// Copyright (C) 2007 Joe Linhoff, All Rights Reserved
// qenet.h
#ifndef QENET_H
#define QENET_H

// C in C++ wrapper
#ifdef __cplusplus
extern "C" {
#endif

#ifndef QEGL_API
#define QEGL_API 1
#endif // QEGL_API

#ifdef QE_EXPORTS
#define GLAPI __declspec(dllexport)
#else // !QE_EXPORTS
#define GLAPI _declspec(dllimport)
#endif // !QE_EXPORTS

QE_API int qeSocketNewSP(chr *name,chr *sp,...); // register a socket
QE_API int qeSocketOpen(chr *name,uns flags); // open the socket
QE_API int qeSocketClose(chr *name,uns flags); // close the socket
QE_API int qeSocketSendSP(chr *name,chr *sp,...); // send a message

typedef int (*qeNetCallback)(qeObj* netObj,uns op,...);
#define M_NETCALLBACK 0xff
enum {
   NETCALLBACK_NONE,
   NETCALLBACK_PACKET_RECEIVED, // uns8* buf,int len
   NETCALLBACK_NOP,
};

enum {
   QENETOP_NETOBJ_SENDERNAME=1, // qeObj *netObj,chr *buf,uns bufSize
   QENETOP_NETOBJ_SENDERIP4, // qeObj *netObj,uns32 *ip
   QENETOP_STRING_TO_IP4, // uns32 *dstip,chr *srcadr,chr *srcadrx
   QENETOP_SOCKET_NAME, // chr *socket,chr *buf,uns bufSize
   QENETOP_LOCALHOST_NAME, // chr *buf,uns bufSize
   QENETOP_LOCALHOST_IP4, // uns32 *dstip
};

QE_API int qeNetOp(uns op,...);

// C in C++ wrapper
#ifdef __cplusplus
}
#endif

#endif // QEGL_H

