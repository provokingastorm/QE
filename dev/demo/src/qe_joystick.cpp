// Copyright (C) 2006 Joe Linhoff, All Rights Reserved
// e_joystick.c
#include "e_config.h"
#if BUILD_JOYSTICK // compile this app
#include "qe.h"

// JFL 12 Aug 06
int fncEveryLoop(void)
{
   uns b,t1,t2,hat;
   int xyLeft[2];
   int xyRight[2];

   // read joystick button and axes
   b=qeInpJoyButton(0,0); // joystick 0, button 0
   t1=qeInpJoyButton(0,4); // joystick 0, button 0
   t2=qeInpJoyButton(0,5); // joystick 0, button 0
   xyLeft[0]=qeInpJoyAxis(0,0); // joystick 0, axis 0
   xyLeft[1]=qeInpJoyAxis(0,1); // joystick 0, axis 1
   xyRight[0]=qeInpJoyAxis(0,2); // joystick 0, axis 2
   xyRight[1]=qeInpJoyAxis(0,3); // joystick 0, axis 3
   hat=qeInpJoyHat(0,0);

   // print the values
   qePrintf("joyNum=%d b=%d t1=%d t2=%d left (%d, %d) right (%d, %d) " \
   "hat=0x%x\n",
   qeInpJoyNum(),b,t1,t2,xyLeft[0],xyLeft[1],xyRight[0],xyRight[1],hat);

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
   qePrintf("** qeObjAddFnc failed\n");

   // turn control over to qe until the user closes the program
   qeForever();
   return 0;
} // qeMain()
#endif // compile this app
// EOF