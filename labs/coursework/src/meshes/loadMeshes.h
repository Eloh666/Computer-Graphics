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

vec3 scaleVecByVec(vec3 aaBB, vec3 scaleSize)
{
	vec3 result = aaBB;

	result.x *= scaleSize.x;
	result.y *= scaleSize.y;
	result.z *= scaleSize.z;

	return result;
}

inline void calculateMeshesRadiuses(map<string, mesh> &meshes, map<string, float> &radiuses)
{
	for (auto &e : meshes)
	{
		auto m = e.second;
		geometry geom = m.get_geometry();
		vec3 scl = m.get_transform().scale;
		vec3 maxPoint = scaleVecByVec(m.get_geometry().get_maximal_point(), scl);
		vec3 minPoint = scaleVecByVec(m.get_geometry().get_minimal_point(), scl);
		radiuses[e.first] = length((maxPoint - minPoint) * 0.5f);
	}
}
