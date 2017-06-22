// Copyright (C) 2006-2007 Joe Linhoff, All Rights Reserved
// e_icons.cpp
#include "e_config.h"
#if BUILD_ICONS  
#include "qe.h"

typedef struct {
   int icon;
   uns upCount;
   uns downCount;
} gameRec;

gameRec game;

// window width of target platform
#define GAME_TARGET_WIDTH 800.0
#define GAME_TARGET_HEIGHT 600.0

// width & height of icons in image pixels coordinates
#define GAME_ICON_WIDTH 32
#define GAME_ICON_HEIGHT 32

// JFL 12 Aug 06
// JFL 02 Oct 06
int fncEveryLoop(void)
{
   int r;
   float u0,v0,u1,v1,x,y,w,h;
   uns imgWidth,imgHeight;
   chr *imgName;
   uns j;

   imgName="icon1";

   // set 2D camera
   qefnSysCam2D();

   // qeFSysCam2D() sets up the view:
   //  (0,1,0)    ...    (1,1,0)
   //         (0.5,0.5,0)
   //  (0,0,0)           (1,0,0)

   // change icon based on up/down key presses
   j=qeInpButton(QEINPBUTTON_UP);
   if((j&1)&&(j!=game.upCount))
   { // up key
      game.upCount=j;
      game.icon++;
   } // up key
   j=qeInpButton(QEINPBUTTON_DOWN);
   if((j&1)&&(j!=game.downCount))
   { // down key
      game.downCount=j;
      game.icon--;
   } // down key

   // wrap icon if needed
   if(game.icon<0)
      game.icon=9;
   else if(game.icon>9)
      game.icon=0;

   // get image width & height
   if((r=qeImgInfo(imgName,&imgWidth,&imgHeight,NUL,NUL))<0)
      goto BAIL;

   // find image coordinates
   // icons in our image are 32x32 and there are 8 across horizontally
   // these coordinates are in pixel coordinates of the image
   // top left of the image 0,0
   u0=(game.icon&7); // eight across
   u0*=GAME_ICON_WIDTH; // in pixel image coords
   v0=(game.icon>>3); // eight across
   v0*=GAME_ICON_HEIGHT; // in pixel image coords
   u1=u0+GAME_ICON_WIDTH;
   v1=v0+GAME_ICON_HEIGHT;

   // map image coordinates into uv space
   u0/=imgWidth;
   v0/=imgHeight;
   u1/=imgWidth;
   v1/=imgHeight;

   // xy in target screen coordinates
   x=0.5; // center of the screen
   y=0.5; // center of the screen
   w=GAME_ICON_WIDTH/GAME_TARGET_WIDTH;
   h=GAME_ICON_HEIGHT/GAME_TARGET_HEIGHT;

   // OpenGL states
   qeTexBind(imgName); // bind texture
   glEnable(GL_TEXTURE_2D); // enable texturing
   glColor3f(0,1,0); // set color

   // draw quad
   glBegin(GL_QUADS);

   glTexCoord2f(u0,v0); // top left in texture coords
   glVertex3f(x,y,0); // top left in cam coords

   glTexCoord2f(u0,v1); // bot left in texture coords
   glVertex3f(x,y-h,0); // bot left in cam coords

   glTexCoord2f(u1,v1); // bot right in texture coords
   glVertex3f(x+w,y-h,0); // bot right in cam coords

   glTexCoord2f(u1,v0); // top right in texture coords
   glVertex3f(x+w,y,0); // top right in cam coords

   glEnd();

   glDisable(GL_TEXTURE_2D); // convention is to disable texture

BAIL:
   return 0; // function must return a value, 0 indicates success
} // fncEveryLoop()

// qeMain()
// JFL 05 Aug 06
int qeMain(int argc,chr *argv[])
{
   // initialize
   MEMZ(game);

   qePrintf("%s / %s / %s\n",
      __FILE__,qeVersion(),glGetString(GL_VERSION));

   // load image data
   if(qeImgNew("icon1","../helium/pics/icons16.tga")<0)
      BRK();

   // add a function to the engine's object loop
   if(!qeObjAddFnc(fncEveryLoop))
      BRK();

   // turn control over to engine until the user closes the program
   qeForever();

   return 0;
} // qeMain()

#endif // compile this app
// EOF
