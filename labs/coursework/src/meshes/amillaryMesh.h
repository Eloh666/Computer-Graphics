#include <glm\glm.hpp>
#include <graphics_framework.h>


using namespace std;
using namespace graphics_framework;
using namespace glm;


mesh createAmillaryMesh()
{
	auto statue = mesh(geometry("models/amillary.obj"));
	statue.get_transform().translate(vec3(150, 100, 0));
	statue.get_transform().scale = vec3(0.75, 0.75, 0.75);
	statue.get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	statue.get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	statue.get_material().set_shininess(65.0f);

	return statue;
}