// Copyright (C) 2006-2007 Joe Linhoff, All Rights Reserved
// e_powerbar.cpp
#include "e_config.h"
#if BUILD_POWERBAR // compile this app
#include "qe.h"

// JFL 12 Aug 06
// JFL 29 Sep 06
// JFL 02 Oct 06
int fncEveryLoop(void)
{
   float val,w,h;

   val=1;//0.75; // 75% power

   // set 2D camera
   qefnSysCam2D();

   // the idea is to draw a bar on the top right
   // a full powerbar will be 1/4 of the view width and 1/20 of the height
   // qeFSysCam2D() sets up the view:
   //  (0,1,0)    ...    (1,1,0)
   //         (0.5,0.5,0)
   //  (0,0,0)           (1,0,0)

   // compute bar width & height in cam coords
   w=(1.0/4.0)*val;
   h=1.0/20.0;

   // color & fill
   glColor3f(1,1,1);
   glPolygonMode(GL_FRONT,GL_FILL);

   // draw quad bar
   glBegin(GL_QUADS);
   glVertex3f(1,1,0); // top right
   glVertex3f(1-w,1,0); // top left
   glVertex3f(1-w,1-h,0); // bot left
   glVertex3f(1,1-h,0); // bot right
   glEnd();

   return 0; // function must return a value, 0 indicates success
} // fncEveryLoop()

// qeMain()
// JFL 05 Aug 06
int qeMain(int argc,chr *argv[])
{
   qePrintf("%s / %s / %s\n",
      __FILE__,qeVersion(),glGetString(GL_VERSION));

   // add a function to the engine's object loop
   if(!qeObjAddFnc(fncEveryLoop))
      BRK();

   // turn control over to engine until the user closes the program
   qeForever();

   return 0;
} // qeMain()

#endif // compile this app
// EOF
