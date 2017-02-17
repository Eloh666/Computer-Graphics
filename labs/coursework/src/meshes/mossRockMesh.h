#include <glm\glm.hpp>
#include <graphics_framework.h>


using namespace std;
using namespace graphics_framework;
using namespace glm;


mesh createMossyRockMesh()
{
	auto statue = mesh(geometry("models/mossyRock.obj"));
	statue.get_transform().translate(vec3(175, 8.5, -10));
	statue.get_transform().scale = vec3(2.3, 2.3, 2.3);
	statue.get_transform().rotate(vec3(pi<float>() / 18, pi<float>(), 0.0f));
	statue.get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	statue.get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	statue.get_material().set_shininess(65.0f);

	return statue;
}