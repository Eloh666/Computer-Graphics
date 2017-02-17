#include <glm\glm.hpp>
#include <graphics_framework.h>


using namespace std;
using namespace graphics_framework;
using namespace glm;


mesh createStoneSwordMesh()
{
	auto statue = mesh(geometry("models/stoneSword.obj"));
	statue.get_transform().translate(vec3(-100, 80, 15));
	statue.get_transform().scale = vec3(0.1, 0.1, 0.1);
	statue.get_transform().rotate(vec3(-half_pi<float>(), pi<float>(), 0.0f));
	statue.get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	statue.get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	statue.get_material().set_shininess(65.0f);

	return statue;
}