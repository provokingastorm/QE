// Copyright (C) 2007 Joe Linhoff, All Rights Reserved
// qesl.h
#ifndef QESL_H
#define QESL_H

// C in C++ wrapper
#ifdef __cplusplus
extern "C" {
#endif

QE_API int qeslExit(ptr sf,chr *sp,...);
QE_API int qeslShowWire(ptr sf,chr *sp,...);
QE_API int qeslShowJoints(ptr sf,chr *sp,...);
QE_API int qeslStep(ptr sf,chr *sp,...);
QE_API int qeslBack(ptr sf,chr *sp,...);
QE_API int qeslRun(ptr sf,chr *sp,...);
QE_API int qeslList(ptr sf,chr *sp,...);
QE_API int qeslLibs(ptr sf,chr *sp,...);
QE_API int qeslWipeout(ptr sf,chr *sp,...);
QE_API int qeslMemInfo(ptr sf,chr *sp,...);

typedef struct {
   chr *name;
   chr *namex;
   ptr adr;
   uns f;
} qeslFncRec;

QE_API int qeslAddFncTable(qeslFncRec *fncRecs,uns numFncRecs);
QE_API int qeslString(chr *name,chr *namex,chr *ss,chr *sx);

// C in C++ wrapper
#ifdef __cplusplus
}
#endif

#endif // QESL_H

