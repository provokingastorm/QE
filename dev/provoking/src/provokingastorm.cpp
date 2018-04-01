// Copyright (C) 2006-2007 Joe Linhoff, All Rights Reserved
// provokingastorm.cpp
#include "e_config.h"
#if BUILD_PROVOKING // compile this app
#include "qe.h"
#include "qefn.h"
#include "qeinst.h"

#define MAX_MESHES	50

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
};

// 6 faces
int cube_faces[]=
{
 4,0,1,3,2,
 4,2,3,5,4,
 4,4,5,7,6,
 4,6,7,1,0,
 4,1,7,5,3,
 4,6,0,2,4,
};

qeGeo1 cube_geo = 
{
   8, // numv
   6, // numf
   cube_verts, // verts
   cube_faces, // faces
};

typedef struct {
	chr* Name;
	qeLibGeo3* Geometry;
	qeLibMat1* Matrix;
	qeLibMtr1* Material;
	bool Initialized;
} Mesh;

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

   float currentRotAngle;

   Mesh GameMeshes[MAX_MESHES];
   int NextMeshIndex;
} ProvokingGame;

ProvokingGame game;

class Ship {
	float mShipM12[12]; // matrix
public:
	qeLibGeo3 *mGeo; // geometry definition
	int dispatch(int cprOp); // dispatch for ship
	int draw(void); // draw the ship
	int create(void); // create ship objects
}; // Ship

// game record
Ship ship1;

// JFL 16 Feb 07
int Ship::create(void)
{
	// add C++ function w/valid this pointer, static name
	/*if(!qeObjAddCFncSP(this,&Ship::draw,"N","f-shipDraw"))
		BRK();*/

	return 0;
} // Ship::create()

// JFL 16 Feb 07
int Ship::draw(void)
{
	if(!mGeo)
	{BRK();goto BAIL;}

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

	for(int i = 0; i < MAX_MESHES; i++)
	{
		game.GameMeshes[i].Geometry		= NULL;
		game.GameMeshes[i].Matrix		= NULL;
		game.GameMeshes[i].Material		= NULL;
		game.GameMeshes[i].Initialized	= false;
	}

	M12Id(mShipM12); // reset the matrix for the ship

	//
	// DISPATCH
	//

	qeCprSleep(1.2); // wait 1.2 seconds

	// --- CREATE ANI & SHIP ---

	if(create()<0)
		goto BAIL;

BAIL:
	return 0; // any return from a cpr deletes it
} // Ship::dispatch()

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

   ship1.draw();

   /*for(int i = 0; i < game.NextGeometryIndex; i++)
   {
	   if(game.AllGeometry != NULL)
	   {
		   glPushMatrix();
		   qeGLM12f(mShipM12);
		   qeDrawLitFaces(mGeo->verts,mGeo->normals,mGeo->faces,mGeo->numf);
		   glPopMatrix();
	   }
   }*/

   glColor3f(0,0,1); // color
   glRotatef(game.currentRotAngle, 0.0f, 1.0f, 0.0f);
   qeDrawFaces(geo->verts,geo->faces,geo->numf);

   //
   // DRAW COLLISION
   //

   if(game.col)
   {
      glColor3f(.5,0,0); // dark red

      glBegin(GL_LINES);
         
      glVertex3f(game.xyzCol[0],game.xyzCol[1],game.xyzCol[2]);
      glVertex3f(
         game.xyzCol[0]+game.nrmCol[0], 
         game.xyzCol[1]+game.nrmCol[1],
         game.xyzCol[2]+game.nrmCol[2]);
         
      glEnd();
   }

   return 0;
}

#define MPH_TO_FEETPERSEC(m) ((m)*5280.0/60.0/60.0)

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
}

int fncUpdate(void)
{
   float xyzOld[3],tCur,t;
   qeGeo1 *geo=&cube_geo;

   tCur=qeTimeFrame(); // get current time

   game.currentRotAngle += 1.0f;


   if(!game.shotActive)
      return 0; // not active
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
	qeLibChunk chunkmem,*chunk=&chunkmem; // pointer to mem for search result
	qeLibScanRec librec;
	qeLibScanRec *lib=&librec;
	MEMZ(game);

	qePrintf("%s / %s / %s\n",
		__FILE__,qeVersion(),glGetString(GL_VERSION));

	if(qeSndNew("shot1",0xf0,0,"../../bin/data/snd/one/shot1.wav")<0)
		BRK();
	if(qeSndNew("exp1",0xf0,0,"../../bin/data/snd/one/hitgnd1.wav")<0)
		BRK();

	// force a breakpoint if the lib file isn't found
	if((r = qeLibNew("buildings", 0, "../../bin/data/buildings.lib"))<0)
	{
		BRK();
	}

	// force a breakpoint if the lib failed to load into memory
	if((r = qeLibLoad("buildings"))<0)
	{
		BRK();
	}

	// zero everything we will look for in case we don't find it
	// we could do this in a constructor, but doing it here keeps
	// everything together which may help avoid errors
	ship1.mGeo=NUL; // geometry for the ship

	qeLibScanFirst(lib);

	// find chunks
	while(qeLibScanNext(lib, chunk, NUL, NUL)>=0)
	{
		switch(chunk->type)
		{
		case C4TYPE_CAM1:
			BRK();
			break;

		case C4TYPE_GEO3:
			{
				ship1.mGeo=(qeLibGeo3*)chunk->data; // found chunk

				/*if(game.NextGeometryIndex < (MAX_GEOMETRY-1))
				{
					game.AllGeometry[game.NextGeometryIndex] = (qeLibGeo3*)chunk->data;
					game.NextGeometryIndex++;
				}*/
			}
			break;

		case C4TYPE_IMG1:
			BRK();
			break;

		case C4TYPE_MAT:
			BRK();
			break;

		case C4TYPE_MAT1:
			BRK();
			break;

		case C4TYPE_MTR1:
			BRK();
			break;
		}
	}

	// add a C++ CPR function to dispatch (control over time) the ship
	if(!qeObjAddCprSP(&ship1,"CN",&Ship::dispatch,"c-shipDispatch"))
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