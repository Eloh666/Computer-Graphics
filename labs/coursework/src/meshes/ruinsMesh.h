#include <glm\glm.hpp>
#include <graphics_framework.h>


using namespace std;
using namespace graphics_framework;
using namespace glm;


inline mesh createRuinsMesh()
{
	auto ruins = mesh(geometry("models/ruin.obj"));
	ruins.get_transform().translate(vec3(50, 50, 45));
	ruins.get_transform().scale = vec3(0.12, 0.12, 0.12);
	ruins.get_transform().rotate(vec3(0, -half_pi<float>() * 0.5, 0));
	ruins.get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	ruins.get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));

	ruins.get_material().set_shininess(100);

	return ruins;
}