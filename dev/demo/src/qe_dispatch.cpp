// e_dispatch.cpp
#include "e_config.h"
#if BUILD_DISPATCH // compile this app
#include "qe.h"
#include "qesl.h"

///////////////////////////////////////////////////////////////////////////////

// JFL 23 Jan 07
class GameControl {
   uns mLoop;
   uns mAttractLoop;
   int mAllocCount;
   int startGame(void);
   int startAttract(float *timep);
public:
   GameControl(void);
   ~GameControl(void);
   int cprDispatch(int);
}; // GameControl

qefnArcBallRec arcBall;
qeLibMat1 *mat;
qeLibGeo3 *geo;
qeLibImg1 *img;

// LAL 28 Jan 07
int fncCam(void)
{
	qefnCamArcBall(&arcBall);
	qefnSysLight0();
	return 0;
} // fncCam()

// LAL 28 Jan 07
int fncDrawGrid(void)
{
	qefnDrawAxes(1);
	qefnDrawGrid(20,1);
// code to draw geometry

   if(!geo) return 0;

   if(mat)
   {
      glPushMatrix();
      qeGLM12f(mat->mat12);
   }
         
   qeTexBind(geo->imgname);
   glPolygonMode(GL_FRONT,GL_FILL);
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL); // RB416,444
   qeDrawLitUVFaces(geo->verts,geo->normals,
      geo->faces,geo->uvs,geo->numf);
   glDisable(GL_TEXTURE_2D);
   qeTexBind(NUL);
   
   if(mat)
      glPopMatrix();
   
	return 0;
} // fncDrawGrid()

GameControl gameControl;

// JFL 22 Jan 07
GameControl::GameControl()
{
   mLoop=0;
   mAttractLoop=0;
} // GameControl::GameControl()

// JFL 22 Jan 07
GameControl::~GameControl()
{
   mLoop=0;
} // GameControl::~GameControl()

struct {
   chr *name;
   chr *path;
} gameLibTable[] = 
{
   // paths relative to project's home directory
   {"env","libs/env1.lib"}, // env
   {"player","libs/player1.lib"}, // player
}; // gameLibTable[]

// JFL 22 Jan 07
int GameControl::startGame(void)
{
   int r,i, fail = 0;
   chr *name,*path;

  	// create and load test library
	path="libs/genericbldg.lib";
	if(qeLibNew("test",0,path)<0)
		{BRK();fail=1;qePrintf("* qeLibNew failed: %s\n",path);}
	if(qeLibLoad("test")<0)

   // run through all game libraries
   for(i=0;i<NUM(gameLibTable);i++)
   {
      name=gameLibTable[i].name;
      path=gameLibTable[i].path;

      // create lib entry & start an instance of all the chunks
      if((r=qeLibNew(name,0,path))<0)
         BRK(); // probably means the library not found (path? built?)
      if(qeLibStart(name)<0)
         BRK(); // error starting the library
   } // for

   // add camera and grid

   MEMZ(arcBall); // camera uses this record, zero it
   qeObjAddFncSP(fncCam,"n","f-cam"); // camera
   qeObjAddFncSP(fncDrawGrid,"n","f-grid"); // grid

  qeLibScanRec lsrmem,*lsr=&lsrmem;
   qeLibChunk libmem,*lib=&libmem;

   mat=NUL;
   geo=NUL;
   img=NUL;

 qeLibScanFirst(lsr);
   
   while(qeLibScanNext(lsr,lib,NUL,NUL)>=0)
   {
      switch(lib->type)
      {
         case C4TYPE_GEO3:
            qePrintf("geometry %C %s\n",lib->type,lib->name);
            geo=(qeLibGeo3*)lib->data;
            break;

         case C4TYPE_MAT1:
            qePrintf("matrix %C %s\n",lib->type,lib->name);
            mat=(qeLibMat1*)lib->data;
            break;
      
         case C4TYPE_IMG1:
            qePrintf("image %C %s\n",lib->type,lib->name);
            img=(qeLibImg1*)lib->data;
            break;
      
         default:
            qePrintf(" %C %s\n",lib->type,lib->name);
      } // switch
   } // while

   return 0;
} // GameControl::startGame()

struct {
   chr *name;
   chr *path;
   float time;
} atLibTable[] = 
{
   // paths relative to project's home directory
   {"atname","libs/lolmike.lib",1}, // attract mode title screen
   {"atcred","libs/lolmike.lib",1}, // attract mode credits screen
}; // atLibTable[]

// JFL 22 Jan 07
int GameControl::startAttract(float *timep)
{
   int r,i, fail = 0;
   chr *name,*path;

   // get name, path, and time for attract lib
   if(mAttractLoop>=NUM(atLibTable))
      mAttractLoop=0; // wrap around

   name=atLibTable[mAttractLoop].name;
   path=atLibTable[mAttractLoop].path;
   *timep=atLibTable[mAttractLoop].time;
   mAttractLoop++;

   // create lib entry & start an instance of all the chunks
   if((r=qeLibNew(name,0,path))<0)
      BRK(); // probably means the library not found (path? built?)
   if(qeLibStart(name)<0)
      BRK(); // error starting the library

   // run through all game libraries
   for(i=0;i<NUM(gameLibTable);i++)
   {
      name=gameLibTable[i].name;
      path=gameLibTable[i].path;

      // create lib entry & start an instance of all the chunks
      if((r=qeLibNew(name,0,path))<0)
         BRK(); // probably means the library not found (path? built?)
      if(qeLibStart(name)<0)
         BRK(); // error starting the library
   } // for

   // add camera and grid

   MEMZ(arcBall); // camera uses this record, zero it
   qeObjAddFncSP(fncCam,"n","f-cam"); // camera
   qeObjAddFncSP(fncDrawGrid,"n","f-grid"); // grid

  qeLibScanRec lsrmem,*lsr=&lsrmem;
   qeLibChunk libmem,*lib=&libmem;

   mat=NUL;
   geo=NUL;
   img=NUL;

 qeLibScanFirst(lsr);
   
   while(qeLibScanNext(lsr,lib,NUL,NUL)>=0)
   {
      switch(lib->type)
      {
         case C4TYPE_GEO3:
            qePrintf("geometry %C %s\n",lib->type,lib->name);
            if(lib->name == "c_cloud"
                geo=(qeLibGeo3*)lib->data;
            break;

         case C4TYPE_MAT1:
            qePrintf("matrix %C %s\n",lib->type,lib->name);
            mat=(qeLibMat1*)lib->data;
            break;
      
         case C4TYPE_IMG1:
            qePrintf("image %C %s\n",lib->type,lib->name);
            img=(qeLibImg1*)lib->data;
            break;
      
         default:
            qePrintf(" %C %s\n",lib->type,lib->name);
      } // switch
   } // while

   return 0;
} // GameControl::startAttract()

// JFL 22 Jan 07
int GameControl::cprDispatch(int cprOp)
{  // co-process (keep the local frame small, allocate memory if needed)
   int r;
   float t;

   // handle reasons the engine can call this function
   switch(cprOp&M_QECPROP)
   {
   case QECPROP_RUN:
      break; // normal run, continue through function
   case QECPROP_INIT:
      return 1; // init, return
   case QECPROP_FINAL:
      return 1; // final, return
   default:
      return 1; // not handled, return
   } // switch

   // only passes through here once
   mAllocCount=MemAllocCount(); // get memory alloc count

lp:
   // make sure resources are balanced
   if((r=MemAllocCount())!=mAllocCount)
   {
      BRK(); // unbalanced
      qeslMemInfo(NUL,NUL); // print info
      mAllocCount=r; // reset count
   }

   // start the game
   qeTimeReset(); // reset time
   qeLoopReset(); // reset loop count
   startGame(); // start the game
   qeCprSleep(3); // sleep for 3 seconds (wait until game is over)
   qeslWipeout(NUL,NUL); // reset the system
   qeCprSloop(1); // sleep for 1 loop after wipeout
   startAttract(&t); // start attract mode
   qeCprSleep(t); // sleep for t seconds
   qeslWipeout(NUL,NUL); // reset the system
   qeCprSloop(1); // sleep for 1 loop after wipeout
   mLoop++; // keep track of time through this loop
   goto lp; // loop

   // never returns

   return 0; // returns & kills obj
} // GameControl::cprDispatch()

///////////////////////////////////////////////////////////////////////////////

// qeMain()
// JFL 30 Jul 06
int qeMain(int argc,chr *argv[])
{
   int r,fail=0;

   // turn on/off initial options
   qeOptionTurnOff(M_QEOPTION_FIRSTCAM|M_QEOPTION_FIRSTLIGHT);

   // print version info
   qePrintf("%s / qe %s / OpenGL / %s\n",
      __FILE__,qeVersion(),glGetString(GL_VERSION));

   // add game control co-process (I=indestructible i=init f=final)
   qeObjAddCprSP(&gameControl,"CnIif",&GameControl::cprDispatch,"c-dispatch");

   // handle errors
   while((r=qeErrNext())<0)
      qePrintf("qeMainErr %s\n",qeErrString(r));

   // turn control over to the engine
   qeForever();

   // check for unbalanced allocations
   if(MemAllocCount())
      BRK();

   return 0;
} // qeMain()

#endif // compile this app
// EOF