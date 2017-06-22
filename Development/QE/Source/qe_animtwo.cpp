// e_animtwo.cpp
#include "e_config.h"
#if BUILD_ANIMTWO 
#include "qe.h"
#include "qefn.h"
#include "qeinst.h"

enum {
   PERSTYPE_NONE,
   PERSTYPE_WOMAN,
   PERSTYPE_MAN,
};



// animation table
typedef struct {
    qeObj *idle;
    qeObj *walk;
    qeObj *jump;
} PersonAnimations;



class Person 
{
    qeObj *m_JointedObject; // jointed object
    qeObj *m_Skeleton;
public:
    uns8 persType;
    qeObj *m_Matrix;

    int Init(void); 
    void Shutdown(void); 
    int Start(char *libName,char *name);
    int Dispatch(int cprOp); 
    int Update(void); 
    int Render(void); 
};





Person man1;
Person man2;
Person woman1;
PersonAnimations personAnimTable;

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


int Person::Init(void)
{
    m_JointedObject = NULL;
    return 0;
} 



void Person::Shutdown(void)
{
    if(m_JointedObject)
    {
        // Note: you must free the jointed object
        qeObjJointedFree(m_JointedObject);
        m_JointedObject = NULL;
    }
} 




int Person::Start(char *libName, char *name)
{ 
    int r;
    qeLibChunk chunkmem,*chunk=&chunkmem; 
    char buffer[128];

    // find the root joint of the skeleton
    szcpycat(buffer, sizeof(buffer), libName, NULL,"/root",NULL); // create name buffer
    if((r=   qeLibFind(chunk,buffer,NULL,C4TYPE_SKL1   ))<0)
        goto BAIL;

    // notes:
    // name is not static, so object creation code must use 'n'
    // this depends on the root node of the skeleton being named 'root'

    // create jointed object nodes -- this is a complex set of nodes
    // creating with a 'b',NULL parameter causes obj to not be linked in anywhere
    if(!(this->m_JointedObject = qeObjJointedAddSP(chunk->libObj,"nb",name,NULL)))
        bret(-1);

    // create matrix object in the skeleton nodes
    szcpycat(buffer,sizeof(buffer),"m-",NULL,name,NULL); // create name buffer
    if(!(this->m_Matrix = qeObjAddInstSP(NULL,C4TYPE_MAT,"nsa",
        buffer,sizeof(qeInstNodeMat),qeObjChild(this->m_JointedObject))))
        BRK();

    // find jointed object's skeleton node
    if(!(this->m_Skeleton = qeObjJointedSkel(this->m_JointedObject)))
        BRK();

    // create dispatch cpr
    szcpycat(buffer,sizeof(buffer),"c-",NULL,name,NULL); // create name buffer
    if(!qeObjAddCprSP(this,"Cn",&Person::Dispatch,buffer))
        BRK();

    r=0;
BAIL:
    return r;
} 



int Person::Dispatch(int cprOp)
{  // co-process (keep the local frame small, allocate memory if needed)

   if(cprOp!=QECPROP_RUN) // co-process operation, only run is handled
      {BRK();return -1;}

   // this is the dispatch routine for all persons

   //
   // DISPATCH
   //

   qeCprSleep(2); // sleep 2 seconds

   // --- START ANI ---
   qeObjAniStart(m_Skeleton,personAnimTable.idle); // start anim
   qeObjAniSetFlags(m_Skeleton,M_QEOBJANI_LOOP); // override anim flag
   qeObjJointedOp(m_JointedObject,QEJOINTEDOP_VISIT); // turn display on

   qeCprSleep(4); // sleep 4 seconds

loop:

   // jump
   qeObjAniStart(m_Skeleton,personAnimTable.jump); // start anim
   qeObjAniSetFlags(m_Skeleton,M_QEOBJANI_LOOP); // override anim flag

   qeCprSleep(4); // sleep 4 seconds

   // idle
   qeObjAniStart(m_Skeleton,personAnimTable.idle); // start anim
   qeObjAniSetFlags(m_Skeleton,M_QEOBJANI_LOOP); // override anim flag

   qeCprSleep(14); // sleep 14 seconds
   goto loop;

   // -- b/c of goto, code never gets here

   qeCprSleep(5*60*60); // sleep for 5 minutes

//BAIL:
   return 0; // any return from a cpr deletes it
} 


int Person::Update(void)
{
   qeObj *o;
   qeObjInst *inst;

   // run through children of jointed object
   for(o=qeObjChild(m_JointedObject);o;o=qeObjNext(o))
   {
      if(QEOBJTYPE_INST!=qeObjType(o))
         continue; // skip if not an instance
      if(!(inst=(qeObjInst*)qeObjNode(o)))
         continue;
      switch(inst->typeC4)
      {
      case C4TYPE_ANI:
         qeInstVisit_ANI(inst->p,0);
         break;
      case C4TYPE_SKL:
         qeInstVisit_SKL(inst->p,0);
         break;
      } // switch
   } // for

   return 0;
} // Person::update()

// JFL 03 May 07
int gameDrawSkn(qeInstNodeSkn *skn)
{
   int r;
   qeLibGeo3 *geo3;
   int numf,*faces;
   float *uvs;
   chr *imgname;
   bit dontlight,wireframe=0;

   //
   // LOAD UP
   //

   switch(skn->geoType)
   {
   case C4TYPE_GEO3:
      if(!(geo3=(qeLibGeo3*)skn->geoLib))
         break;
      numf=geo3->numf;
      faces=geo3->faces;
      uvs=geo3->uvs;
      imgname=geo3->imgname;
      dontlight=geo3->libFlags&M_QELIBGEO3_DONTLIGHT?1:0;
      break; // C4TYPE_GEO3
   default:
      bret(-1); // type not handled yet
   } // switch

   //
   // MODE-DEPENDENT DRAW
   //

   if(wireframe)
   {
      glPolygonMode(GL_FRONT,GL_LINE);
      glColor3f(1,1,1);
      qeDrawFaces(skn->verts,faces,numf);
   }
   else if((r=qeTexBind(imgname))<0)
   {
      glColor3f(1,1,1);
      glShadeModel(GL_SMOOTH);
      glPolygonMode(GL_FRONT,GL_FILL);
      qeDrawFaces(skn->verts,faces,numf);
   }
   else if(!dontlight)
   {
      glPolygonMode(GL_FRONT,GL_FILL);
      glEnable(GL_TEXTURE_2D);
      glShadeModel(GL_SMOOTH);
      glColor3f(1,1,1);
      glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL); // RB416,444
      qeDrawLitUVFaces(skn->verts,skn->normals,faces,uvs,numf);
      glDisable(GL_TEXTURE_2D);
      qeTexBind(NULL);
   }
   else
   {
      glPolygonMode(GL_FRONT,GL_FILL);
      glEnable(GL_TEXTURE_2D);
      glShadeModel(GL_SMOOTH);
      glColor3f(1,1,1);
      glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL); // RB416,444
      qeDrawUVFaces(skn->verts,faces,uvs,numf);
      glDisable(GL_TEXTURE_2D);
      qeTexBind(NULL);
   }

   r=0;
BAIL:
   return r;
} // gameDrawSkn()





// JFL 04 Feb 07
int Person::Render(void)
{
    qeObj *object;
    qeObjInst *inst;
    qeInstNodeSkn *skn;
    qeInstNodeMat *mat;

    if(!(this->m_JointedObject))
      return 0; // no joint object

    // no more easy exits..

    glPushMatrix();

    object = qeObjChild(m_JointedObject);

    // run through all joint object children
    for(object; object != NULL ;object = qeObjNext(object))
    {
      if(QEOBJTYPE_INST!=qeObjType(object))
         continue; // skip if not an instance
      if(!(inst=(qeObjInst*)qeObjNode(object)))
         continue;


      switch(inst->typeC4)
      {

      case C4TYPE_MAT:
         if(!(mat=(qeInstNodeMat*)inst->p))
            break;
         qeGLM12f(mat->mat12);
         break; 


      case C4TYPE_SKN:
         if(!(skn=(qeInstNodeSkn*)inst->p))
            break;
         qeInstVisit_SKN(skn,M_QEINSTVISITSKN_DONTDRAW);
         gameDrawSkn(skn);
         break; 
      } // switch
    }

   glPopMatrix();
   return 0;
} 








int qeMain(int argc,chr *argv[])
{
    int r;
    qeLibChunk chunkmem,*chunk=&chunkmem; // pointer to mem for search result
    qeInstNodeMat *mat;

    MEMZ(arcBall);
    qeObjAddFncSP(fncCamGrid,"N","f-camGrid"); // camera

    if(qeLibNew("woman",M_QELIBNEW_LOAD|M_QELIBNEW_STARTIMAGES,"../helium/libs/pvkwoman.lib")<0)
      BRK();
    if(qeLibNew("man",M_QELIBNEW_LOAD|M_QELIBNEW_STARTIMAGES,"../helium/libs/pvkman.lib")<0)
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
    man1.Init(); // instance of character
    woman1.Init(); // instance of character
    MEMZ(personAnimTable); // animation table

    // find animation chunks
    if(qeLibFind(chunk,"anim/idle",NULL,C4TYPE_ANI2)>=0)
      personAnimTable.idle  =   chunk->libObj; // found animation
    if(qeLibFind(chunk,"anim/jump",NULL,C4TYPE_ANI2)>=0)
      personAnimTable.jump=chunk->libObj; // found animation

    // add CPR functions for dispatch (control over time)

    // -- man --
    if(man1.Start("man","man1")<0)
      BRK();
    man1.persType=PERSTYPE_MAN;
    if(!qeObjAddCFncSP(&man1,&Person::Update,"N","f-manUpdate"))
      BRK();
    if(!qeObjAddCFncSP(&man1,&Person::Render,"N","f-manDraw"))
      BRK();
    if((mat=(qeInstNodeMat*)qeObjExt(man1.m_Matrix)))
      M12BuildRTf(mat->mat12,0,0,0,1,0,0);

    // -- woman --
    if(woman1.Start("woman","woman1")<0)
      BRK();
    woman1.persType=PERSTYPE_WOMAN;
    if(!qeObjAddCFncSP(&woman1,&Person::Update,"N","f-womanUpdate"))
      BRK();
    if(!qeObjAddCFncSP(&woman1,&Person::Render,"N","f-womanDraw"))
      BRK();
    if((mat=(qeInstNodeMat*)qeObjExt(woman1.m_Matrix)))
      M12BuildRTf(mat->mat12,0,QE_DEGTORAD(90),0,-1,0,1);




    qeOptionTurnOff(M_QEOPTION_FIRSTCAM|M_QEOPTION_FIRSTLIGHT);
    qeForever();



    man1.Shutdown();
    woman1.Shutdown();

    // check for errors
    while((r=qeErrNext())<0)
    {
      qePrintf("qeMainErr %s\n",qeErrString(r));
      BRK(); // look in the log to find the error string
    } // while

   return 0;
} 




#endif 
