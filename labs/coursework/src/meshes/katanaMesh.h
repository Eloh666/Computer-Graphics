#include <glm\glm.hpp>
#include <graphics_framework.h>


using namespace std;
using namespace graphics_framework;
using namespace glm;


mesh createKatanaMesh()
{
	auto katana = mesh(geometry("models/katana.obj"));
	katana.get_transform().scale = vec3(0.1f, 0.1f, 0.1f);
	katana.get_transform().translate(vec3(40, 44, -5));
	//katana.get_transform().translate(vec3(-250, 45, 45));
	katana.get_transform().rotate(vec3(half_pi<float>(), 0.0f, half_pi<float>()));
	katana.get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 0.0f));
	katana.get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	katana.get_material().set_shininess(65.0f);

	return katana;
}