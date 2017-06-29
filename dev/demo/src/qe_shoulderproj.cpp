// Copyright (C) 2006-2007 Joe Linhoff, All Rights Reserved
// e_shoulderproj.c
#include "e_config.h"
#if BUILD_SHOULDERPROJ // compile this app
#include "qe.h"

// GameRec
// Game data record
// all defaults are zero
// JFL 29 Oct 06
typedef struct 
{
   float tUpdateLast; // time
   float xyz[3]; // pos
   float quat[4]; // rotations as quaternion
   uns bStopCam; // button count to stop the camera
   bit stopCam; // stop the camera
   float xyzCam[3]; // camera positon
} GameRec;

typedef struct 
{
   uns button;
   float tUpdateLast;

   // one shot
   float xyzShot[3];
   float velShot[3];
   float timeShot;

   bit xyzRay;
   float xyza[3];
   float xyzb[3];

} ProjRec;

GameRec game;
ProjRec proj;

#define MPH_TO_FEETPERSEC(m) ((m)*5280.0/60.0/60.0)

// JFL 29 Oct 06
int fncShipUpdate(void)
{
   uns b;
   float t;
   float fwd,xa,ya,za;
   float q[4],m[9],v[3];

   //
   // TIME
   //

   // find time since last update
   t=game.tUpdateLast;
   game.tUpdateLast=qeTimeFrame();
   if(!t)
      goto BAIL; // first loop
   t=game.tUpdateLast-t;

   //
   // INPUT
   //

   // default move amounts to zero
   fwd=xa=ya=za=0;

   // buttons
   b=qeInpButton(QEINPBUTTON_UP);
   if((b&1))
      fwd+=MPH_TO_FEETPERSEC(5);

   b=qeInpButton(QEINPBUTTON_DOWN);
   if((b&1))
      fwd-=MPH_TO_FEETPERSEC(5);

   b=qeInpButton(QEINPBUTTON_LEFT);
   if((b&1))
      ya+=2*PI/10.0;

   b=qeInpButton(QEINPBUTTON_RIGHT);
   if((b&1))
      ya-=2*PI/10.0;

   b=qeInpButton(QEINPBUTTON_UP2);
   if((b&1))
      xa-=2*PI/10.0;

   b=qeInpButton(QEINPBUTTON_DOWN2);
   if((b&1))
      xa+=2*PI/10.0;

   b=qeInpButton(QEINPBUTTON_X);
   if((b&1))
      za+=2*PI/10.0;
   b=qeInpButton(QEINPBUTTON_Y);
   if((b&1))
      za-=2*PI/10.0;

   b=qeInpButton(QEINPBUTTON_A);
   if((b&1)&&(game.bStopCam!=b))
   {
      game.bStopCam=b;
      game.stopCam=game.stopCam?0:1;
   }

   if(!fwd && !xa && !ya && !za)
      goto BAIL;

   //
   // MOVEMENT
   //

   // all moves are time based
   fwd*=t;
   xa*=t;
   ya*=t;
   za*=t;

   QuatAngf(q,xa,ya,za); // build quat for this loop
   QuatMulf(game.quat,game.quat,q); // concatenate with current quat

   if(fwd)
   { // add fwd vel
      M9FromQuatf(m,game.quat); // mat for current rotation
      SET3(v,0,0,fwd); // unorotated forward velocity
      VecRotM9(q,v,m); // rotated forward velocity
      game.xyz[0]+=q[0];
      game.xyz[1]+=q[1];
      game.xyz[2]+=q[2];
   } // add fwd vel

BAIL:
   return 0;
} // fncShipUpdate()

// JFL 29 Oct 06
// JFL 18 Feb 07
int fncCam(void)
{
   int r;
   float m[12],vec[3],v1[3];
   qeCam *cam;

   if(!(cam=qeCamGet(NUL,NUL)))
      bret(-1);

   // load the perspective camera & identity modelview matrix
   qefnSysCamPerspectiveId();

   // if not stopped, update
   if(!game.stopCam)
   { // update cam pos
      M9FromQuatf(m,game.quat);
      SET3(v1,0,5,-10); // from ship to cam
      VecRotM9(vec,v1,m);
      VecAdd(game.xyzCam,vec,game.xyz);

   } // update cam pos

   // cam
   SET3(v1,0,1,0); // up
   qeCamLookAtM12f(cam->m12,game.xyzCam,game.xyz,v1);
   cam->type=QECAM_M12; // camera uses matrix
   qeGLM12f(cam->m12); // set matrix

   r=0;
BAIL:
   return r;
} // fncCam()

// JFL 29 Oct 06
int fncShipDraw(void)
{
   float m[12];

   // draw ground
   qefnDrawGrid(10,10);
   qefnDrawAxes(2);

   glColor3f(1,1,0.5);

   // draw ship
   glPushMatrix();
   SET3(m,game.xyz[0],game.xyz[1],game.xyz[2]); // xyz
   M9FromQuatf(m+3,game.quat); // rot
   qeGLM12f(m);
   qefnDrawModelLit(QE_PTRID_JET01_GEO1,1,1,1);
   glPopMatrix();

   return 0;
} // fncShipDraw()

// JFL 07 Aug 06
// JFL 18 Feb 07
int fireShot(void)
{
   int r;
   float dir[3],speed;
   int xy[2];
   qeCam *cam;

   if(!(cam=qeCamGet(NUL,NUL)))
      bret(-1);

   // play sound
   qeSndPlay("shot1");

   qeInpMouseXY(xy);
   qefnWinRayCamM12f(proj.xyza,proj.xyzb,0,xy[0],xy[1],0,10,cam->m12);
   proj.xyzRay=1;

   // initial position of shot is camera
   proj.xyzShot[0]=proj.xyza[0];
   proj.xyzShot[1]=proj.xyza[1];
   proj.xyzShot[2]=proj.xyza[2];

   // direction
   VecSub(dir,proj.xyzb,proj.xyza);
   VecNormalizef(dir);

   // initial velocity is same direction camera is pointing
   speed=MPH_TO_FEETPERSEC(20);
   proj.velShot[0]=dir[0]*speed;
   proj.velShot[1]=dir[1]*speed;
   proj.velShot[2]=dir[2]*speed;

   proj.timeShot=qeTimeFrame()+5;

   r=0;
BAIL:
   return r;
} // fireShot()

// JFL 08 Aug 06
int fncProjUpdate(void)
{
   uns b;
   float t;

   b=qeInpButton(QEINPBUTTON_MOUSELB);
   if((b&1)&&(proj.button!=b))
   {
      proj.button=b;
      fireShot();
   }

   if(proj.xyzRay)
   {
      glColor3f(0,1,0);
      glBegin(GL_LINES);
      glVertex3f(proj.xyza[0],proj.xyza[1],proj.xyza[2]);
      glVertex3f(proj.xyzb[0],proj.xyzb[1],proj.xyzb[2]);
      glEnd();
   }

   // find time since last update
   t=proj.tUpdateLast;
   proj.tUpdateLast=qeTimeFrame();
   if(!t)
      goto BAIL; // first loop
   t=proj.tUpdateLast-t;

   // move & draw proj
   if(proj.timeShot)
   { // proj

      // move shot
      proj.xyzShot[0]+=proj.velShot[0]*t;
      proj.xyzShot[1]+=proj.velShot[1]*t;
      proj.xyzShot[2]+=proj.velShot[2]*t;
   } // proj

BAIL:
   return 0;
} // fncProjUpdate()

// JFL 08 Aug 06
int fncProjDraw(void)
{

   // move & draw proj
   if(!proj.timeShot)
      return 0;

   //
   // draw shot
   //

   glPolygonMode(GL_FRONT,GL_FILL); // RB56, draw wireframe polygons
   glColor3f(0,0,1);

   // translate to world position, draw, untranslate
   glTranslatef(proj.xyzShot[0],proj.xyzShot[1],proj.xyzShot[2]);
   glutSolidSphere(.1,6,6); // RB20
   glTranslatef(-proj.xyzShot[0],-proj.xyzShot[1],-proj.xyzShot[2]);
      
   // keep track of 'time' shot has been active
   // stop shot after so long
   if(proj.timeShot<qeTimeFrame())
      ;//proj.timeShot=0; // shot time is up

   return 0;
} // fncProjDraw()

// qeMain()
// JFL 30 Jul 06
int qeMain(int argc,chr *argv[])
{
   // INIT
   MEMZ(game);
   QuatId(game.quat);

   // options
   qeOptionTurnOff(M_QEOPTION_FIRSTCAM|M_QEOPTION_FIRSTLIGHT);

   qePrintf("%s / %s / %s\n",__FILE__,qeVersion(),glGetString(GL_VERSION));

   // sound
   if(qeSndNew("shot1",0xf0,0,"../../bin/data/snd/one/shot1.wav")<0)
      BRK();

   // -- game loop --

   // update
   if(!qeObjAddFncSP(fncShipUpdate,"N","f-shipUpdate"))
      BRK();
   if(!qeObjAddFncSP(fncProjUpdate,"N","f-projUpdate"))
      BRK();

   // camera
   if(!qeObjAddFncSP(fncCam,"N","f-cam"))
      BRK();

   // draw
   if(!qeObjAddFncSP(fncShipDraw,"N","f-shipDraw"))
      BRK();
   if(!qeObjAddFncSP(fncProjDraw,"N","f-projDraw"))
      BRK();

   // turn control over to the engine until the user closes the program
   qeForever();

   return 0;
} // qeMain()

#endif // compile this app
// EOF