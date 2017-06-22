// Copyright (C) 2006 Joe Linhoff, All Rights Reserved
// e_lib.c
#include "e_config.h"
#if BUILD_LIB // compile this app
#include "qe.h"

qefnArcBallRec arcBall;

// JFL 29 Jul 06
int fncCam(void)
{
   qefnCamArcBall(&arcBall);
   qefnSysLight0();
	return 0;
} // fncCam()

// JFL 29 Jul 06
int fncDrawGrid(void)
{
   qefnDrawAxes(1);
   qefnDrawGrid(20,10);
	return 0;
} // fncDrawGrid()

// JFL 30 Dec 06
// JFL 08 Jan 07
int addExtras(void)
{
   int r;
   qeObj *o,*first;
   qeObjInst *inst;
   qeObjScan scn;
   uns t;

   first=NUL;

   MEMZ(scn);
   for(o=qeObjScanNext(&scn,NUL,0,NUL);o;o=qeObjScanNext(&scn,NUL,0,NUL))
   {
      // get intstance pointer, skip if not an instance
      if((t=qeObjType(o))!=QEOBJTYPE_INST)
         continue;
      inst=(qeObjInst*)QEOBJ_REC(o);

      // keep track of first inst
      if(!first)
         first=o;

      // check for first camera
      if(!C4Cmp(C4TYPE_CAM,inst->typeC4))
         goto gotcam;
   } // for

   // couldn't find camera
   if(!first)
      ret(1);

   // add camera before first inst
   MEMZ(arcBall); // camera uses this record, zero it
   o=qeObjAddFncSP(fncCam,"bn",first,"f-cam");

gotcam:

   // add grid function after
   qeObjAddFncSP(fncDrawGrid,"an",o,"f-grid");

   r=0;
BAIL:
   return r;
} // addExtras()

// qeMain() -- entry point
// JFL 30 Nov 06
int qeMain(int argc,chr *argv[])
{
   int r,fail=0;
   chr *s1,*lib=NUL;

   // turn on/off initial options
   qeOptionTurnOff(M_QEOPTION_FIRSTCAM|M_QEOPTION_FIRSTLIGHT);

   // print info
   qePrintf("%s / qe %s / OpenGL / %s\n",
      __FILE__,qeVersion(),glGetString(GL_VERSION));

   // scan command line args
   while(argc>0)
   {
      // look for 'lib' <filename.lib>
      s1=*argv++,argc--;
      if(!szicmp("lib",s1,NUL) && (argc>0))
         lib=*argv++,argc--;
   } // while

   //
   // LOAD LIB
   //

   if(!lib)
      lib="libs/test.lib";

   // create lib entry
   if(qeLibNew("lib1",0,lib)<0)
   {
      qePrintf("Couldn't open lib: %s\n",lib);
      fail=1;
   }
   else
   {
      if(qeLibStart("lib1")<0)
         fail=1;
   }

   addExtras();

   // handle errors
   while((r=qeErrNext())<0)
      qePrintf("qeMainErr: %s\n",qeErrString(r));

   // turn control over to the engine
   qeForever();

   return 0;
} // qeMain()

#endif // compile this app
// EOF
