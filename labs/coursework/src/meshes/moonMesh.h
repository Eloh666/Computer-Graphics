#include <glm\glm.hpp>
#include <graphics_framework.h>


using namespace std;
using namespace graphics_framework;
using namespace glm;


mesh createMoonMesh()
{
	auto moon = mesh(geometry("models/moon.obj"));
	moon.get_transform().scale = vec3(0.75f, 0.75f, 0.75f);
	moon.get_transform().translate(vec3(-400, 400, 350));
	moon.get_material().set_emissive(vec4(0.0, 0.0, 0.0, 0.0));
	moon.get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	moon.get_material().set_shininess(5000.0f);

	return moon;
}