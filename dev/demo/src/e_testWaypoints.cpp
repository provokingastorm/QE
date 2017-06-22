
// Copyright (C) 2007 Joe Linhoff, All Rights Reserved
// e_waypoints.cpp
#include "e_config.h"
#if BUILD_PROVOKING // compile this app
#include "qe.h"

qefnArcBallRec thearcBall;

//////For testing
bool doOnce = true;
//////


// JFL 29 Jul 06
int fncCam(void)
{
   // camera, axis, grid
   qefnCamArcBall(&thearcBall);
   qefnDrawAxes(1); 
   qefnDrawGrid(20,1);
   return 0;
} // fncCam()


////// Needs to go in a class
float playerSPEED = 5;              //Replace with persons velocity 
float currentXYZ[3] = {2,0,10};            //Current position of the player, substitute with m_XYZ or whatever



#if 0
int currentWaypoint;            //Current waypoint, different than path
int destinationWaypoint = 0;    //Destination waypoint
float destinationXYZ[3];        //Destination XYZ, needs to be added
bool atDestination = false;             //Might be needed, not sure yet.
int **currentpath;              //The current path that is being traversed
int dir;                        //Location of the destination in relation to the current waypoint (-1 or 1)
                                //I always go to the current waypoint and then move waypoint towards
                           //the end destination, this just makes it easier to know which way.

float waypoints[] =
{ // xzr
   0,0,3,           //0
   10,0,3,          //1
   10,-10,3,        //2
   0,-10,3,         //3
   10,10,3,         //4
}; // waypoints[]

int path1[] = {0,1,2,3,-1};
int path2[] = {0,1,4,-1};

int *paths[] =
{
   path1,
   path2,
   NUL // terminate
};
 


int DetermineEndLocation()
{
    float dest[3];  // 0 = X, 1 = Z, 2 = Radius
    dest[0] = waypoints[currentWaypoint*3];
    dest[1] = waypoints[currentWaypoint*3+1];
    dest[2] = waypoints[currentWaypoint*3+2];
    destinationXYZ[0] = dest[0] + (qeRand01()*(dest[2]*2)-dest[2]); //Ex:  Radius = 3, X = 5
                                                                //random(3*2) = 4.5
                                                                //5 + (4.5-3) = 6.5
    destinationXYZ[1] = 0;  
    destinationXYZ[2] = dest[1] + (qeRand01()*(dest[2]*2)-dest[2]); //Ex:  Radius = 3, Z = 10
                                                                //random(3*2) = 1
                                                                //10 + (1-3) = 8 (allows for negatives)
    return 0;
}

//Given a starting position that is already defined
//findPath finds a path including both the start and end
int findPath(int location)
{
   int i,num;
   float *v;
   int **pathp,*wayp;   
   int *findtarget; 
   bool found = false;

   for(pathp=paths;*pathp;pathp++)      //Traverse paths
   {
      qePrintf("path :");
      for(wayp=*pathp;;wayp++)
      {
         if(found)
            break;
         i=*wayp;
         if(i<0)
            break; // No more paths left
         if(i == currentWaypoint)  //Starting location found
         {
              for(findtarget=*pathp;;findtarget++)     //Search for end location
              {
                 i=*findtarget;
                 if(i<0)
                    break; // End location not found
                 if(i == location)
                 {
                    found = true;
                    currentpath = pathp;
                 }
              }
         }
      } // for
   } // for
    qePrintf("Current path = %d", currentpath);
    if(!found)
    {}

    for(wayp=*currentpath;;wayp++)     //Determines the direction to traverse the path
    {
       i=*wayp;
       if(i == currentWaypoint)
       {
           dir = 1;
           break;
       }
       if(i == location)
       {
           dir = -1;
           break;
       }
       if(i<0)
          break; // done
       qePrintf("%d ",i);
    } // for

   return 0;
}
#endif



int NewEndPoint(int &destinationWaypoint, int currentWaypoint)
{
    float deltaTime = qeTimeDelta();
	//m_RandomMovement = (qeRand01()*4)*deltaTime;
    while(destinationWaypoint == currentWaypoint)
		destinationWaypoint = qeRand()%38;
    return 0;
}

      

// given an xz, find nearest waypoint
int NearestWaypoint(float personX, float personZ, float* waypoints, int num)
{
    int i, current;
    float *v,x,z,dist;                      //Person coordinates
    float bestdist;                               


    //num=NUM(waypoints)/3; // 3 coords per xzr (hardcoded because there is NO reason this should ever chagne)
    x=personX;
    z=personZ;
    current=-1;
    bestdist=1e30;                          //Starts with a huuuuuge number as a placeholder
    for(v=waypoints,i=0;i<num;i++,v+=3)     //Finds the closest waypoint
    {
       dist=(x-v[0])*(x-v[0])+(z-v[1])*(z-v[1]);
       dist=MathSqrtf(dist);                //Go go distance formula
       if(bestdist>dist)                    //New closest waypoint
       {
          bestdist=dist;
          current=i;
       }
    } // for
    
    return current;
}







//=----------------------------------------------------------------
// JFL 25 Mar 07

int fncDraw(void)
{/*
   int i,num;
   //float *v,x,z,bestdist,dist;
   float *v;
   int **pathp,*wayp;


   //
   // DRAW WAYPOINTS POINTS
   //

   glColor4f(1,1,1,1);
   num=NUM(waypoints)/3; // 3 coords per xzr
   for(v=waypoints,i=0;i<num;i++,v+=3)
   {
      glTranslatef(v[0],0,v[1]);
      glRotatef(90,1,0,0);
      glutWireTorus(0.2,0.5,5,6);
      glRotatef(-90,1,0,0);
      glTranslatef(-v[0],0,-v[1]);
   } // for
    
    glTranslatef(currentXYZ[0],0,currentXYZ[2]);
    glutWireTorus(0.2,0.5,5,6);
    glTranslatef(-currentXYZ[0],0,-currentXYZ[2]);
*/

   // given an xz, find nearest waypoint
 /*
   x=2;
   z=10;
   currentWaypoint=-1;
   bestdist=1e30;
   for(v=waypoints,i=0;i<num;i++,v+=3)
   {
      dist=(x-v[0])*(x-v[0])+(z-v[1])*(z-v[1]);
      dist=MathSqrtf(dist);
      if(bestdist>dist)
      {
         bestdist=dist;
         currentWaypoint=i;
      }
   } // for
*/
 /* good stuff
   if(doOnce)
   {
       currentWaypoint = NearestWaypoint(2,10, waypoints);
       findPath(destinationWaypoint);
       DetermineEndLocation();
       doOnce = false;
       qePrintf("currentWaypoint = %i\n", currentWaypoint);
   }
   
   MovePerson(.01); //.01 is just used as a placeholder, deltaTime should go in its place
					//updates movement
   if(atDestination)
   {
       if(currentWaypoint == destinationWaypoint)
       {
          NewEndPoint();
          findPath(destinationWaypoint);
       }
       NextWaypoint();
   }
   end good stuff */
/*   
   for(pathp=paths;*pathp;pathp++)
   {
      qePrintf("path :");
      for(wayp=*pathp;;wayp++)
      {
         i=*wayp;
         if(i<0)
            break; // done
         qePrintf("%d ",i);
      } // for
      qePrintf("\n");
   } // for

      qePrintf("path :");
      for(wayp=*currentpath;;wayp++)
      {
         i=*wayp;
         if(i<0)
            break; // done
         qePrintf("%d ",i);
      } // for
      qePrintf("\n");
*/

   return 0;
} // fncDraw()
#if BUILD_WAYPOINTS
// qeMain()
// JFL 30 Jul 06
int qeMain(int argc,chr *argv[])
{
   qePrintf("%s / %s / %s\n",__FILE__,glGetString(GL_VERSION),qeVersion());

   // add camera and grid
   MEMZ(arcBall); // camera uses this record, zero it
   qeObjAddFncSP(fncCam,"N","f-cam"); // camera
   qeObjAddFncSP(fncDraw,"N","f-draw"); // draw

   // turn control over to the engine until the user closes the program
   qeForever();

   // check for unbalanced allocations
   if(MemAllocCount())
      BRK();

   return 0;
} // qeMain()
#endif

#endif // compile this app
// EOF
