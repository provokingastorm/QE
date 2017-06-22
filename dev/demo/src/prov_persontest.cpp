#include "e_config.h"
#if BUILD_PERSONTEST
#include "qe.h"
#include "../provoking/Person.h"
#include "../provoking/WorldBuilder.h"
#include "../provoking/LoadEnvironment.h"
#include "../provoking/Camera.h"
#include "../provoking/Cloud.h"
#include "../provoking/AnimatedMesh.h"
#include "../provoking/ResourceManager.h"
#include "../provoking/SceneObject.h"

using namespace Environment;

Person *g_Person;
Camera *g_Camera;
Cloud *g_Cloud;
SceneObject *g_Land;
WorldBuilder *g_WorldBuilder;

void Init()
{
    g_Camera = new Camera();
    g_Cloud = new Cloud();
    g_Person = new Person();
    g_Land = new SceneObject();
    g_WorldBuilder = new WorldBuilder();
    g_Person->Init(0.0f, 0.0f, 0.0f);
    g_Cloud->Init(0.0f, 0.0f, 0.0f);
    g_Land->Init(0.0f, 10.0f, 0.0f);
    g_Camera->Init(0.0f, 0.0f, 0.0f);
    g_Camera->AttachFollowObj(g_Cloud);
    g_WorldBuilder->Init();

    MeshScan();

    Mesh *mesh = g_ResourceManager.GetMesh("tinydude");
    AnimatedMesh *copiedMesh = static_cast<AnimatedMesh*>(mesh);
    if(copiedMesh != NULL)
    {
        g_Person->SetMesh(copiedMesh);
    }
    g_Person->LoadData();

    mesh = g_ResourceManager.GetMesh("cb_face");
    if(mesh)
        g_Land->SetMesh(mesh);

    mesh = g_ResourceManager.GetMesh("cloud");
    if(mesh)
        g_Cloud->SetMesh(mesh);

    mesh = g_ResourceManager.GetMesh("tree01");
    if(mesh)
        g_Cloud->SetFaceMesh(mesh);
}

void Update()
{
    float deltaTime = qeTimeDelta();
    g_Camera->Update(deltaTime);
    g_Cloud->Update(deltaTime);
    g_Person->Update(deltaTime);
    g_Land->Update(deltaTime);
}

void Render() 
{
    g_Camera->Render();
    g_Cloud->Render();
    g_Land->Render();
    g_Person->Render();
}


int qeMain(int argc,chr *argv[])
{
#if DEBUG
    qeLogf("\n//-------------------- Game Start -----------------------------\n");
#endif

	int numOfErrors = 0;

    // check for errors
    numOfErrors = qeErrNext();
    // print all errors to screen 
    while(numOfErrors < 0)
       qePrintf("qeMainErr: %s\n",qeErrString(numOfErrors));

    Init();

    // setup initial engine object list
    qeOptionTurnOff(M_QEOPTION_FIRSTCAM|M_QEOPTION_FIRSTLIGHT);
    // enables fullscreen mode
#if FULLSCREEN
    qeFullscreen(1);
#endif

    // add engine obj (N=static-name/not-copied I=indestructable)
    if(!(qeObjAddFncSP(Update, "NI","update")))
        BRK();
    // add engine obj (N=static-name/not-copied I=indestructable)
    if(!(qeObjAddFncSP(Render, "NI","update")))
        BRK();

    // turn control over to the engine until the user closes the program
    qeForever();

#if DEBUG
    qeLogf("\n//-------------------- Game Over -----------------------------\n");
#endif
   return 0;
}


#endif