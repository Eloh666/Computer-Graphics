#include <glm\glm.hpp>
#include <graphics_framework.h>


using namespace std;
using namespace graphics_framework;
using namespace glm;


inline mesh createLampMesh()
{
	auto lamp = mesh(geometry("models/lamp.obj"));
	lamp.get_transform().scale = vec3(1.2, 1.2, 1.2);
	lamp.get_transform().rotate(vec3(-half_pi<float>() / 15.0f, 0.0f, -half_pi<float>() / 10.0f));
	lamp.get_transform().translate(vec3(131, 22.5, -90));
	lamp.get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	lamp.get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	lamp.get_material().set_shininess(65.0f);

	return lamp;
}