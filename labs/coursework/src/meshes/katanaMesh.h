#include <glm\glm.hpp>
#include <graphics_framework.h>


using namespace std;
using namespace graphics_framework;
using namespace glm;


mesh createKatanaMesh()
{
	auto katana = mesh(geometry("models/katana.obj"));
	katana.get_transform().scale = vec3(0.1f, 0.1f, 0.1f);
	//katana.get_transform().translate(vec3(0.0f, 7.5f, -30.0f));
	katana.get_transform().rotate(vec3(0.0f, 0.0f, half_pi<float>()));
	katana.get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	katana.get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	katana.get_material().set_shininess(65.0f);

	return katana;
}