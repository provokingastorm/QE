// Copyright (C) 2006-2007 Joe Linhoff, All Rights Reserved
// e_petedemo1.cpp
#include "e_config.h"
#if BUILD_PETEDEMO1 // compile this app
#include "qe.h"

// mesh cube

// 8 verts
float cube_verts[]=
{
 -0.5,-0.5,0.5,
 0.5,-0.5,0.5,
 -0.5,0.5,0.5,
 0.5,0.5,0.5,
 -0.5,0.5,-0.5,
 0.5,0.5,-0.5,
 -0.5,-0.5,-0.5,
 0.5,-0.5,-0.5,
}; // cube_verts[]

// 6 faces
int cube_faces[]=
{
 4,0,1,3,2,
 4,2,3,5,4,
 4,4,5,7,6,
 4,6,7,1,0,
 4,1,7,5,3,
 4,6,0,2,4,
}; // cube_faces[]

// cube geometry
qeGeo1 cube_geo = 
{
   8, // numv
   6, // numf
   cube_verts, // verts
   cube_faces, // faces
}; // cube_geo

// GameRec
// Game data record
// all defaults are zero
// JFL 03 Aug 06
typedef struct {

   // one shot
   bit shotActive;
   uns buttonShoot;
   float shotXYZ[3];
   float shotVel[3];
   float shotTime;
   float shotLastTime;

   // collision information
   int col; // collision
   float xyzCol[3];
   float nrmCol[3];

} GameRec;

GameRec game;

// JFL 18 Aug 06
// JFL 09 Oct 06
int fncDraw(void)
{
   qeGeo1 *geo=&cube_geo; // pointer to geo

   //
   // DRAW GROUND
   //

   qefnDrawGrid(10,10);

   //
   // DRAW SHOT
   //
   
   if(game.shotActive)
   { // draw shot
      // translate to world position, draw, untranslate
      glColor3f(0.5,0,0); // dark red
      glTranslatef(game.shotXYZ[0],game.shotXYZ[1],game.shotXYZ[2]);
      glutSolidSphere(0.1,6,6); // RB20
      glTranslatef(-game.shotXYZ[0],-game.shotXYZ[1],-game.shotXYZ[2]);
   } // draw shot

   //
   // DRAW GEOMETRY
   //

   glColor3f(0,0,1); // color
   qeDrawFaces(geo->verts,geo->faces,geo->numf);

   //
   // DRAW COLLISION
   //

   if(game.col)
   { // draw col
      glColor3f(.5,0,0); // dark red

      glBegin(GL_LINES);
         
      glVertex3f(game.xyzCol[0],game.xyzCol[1],game.xyzCol[2]);
      glVertex3f(
         game.xyzCol[0]+game.nrmCol[0], 
         game.xyzCol[1]+game.nrmCol[1],
         game.xyzCol[2]+game.nrmCol[2]);
         
      glEnd();
   } // draw col

   return 0;
} // fncDraw()

#define MPH_TO_FEETPERSEC(m) ((m)*5280.0/60.0/60.0)

// JFL 07 Aug 06
int fireShot(void)
{
   int r;
   float pos[3],dir[3],speed,tCur;

   tCur=qeTimeFrame();
   game.shotActive=1; // set this immediately after test for efficiency
   game.shotTime=tCur+1; // time to live
   game.shotLastTime=tCur; // current time

   // play sound
   qeSndPlay("shot1");

   // initial position of shot is camera position
   if((r=qeCamGetPDUR(NUL,pos,dir,NUL,NUL))<0)
      goto BAIL; // failed
   game.shotXYZ[0]=pos[0];
   game.shotXYZ[1]=pos[1];
   game.shotXYZ[2]=pos[2];

   // initial velocity is same direction camera is pointing
   speed=MPH_TO_FEETPERSEC(25);
   game.shotVel[0]=dir[0]*speed;
   game.shotVel[1]=dir[1]*speed;
   game.shotVel[2]=dir[2]*speed;

   r=0;
BAIL:
   return r;
} // fireShot()

// JFL 08 Aug 06
int fncUpdate(void)
{
   float xyzOld[3],tCur,t;
   qeGeo1 *geo=&cube_geo;

   if(!game.shotActive)
      return 0; // not active

   tCur=qeTimeFrame(); // get current time

   // move shot
   VecCpyf(xyzOld,game.shotXYZ); // save the 'old' position
   t=tCur-game.shotLastTime; // find elapsed time
   game.shotLastTime=tCur;
   game.shotXYZ[0]+=game.shotVel[0]*t; // move
   game.shotXYZ[1]+=game.shotVel[1]*t; // move
   game.shotXYZ[2]+=game.shotVel[2]*t; // move

   // test for collision
   if(!qeColRayPoly(game.xyzCol,game.nrmCol,
      xyzOld,game.shotXYZ,
      NULL,geo->verts,geo->faces,geo->numf))
   { // collision
      game.col=1; // collision!
      qeSndPlay("exp1"); // play collision sound
   } // collision

   // stop shot after a while
   if(game.shotTime<tCur)
      game.shotActive=0; // shot time is up

   return 0;
} // fncUpdate()

// JFL 06 Aug 06
int fncUser(void)
{
   uns b;

   b=qeInpButton(QEINPBUTTON_X);
   if((game.buttonShoot!=b)&&(b&1))
   {
      game.buttonShoot=b;
      fireShot();
   }

   return 0;
} // fncUser()

// qeMain()
// JFL 30 Jul 06
int qeMain(int argc,chr *argv[])
{
   int r;
   MEMZ(game);

   qePrintf("%s / %s / %s\n",
      __FILE__,qeVersion(),glGetString(GL_VERSION));

   if(qeSndNew("shot1",0xf0,0,"../../bin/data/snd/one/shot1.wav")<0)
      BRK();
   if(qeSndNew("exp1",0xf0,0,"../../bin/data/snd/one/hitgnd1.wav")<0)
      BRK();

   if(!qeObjAddFnc(fncUser))
      BRK();
   if(!qeObjAddFnc(fncUpdate))
      BRK();
   if(!qeObjAddFnc(fncDraw))
      BRK();

   // turn control over to the engine until the user closes the program
   qeForever();

   // handle errors
   while((r=qeErrNext())<0)
   {
      qePrintf("qeMainErr %s\n",qeErrString(r));
      BRK();
   } // while

   return 0;
} // qeMain()

#endif // compile this app
// EOF