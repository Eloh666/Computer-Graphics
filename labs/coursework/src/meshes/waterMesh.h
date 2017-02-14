
#include <glm\glm.hpp>
#include <graphics_framework.h>


using namespace std;
using namespace graphics_framework;
using namespace glm;


mesh createWaterMesh()
{
	auto waterMesh = mesh(geometry_builder::create_plane());
	waterMesh.get_transform().translate(waterMesh.get_transform().position + vec3(0, 5, 0));
	waterMesh.get_transform().scale += vec3(100, 100, 100);
	waterMesh.get_material().set_shininess(25);

	return waterMesh;
}