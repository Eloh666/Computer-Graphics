
#include <glm\glm.hpp>
#include <graphics_framework.h>
#include "terrainMesh.h"


using namespace std;
using namespace graphics_framework;
using namespace glm;


inline mesh createWaterMesh()
{

	const texture height_map("textures/waterHM.jpg");
	auto waterMesh = createTerrainMesh(height_map, 20, 20, 1);
	waterMesh.get_transform().translate(vec3(0, -50, 0));
	waterMesh.get_transform().scale += vec3(100, 100, 100);
	waterMesh.get_material().set_shininess(10);

	waterMesh.get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	waterMesh.get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	waterMesh.get_material().set_shininess(20.0f);

	return waterMesh;
}