// prov_singleton.cpp

#include "e_config.h"
#if BUILD_SINGLETON
#include "qe.h"
#include "qesl.h"
#include "Utils.h"
#include "Singleton.h"
#include "Building.h"

#define BuildingFac GetInstance()
enum BUILDING_TYPE { HOUSE = 0, BANK, SKYSCRAPPER, NUM_OF_BUILD_TYPES};

class BuildingFactory
{
public:
	static BuildingFactory* GetInstance();
	GameEntity* CreateObject(BUILDING_TYPE type);

private:
    BuildingFactory() {};
    BuildingFactory(const BuildingFactory&);
    BuildingFactory& operator=(const BuildingFactory&);
	
};
////////////////////
// IMPLEMENTATION
////////////////////
BuildingFactory* BuildingFactory::GetInstance()
{
    static BuildingFactory bf;
	return &bf;
}

GameEntity* BuildingFactory::CreateObject(BUILDING_TYPE type)
{
    GameEntity* entity = 0;

	switch(type)
    {
    case HOUSE:
        entity = new Building();
        break;
    case BANK:
        entity = new Building();
        break;
    case SKYSCRAPPER:
        entity = new Building();
        break;
    default:
        assert(!"Error: Invalid class ID passed to factory");
    }

    return entity;
}





int qeMain(int argc,chr *argv[])
{
   int r,fail=0;
   

   // turn on/off initial options
   qeOptionTurnOff(M_QEOPTION_FIRSTCAM|M_QEOPTION_FIRSTLIGHT);

 
   // print version info
   qePrintf("%s / qe %s / OpenGL / %s\n",
      __FILE__,qeVersion(),glGetString(GL_VERSION));


   // handle errors
   while((r=qeErrNext())<0)
      qePrintf("qeMainErr %s\n",qeErrString(r));

   // turn control over to the engine
   qeForever();

   // check for unbalanced allocations
   if(MemAllocCount())
      BRK();

   return 0;
}



#endif