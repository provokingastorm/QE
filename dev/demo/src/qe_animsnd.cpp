// Copyright (C) 2007 Joe Linhoff, All Rights Reserved
// qe_animsnd.cpp
#include "e_config.h"
#if BUILD_ANIMSND // compile this app
#include "qe.h"
#include "qefn.h"
#include "qeinst.h"

// JFL 04 Mar 07
class Snd {
public:
   int mIdSong;
   float mDelay;
   int init(void); // init
   int tune1(int cprOp); // dispatch
}; // Snd

// game records
Snd snd1;

///////////////////////////////////////////////////////////////////////////////

qefnArcBallRec arcBall;

// JFL 29 Jul 06
int fncCamGrid(void)
{
   qefnCamArcBall(&arcBall);
   qefnSysLight0();
   qefnDrawAxes(1);
   qefnDrawGrid(20,1);
   return 0;
} // fncCamGrid()

///////////////////////////////////////////////////////////////////////////////

// JFL 04 Feb 07
int Snd::init(void)
{
   return 0;
} // Snd::init()

// JFL 16 Feb 07
int Snd::tune1(int cprOp)
{  // co-process (keep the local frame small, allocate memory if needed)

   if(cprOp!=QECPROP_RUN) // co-process operation, only run is handled
      {BRK();return -1;}

   // this is the dispatch routine for all persons

   //
   // DISPATCH
   //

   mDelay=0.5; // seconds

   if((mIdSong=qeSndPlay("song"))<0)
      BRK();

   qeCprSleep(4); // sleep 4 seconds

loop:
   // jump

   qeCprSleep(mDelay);
   qeSndPlay("click");
   qeCprSleep(mDelay);
   qeSndPlay("hit");
   qeCprSleep(mDelay*2);
   mDelay-=mDelay/4;

   //qeSndPlayIdVol(mIdSong,SNDVOL_MAX/2);

   if(mDelay<0.01)
      mDelay=1;
   goto loop;

//BAIL:
   return 0; // any return from a cpr deletes it
} // Snd::tune1()

// qeMain()
// JFL 05 Jan 07
int qeMain(int argc,chr *argv[])
{
   int r;

   qePrintf("%s / %s / %s\n",__FILE__,glGetString(GL_VERSION),qeVersion());

   MEMZ(arcBall);
   qeObjAddFncSP(fncCamGrid,"N","f-camGrid"); // camera


   if(qeSndNew("song",M_SNDNEW_CH_1|M_SNDNEW_LOOPING,0,
      "../helium/snd/one/devilsdoorway.wav")<0)
      BRK();

   if(qeSndNew("click",M_SNDNEW_CH_2|M_SNDNEW_CH_3,0,"../helium/snd/one/click1.wav")<0)
      BRK();
   if(qeSndNew("hit",M_SNDNEW_CH_2|M_SNDNEW_CH_2,0,"../helium/snd/one/hitgnd1.wav")<0)
      BRK();

   if(!qeObjAddCprSP(&snd1,"Cn",&Snd::tune1,"c-tune1"))
      BRK();

   // turn control over to the engine until the user closes the program
   qeOptionTurnOff(M_QEOPTION_FIRSTCAM|M_QEOPTION_FIRSTLIGHT);
   qeForever();

   // check for errors
   while((r=qeErrNext())<0)
   {
      qePrintf("qeMainErr %s\n",qeErrString(r));
      BRK(); // look in the log to find the error string
   } // while

   return 0;
} // qeMain()

#endif // compile this app
// EOF