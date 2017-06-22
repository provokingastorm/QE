
#include "e_config.h"
#if BUILD_HELLOWORLD
#include "qe.h"

int qeMain(int argc, chr *argv[]){

	qePrintf("FILE NAME: %s /GL-VERSION: %s / QE-ENGINE VERSION: %s\n",					__FILE__,glGetString(GL_VERSION),qeVersion());
	qePrintf("Hello World !");
	
	qeForever();
	return 0;
}

#endif