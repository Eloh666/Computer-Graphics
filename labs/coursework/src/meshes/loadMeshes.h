#pragma once
#include <graphics_framework.h>
#include "mossRockMesh.h"
#include "stoneGuardMesh.h"
#include "katanaMesh.h"
#include "graveMesh.h"
#include "glm/gtx/io.hpp"
#include "waterMesh.h"
#include "lampMesh.h"
#include "moonMesh.h"
#include "statueMesh.h"
#include "boatMesh.h"
#include "stoneSwordMesh.h"
#include "ruinsMesh.h"
#include "skyboxMesh.h"
#include "treeMesh.h"
#include "amillaryMesh.h"


using namespace std;
using namespace graphics_framework;
using namespace glm;


inline void loadMeshes(map<string, mesh> &meshes)
{
	meshes["waterBase"] = createWaterMesh();
	meshes["lantern"] = createLampMesh();
	meshes["moon"] = createMoonMesh();
	meshes["buddha"] = createStatueMesh();
	meshes["boat"] = createBoatMesh();
	meshes["grave"] = createGraveMesh();
	meshes["violet"] = createVioletTreeMesh();
	meshes["katana"] = createKatanaMesh();
	meshes["amillary"] = createAmillaryMesh();
	meshes["mossyRock"] = createMossyRockMesh();
	meshes["guardian"] = createStoneGuardianMesh();
	meshes["ruins"] = createRuinsMesh();
	meshes["stoneSword"] = createStoneSwordMesh();
	meshes["deadTree"] = createDeadTreeMesh();
	meshes["waterLantern"] = createLampMesh();

}

inline void loadSpecialRenderMeshes(map<string, mesh> &meshes)
{
	meshes["skybox"] = createSkybox();
	meshes["trees"] = createTreeMesh();
	meshes["crystal"] = mesh(geometry("models/meteor.obj"));
	meshes["amillaryRing"] = mesh(geometry("models/amillaryRing.obj"));
}
