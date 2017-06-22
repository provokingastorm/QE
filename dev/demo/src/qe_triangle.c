// Copyright (C) 2006 Joe Linhoff, All Rights Reserved
// e_triangle.c
#include "e_config.h"
#if BUILD_TRIANGLE // compile this app
#include "qe.h"

// JFL 29 Jul 06
int fncDraw1(void)
{
   // draw one triangle
	glBegin(GL_TRIANGLES);
		glVertex3f( 0, 1, 0);
		glVertex3f(-1,-1, 0);
		glVertex3f( 1,-1, 0);
	glEnd();

	return 0;
} // fncDraw1()

// qeMain()
// Entry point
// JFL 30 Jul 06
int qeMain(int argc,chr *argv[])
{
   qePrintf("Project: %s\n",__FILE__);
   qePrintf("OpenGL version %s\n",glGetString(GL_VERSION));
   qePrintf("engine version %s\n",qeVersion());

   // add a function object to the processing list
   if(!qeObjAddFnc(fncDraw1))
      qePrintf("** qeObjAddFnc failed\n");

   // turn control over to the engine until the user closes the program
   qeForever();

   return 0;
} // qeMain()

#endif // compile this app
// EOF
