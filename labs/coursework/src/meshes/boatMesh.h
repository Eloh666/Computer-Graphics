#include <glm\glm.hpp>
#include <graphics_framework.h>


using namespace std;
using namespace graphics_framework;
using namespace glm;


mesh createBoatMesh()
{
	auto moon = mesh(geometry("models/boat.obj"));
	moon.get_transform().scale = vec3(10.75f, 10.75f, 10.75f);
	moon.get_transform().translate(vec3(160, 27, -80));
	//moon.get_transform().rotate(vec3(half_pi<float>(), 0.0f, half_pi<float>()));
	moon.get_material().set_emissive(vec4(0.0, 0.0, 0.0, 0.0));
	moon.get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	moon.get_material().set_shininess(5000.0f);

	return moon;
}