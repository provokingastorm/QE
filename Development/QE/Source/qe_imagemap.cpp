// Copyright (C) 2006-2007 Joe Linhoff, All Rights Reserved
// e_imagemap.cpp
#include "e_config.h"
#if BUILD_IMAGEMAP // compile this app
#include "qe.h"

#define R 0xff,0x00,0x00,0xff
#define G 0x00,0xff,0x00,0xff
#define B 0x00,0x00,0xff,0xff
#define W 0xff,0xff,0xff,0xff

// define an image
uns8 rgbaImg1[8*8*4] =
{
   R,R,R,W,W,W,G,G,
   R,R,R,W,W,W,G,G,
   R,R,R,W,W,W,G,G,
   G,G,G,W,W,W,G,G,
   G,G,G,W,W,W,G,G,
   G,G,G,W,W,W,G,G,
   B,B,B,B,B,B,G,G,
   B,B,B,B,B,B,G,G,
}; // rgbaImg1[]

// JFL 12 Aug 06
// JFL 29 Sep 06
int fncEveryLoop(void)
{
   // set 2D camera
   qefnSysCam2D();

   // qeFSysCam2D() sets up the view:
   //  (0,1,0)    ...    (1,1,0)
   //         (0.5,0.5,0)
   //  (0,0,0)           (1,0,0)

   // bind image
   qeTexBind("img1");
   glEnable(GL_TEXTURE_2D);

   // color & fill
   glColor3f(1,1,1);
   glPolygonMode(GL_FRONT,GL_FILL);

   // draw quad bar
   glBegin(GL_QUADS);
  
   glTexCoord2f(0,0); // (0,0) is top left of texture
   glVertex3f(0,1,0); // camera coordinates

   glTexCoord2f(0,1); // (0,1) is bot left of texture
   glVertex3f(0,0.5,0); // camera coordinates

   glTexCoord2f(1,1); // (1,1) is bot right of texture
   glVertex3f(0.5,0.5,0); // camera coordinates

   glTexCoord2f(1,0); // (1,0) is top right of texture
   glVertex3f(0.5,1,0); // camera coordinates
   glEnd();

   return 0; // function must return a value, 0 indicates success
} // fncEveryLoop()

// qeMain()
// Entry point
// JFL 05 Aug 06
int qeMain(int argc,chr *argv[])
{
   // add image
   if(qeImgNewRGBA("img1",8,8,rgbaImg1)<0) // name, width, height, data
       qePrintf("** qeImgNewRGBA failed\n"); 

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
