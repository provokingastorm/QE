#include "e_config.h"
#if BUILD_ENVTEST 
#include "qe.h"

#define BANK_NAME "bank"
#define BANK_PATH "libs/buildings.lib"

qeLibTag1 *tags;
qeLibScanRec librec;
qeLibChunk chunkrec;

void LoadLib();
int qeMain(int argc, char *argv[])
{
	tags = NULL;
    qeOptionTurnOff(M_QEOPTION_FIRSTCAM|M_QEOPTION_FIRSTLIGHT);

	LoadLib();

    qeForever();

    return 0;
}

void LoadLib()
{
    int r;
    qeLibScanRec *lib=&librec;
    qeLibChunk *chunk=&chunkrec;

    if((r=qeLibNew(BANK_NAME, 0, BANK_PATH))<0)
       BRK();

    if((r=qeLibLoad(BANK_NAME))<0)
       BRK();

    qeLibScanFirst(lib);
   
    qePrintf("Loaded Data:\n");
	while(qeLibScanNext(lib, chunk, NUL, NUL)>=0)
    {
        switch(chunk->type)
        {
        case C4TYPE_TAG1:
			qePrintf("tag found\n");
            if(!szcmp(chunk->name,BANK_NAME,NUL))
            {
                qePrintf("Bank Notes: \n");
                tags=(qeLibTag1*)chunk->data;
                qePrintf("%s\n", tags->tags);
                break;
            }
		case C4TYPE_GEO3:
			if(!szcmp(chunk->name, BANK_NAME, NUL))
			{
				qePrintf("geometry found\n");
				break;
			}
		default:
			break;
		}
	}
}
#endif