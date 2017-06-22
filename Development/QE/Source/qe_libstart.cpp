#include "e_config.h"
#if BUILD_LIBSTART
#include "qe.h"

int qeMain(int argc, chr *argv[])
{
	int r, fail =0;
	chr *path;

	qePrintf("%s/ %s/ %s\n", __FILE__, glGetString(GL_VERSION), qeVersion());

	path ="..helium/libs/test.lib";
	if(qeLibNew("test",0,path) < 0)
	{	fail=1;
		qePrintf(" * qeLibNew failed : %s\n", path);
	}

	while((r=qeErrNext()) < 0)
		qePrintf("qeMainError %s\n", qeErrString(r));

	qeForever();
	
	if(MemAllocCount())
		BRK();

	return 0;

}

#endif