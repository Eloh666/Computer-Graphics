#include <glm\glm.hpp>
#include <graphics_framework.h>


using namespace std;
using namespace graphics_framework;
using namespace glm;


mesh createLampMesh()
{
	auto lamp = mesh(geometry("models/lamp.obj"));
	lamp.get_transform().scale = vec3(1.2, 1.2, 1.2);
	//lamp.get_transform().translate(vec3(37, 41.5, 0));
	lamp.get_transform().translate(vec3(158, 27, -90));
	lamp.get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	lamp.get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	lamp.get_material().set_shininess(65.0f);

	return lamp;
}