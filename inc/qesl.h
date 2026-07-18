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
   chr *name; // command string start
   chr *namex; // command string end (or null)
   ptr adr; // pointer to function: int fnc(ptr sf,chr *sp,...)
   uns flags; // flags, default is 0
} qeslFncRec;

QE_API int qeslAddFncTable(qeslFncRec *fncRecs,uns numFncRecs);
QE_API int qeslString(chr *name,chr *namex,chr *ss,chr *sx);

// // JFL 06 Feb 08
// int gameCmdHello(ptr sf,chr *sp,...)
// {
//    qePrintf("Hello Console\n");
//    return 0;
// } // gameCmdHello()
//
// ptr sf -- system scripting frame pointer
// chr *sp -- characters signal the variable arguments
//  's' chr *s1 = varg_next(args,chr*) -- "strings must use quotes"
//  'f' float f1 = varg_next(args,float) -- numbers with .
//  'i' int i1 = varg_next(args,int) -- other numbers
//  'c' int i1 = varg_next(args,int) -- non numeric, non alpha characters
//
// the console scripting system is line oriented, begins with the command,
// and is followed by the parameters
//
// qeslFncRec gameConsoleCommands[] = {
//    {"hello",NUL,gameCmdHello,0}, // command,null,function,flags
// }; // gameConsoleCommands[]
//
//    // add console command table
//    if(qeslAddFncTable(gameConsoleCommands,NUM(gameConsoleCommands))<0)
//       BRK();

// C in C++ wrapper
#ifdef __cplusplus
}
#endif

#endif // QESL_H

