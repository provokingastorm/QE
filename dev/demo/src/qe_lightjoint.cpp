// Copyright (C) 2007 Joe Linhoff, All Rights Reserved
// qe_lightjoint.cpp
#include "e_config.h"
#if BUILD_LIGHTJOINT 
#include "qe.h"
#include "qefn.h"
#include "qeinst.h"

enum {
   PERSTYPE_NONE,
   PERSTYPE_MAN,
};

// animation table
typedef struct {
   qeObj *idle;
   qeObj *walk;
   qeObj *jump;
} PersAnimations;

// JFL 04 Mar 07
class Pers {
public:
   uns8 persType;
   qeObj *mMat; // matrix object
   qeObj *mSkel; // skeleton object
   qeObj *mAniRoot; // animation object
   qeObj *mJob; // jointed object
   int init(void); // init
   int start(chr *libName,chr *name);
   int dispatch(int cprOp); // dispatch
   int draw(void); // draw function
}; // Pers

// game records
Pers man1;
PersAnimations persAnimTable;

// global ambient light is dark gray
GLfloat global_ambient[]   = {0.3, 0.3, 0.3, 1.0}; // global ambient light

// this light is a directional light (b/c light_dir[3]==0)
// the light is very far away shinning directly up, it does not add any
// ambient light to the scene -- the light is green and has a white highlight
qeLight1 gameLight = {
   {0.0, 0.0, 1.0, 0.0}, // vector to light from 0,0,0
   {0.2, 0.2, 0.2, 1.0}, // ambient always added in
   {0.7, 1.0, 0.7, 1.0}, // diffuse light
   {1.0, 1.0, 1.0, 1.0} // specular highlight
}; // gameLight

// this material combines all the ambient light in the scene with 70% of the
// light that hits the surface -- no reflection
qeMaterial1 manMtr = {
   {1.0, 1.0, 1.0, 1.0}, // ambient always added in
   {0.7, 0.7, 0.7, 1.0}, // diffuse light hitting the surface
   {0.0, 0.0, 0.0, 1.0}, // specular reflective highlight
   5.0, // 0..128 shininess exponent
}; // manMtr

///////////////////////////////////////////////////////////////////////////////

qefnArcBallRec arcBall;

// JFL 29 Jul 06
int fncCamGrid(void)
{
   qefnCamArcBall(&arcBall); // set up camera matrices
   qefnDrawAxes(1); // draw axes
   qefnDrawGrid(20,1); // draw grid

   // set light parameters -- RB194
   // it is good practice to associate ligthts with a camera
   glLightfv(GL_LIGHT1,GL_AMBIENT, gameLight.ambient); // light always adds
   glLightfv(GL_LIGHT1,GL_DIFFUSE, gameLight.diffuse); // where the light hits
   glLightfv(GL_LIGHT1,GL_SPECULAR, gameLight.specular); // reflective highlight   
   glLightfv(GL_LIGHT1,GL_POSITION, gameLight.posdir); // position or direction
   glEnable(GL_LIGHT1); // enable if setup

   return 0;
} // fncCamGrid()

///////////////////////////////////////////////////////////////////////////////

// JFL 04 Feb 07
int Pers::init(void)
{
   mAniRoot=NUL;
   mJob=NUL;
   mMat=NUL;
   mSkel=NUL;
   return 0;
} // Pers::init()

// JFL 04 Feb 07
int Pers::start(chr *libName,chr *name)
{ // setup nodes
   int r;
   qeLibChunk chunkmem,*chunk=&chunkmem; // pointer to mem for search result
   chr buf[128];

   // notes:
   // name is not static, so object creation code must use 'n'
   // this code depends on the root node of the skeleton being named 'root'

   // find the root joint of the skeleton
   szcpycat(buf,sizeof(buf),libName,NUL,"/root",NUL); // create name buf
   if((r=qeLibFind(chunk,buf,NUL,C4TYPE_SKL1))<0)
      goto BAIL;

   // create jointed object nodes -- this is a complex set of nodes
   if(!(this->mJob=qeObjJointedAddSP(chunk->libObj,"n",name)))
      bret(-1);

   // create matrix object in the skeleton nodes
   szcpycat(buf,sizeof(buf),"m-",NUL,name,NUL); // create name buf
   if(!(this->mMat=qeObjAddInstSP(NUL,C4TYPE_MAT,"nsa",
      buf,sizeof(qeInstNodeMat),qeObjChild(this->mJob))))
      BRK();

   // add draw function
   if(!(qeObjAddCFncSP(this,&Pers::draw,"Na","f-draw",this->mMat)))
      BRK();

   // find jointed object's skeleton node
   if(!(this->mSkel=qeObjJointedSkel(this->mJob)))
      BRK();

   // create dispatch cpr
   szcpycat(buf,sizeof(buf),"c-",NUL,name,NUL); // create name buf
   if(!qeObjAddCprSP(this,"Cn",&Pers::dispatch,buf))
      BRK();

   r=0;
BAIL:
   return r;
} // Pers::start()

// JFL 11 Mar 07
int Pers::draw(void)
{ // draw function, must be linked after skeleton node
   qeObj *o;
   qeObjInst *inst;
   qeInstNodeSkn *skn;
   qeLibGeo3 *geo;
   bit texbound;

   // enable lighting, this turns on any individually enabled lights
   glEnable(GL_LIGHTING);

   // set the material for this object
   glMaterialfv(GL_FRONT,GL_AMBIENT,manMtr.ambient); // always added
   glMaterialfv(GL_FRONT,GL_DIFFUSE,manMtr.diffuse); // where light hits
   glMaterialfv(GL_FRONT,GL_SPECULAR,manMtr.specular); // reflection
   glMaterialf(GL_FRONT,GL_SHININESS,manMtr.shininess); // RB214 -- reflection exponent

   glPolygonMode(GL_FRONT,GL_FILL);
   glShadeModel(GL_SMOOTH);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE); // RB416,444
   
   // draw lighting-sphere to help us debug the lighting
   #if DEBUG
   glColor3f(1.0,0.0,0.0); // not used
   glTranslatef(-2,0,0);
   glutSolidSphere(1,20,20); // RB20
   glTranslatef(2,0,0);
   #endif // DEBUG

   // run through all the siblings and draw skin nodes
   texbound=0;
   for(o=qeObjNext(mSkel);o;o=qeObjNext(o))
   {
      if(qeObjType(o)!=QEOBJTYPE_INST)
         continue; // not inst, skip
      if(!(inst=(qeObjInst*)qeObjNode(o)))
         continue; // no inst pointer, skip
      if(inst->typeC4!=C4TYPE_SKN)
         continue; // inst not desired type, skip
      if(!(skn=(qeInstNodeSkn*)inst->p))
         continue; // no skin pointer
      if(!(geo=(qeLibGeo3*)skn->geoLib)||(skn->geoType!=C4TYPE_GEO3))
         continue; // geometry type, skip

      // bind texture and set mode with the the first skin/geo combination
      if(!texbound)
      { // first bind
         texbound=1;
         if(qeTexBind(geo->imgname)>=0)
            glEnable(GL_TEXTURE_2D);
      } // first bind

      // draw
      qeDrawLitUVFaces(skn->verts,skn->normals,geo->faces,geo->uvs,geo->numf);
   } // for

   // disable
   qeTexBind(NUL);
   glDisable(GL_TEXTURE_2D);
   glDisable(GL_LIGHTING);

   return 0;
} // Pers::draw()

// JFL 16 Feb 07
int Pers::dispatch(int cprOp)
{  // co-process (keep the local frame small, allocate memory if needed)

   if(cprOp!=QECPROP_RUN) // co-process operation, only run is handled
      {BRK();return -1;}

   // this is the dispatch routine for all persons

   //
   // DISPATCH
   //

   qeCprSleep(0.2); // sleep 0.2 seconds

   // --- START ANI ---

   // start idle animation
   qeObjAniStart(mSkel,persAnimTable.idle); // start anim
   qeObjAniSetFlags(mSkel,M_QEOBJANI_LOOP); // override anim flag
   qeObjJointedOp(mJob,QEJOINTEDOP_VISIT); // visit
   qeObjJointedOp(mJob,QEJOINTEDOP_DONTDRAW); // but dont draw

   qeCprSleep(4); // sleep 4 seconds

loop:
   // jump
   qeObjAniStart(mSkel,persAnimTable.jump); // start anim
   qeObjAniSetFlags(mSkel,M_QEOBJANI_LOOP); // override anim flag

   qeCprSleep(4); // sleep 4 seconds

   // idle
   qeObjAniStart(mSkel,persAnimTable.idle); // start anim
   qeObjAniSetFlags(mSkel,M_QEOBJANI_LOOP); // override anim flag

   qeCprSleep(14); // sleep 14 seconds
   goto loop;

   // -- b/c of goto, code never gets here

//BAIL:
   return 0; // any return from a cpr deletes it
} // Pers::dispatch()

// qeMain()
// JFL 05 Jan 07
int qeMain(int argc,chr *argv[])
{
   int r;
   chr *s1;
   qeLibChunk chunkmem,*chunk=&chunkmem; // pointer to mem for search result
   qeInstNodeMat *mat;

   qePrintf("%s / %s / %s\n",__FILE__,glGetString(GL_VERSION),qeVersion());

   MEMZ(arcBall);
   qeObjAddFncSP(fncCamGrid,"N","f-camGrid"); // camera
   
   // man lib
   if(qeLibNew("man",M_QELIBNEW_LOAD,"../helium/libs/pvkman.lib")<0)
      BRK();

   // load the animation set
   if(qeLibNew("anim",M_QELIBNEW_LOAD,"../helium/libs/pvkanim.lib")<0)
      BRK();

   //
   // SETUP    
   //

   // zero everything we will look for in case we don't find it
   // doing this in our own init() function is important b/c it keeps
   // the fields together which will help prevent errors
   // doing this in game code is better than in a constructor b/c
   // it allows us to reinitialize the fields
   man1.init(); // instance of character
   MEMZ(persAnimTable); // animation table

   // find & load image chunks
   if(qeLibFind(chunk,"man/pvkman",NUL,C4TYPE_IMG1)>=0)
      qeInstLoad(chunk->data,chunk->type); // load an instance of the image

   // find animation chunks
   if(qeLibFind(chunk,"anim/idle",NUL,C4TYPE_ANI2)>=0)
      persAnimTable.idle=chunk->libObj; // found animation
   if(qeLibFind(chunk,"anim/jump",NUL,C4TYPE_ANI2)>=0)
      persAnimTable.jump=chunk->libObj; // found animation

   // add CPR functions for dispatch (control over time)

   // -- man --
   man1.start("man","man1");
   man1.persType=PERSTYPE_MAN;
   if((mat=(qeInstNodeMat*)qeObjExt(man1.mMat)))
      M12BuildRTf(mat->mat12,0,0,0,0,0,0);

   // turn control over to the engine until the user closes the program
   qeOptionTurnOff(M_QEOPTION_FIRSTCAM|M_QEOPTION_FIRSTLIGHT);
   qeForever(); // turn control over to the engine

   // check for errors
   while((r=qeErrNext())<0)
   {
      s1=qeErrString(r);
      qePrintf("qeMainErr %s\n",s1);
      BRK(); // look in the log to find the error string
   } // while

   return 0;
} // qeMain()

#endif 
