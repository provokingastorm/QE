// Copyright (C) 2007 Joe Linhoff, All Rights Reserved
// e_animstart.cpp
#include "e_config.h"
#if BUILD_ANIMSTART // compile this app
#include "qe.h"
#include "qefn.h"
#include "qeinst.h"

// animations
typedef struct {
   qeLibAni1 *takeoff;
   qeLibAni1 *rollright;
} ShipAnimations;

class Ship {
   float mShipM12[12]; // matrix
public:
   qeObj *mAniRoot; // animation object
   qeObj *mAniSecond; // animation object
   qeLibGeo3 *mGeo; // geometry definition
   ShipAnimations mAnimations; // animation table
   int dispatch(int cprOp); // dispatch for ship
   int draw(void); // draw the ship
   int create(void); // create ship objects
}; // Ship

// game record
Ship ship1;

///////////////////////////////////////////////////////////////////////////////

// JFL 16 Feb 07
int Ship::create(void)
{
   qeInstNodeAni *ani;
  
   // set to link all new objects before current object
   // because this sets a state variable, it will be invalid after a sleep
   qeObjSetLink(qeObjCur(0),M_QEOBJSETLINK_BEFORE);

   // create & add an instance to the engine object list
   // add matrix memory following ani
   if((mAniRoot=qeObjAddInstSP(NUL,C4TYPE_ANI,"Ne","a-ship1",
      sizeof(qeInstNodeAni)+sizeof(qeM12f))))
   { // created ani
      // flags for animation to drive qeM12f following
      if((ani=qeObjAni(mAniRoot))) // get adr of ani
         ani->flags|=M_QEINSTANI_DRIVEM12F;

   } // created ani

   // create second animation
   if((mAniSecond=qeObjAddInstSP(NUL,C4TYPE_ANI,"Ne","a-ship2",
      sizeof(qeInstNodeAni)+sizeof(qeM12f))))
   { // created ani

      // flags for animation to drive qeM12f following
      if((ani=qeObjAni(mAniSecond))) // get adr of ani
         ani->flags|=M_QEINSTANI_DRIVEM12F;

   } // created ani

   // add C++ function w/valid this pointer, static name
   if(!qeObjAddCFncSP(this,&Ship::draw,"N","f-shipDraw"))
      BRK();

   return 0;
} // Ship::create()

// JFL 16 Feb 07
int Ship::draw(void)
{
   float *aniMatRoot,*aniMatSecond;

   if(!mGeo)
      {BRK();goto BAIL;}

   // build ship matrix from two animations
   if(!(aniMatRoot=qeObjAniM12f(mAniRoot)))
      aniMatRoot=qefnM12Identity; // err, use identity matrix
   if(!(aniMatSecond=qeObjAniM12f(mAniSecond)))
      aniMatSecond=qefnM12Identity; // err, use identity matrix
   M12Mulf(mShipM12,aniMatRoot,aniMatSecond); // combine, total matrix

   glPushMatrix();
   qeGLM12f(mShipM12);
   qeDrawLitFaces(mGeo->verts,mGeo->normals,mGeo->faces,mGeo->numf);
   glPopMatrix();

BAIL:
   return 0;
} // Ship::draw()

// JFL 16 Feb 07
int Ship::dispatch(int cprOp)
{  // co-process (keep the local frame small, allocate memory if needed)

   if(cprOp) // co-process operation -- only QECPROP_RUN==0 is handled
      {BRK();return -1;}

   //
   // INIT
   //

   M12Id(mShipM12); // reset the matrix for the ship

   //
   // DISPATCH
   //

   qeCprSleep(1.2); // wait 1.2 seconds

   // --- CREATE ANI & SHIP ---

   if(create()<0)
      goto BAIL;

   qeCprSleep(2); // wait 2 seconds

   // --- START ANI ---

   qeObjAniStart(mAniRoot,mAnimations.takeoff); // start root ani
   qeCprSleep(2); // sleep
   qeObjAniStart(mAniSecond,mAnimations.rollright); // start secondary ani

   // --- SLEEP ---

   qeCprSleep(5*60); // sleep for 5 minutes

BAIL:
   return 0; // any return from a cpr deletes it
} // Ship::dispatch()

// qeMain()
// JFL 05 Jan 07
int qeMain(int argc,chr *argv[])
{
   int r;
   qeLibChunk chunkmem,*chunk=&chunkmem; // pointer to mem for search result

   qePrintf("%s / %s / %s\n",__FILE__,glGetString(GL_VERSION),qeVersion());

   // load the test lib
   if(qeLibNew("jet",0,"../helium/libs/jet1.lib")<0)
      BRK();
   if(qeLibLoad("jet")<0)
      BRK();

   //
   // SETUP SHIP
   //

   // zero everything we will look for in case we don't find it
   // we could do this in a constructor, but doing it here keeps
   // everything together which may help avoid errors
   ship1.mGeo=NUL; // geometry for the ship
   ship1.mAniRoot=NUL; // animation for the ship
   ship1.mAniSecond=NUL; // animation for the ship
   MEMZ(ship1.mAnimations); // zero all animations at once

   // find chunks
   if(qeLibFind(chunk,"jet/jet01",NUL,C4TYPE_GEO3)>=0)
      ship1.mGeo=(qeLibGeo3*)chunk->data; // found chunk
   if(qeLibFind(chunk,"jet/takeoff",NUL,C4TYPE_ANI1)>=0)
      ship1.mAnimations.takeoff=(qeLibAni1*)chunk->data; // found chunk
   if(qeLibFind(chunk,"jet/rollright",NUL,C4TYPE_ANI1)>=0)
      ship1.mAnimations.rollright=(qeLibAni1*)chunk->data; // found chunk

   // add a C++ CPR function to dispatch (control over time) the ship
   if(!qeObjAddCprSP(&ship1,"CN",&Ship::dispatch,"c-shipDispatch"))
      BRK();

   // turn control over to the engine until the user closes the program
   qeForever();

   // check for errors
   while((r=qeErrNext())<0)
   {
      qePrintf("qeMainErr %s\n",qeErrString(r));
      BRK();
   } // while

   return 0;
} // qeMain()

#endif // compile this app
// EOF