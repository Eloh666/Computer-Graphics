#include <glm\glm.hpp>
#include <graphics_framework.h>


using namespace std;
using namespace graphics_framework;
using namespace glm;


mesh createBoatMesh()
{
	auto boat = mesh(geometry("models/boat.obj"));
	boat.get_transform().scale = vec3(10.75f, 10.75f, 10.75f);
	boat.get_transform().translate(vec3(130, 25, -63));
	boat.get_transform().rotate(vec3(-half_pi<float>() / 15.0f, 0.0f, -half_pi<float>() / 10.0f));
	boat.get_material().set_emissive(vec4(0.0, 0.0, 0.0, 0.0));
	boat.get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	boat.get_material().set_shininess(5000.0f);

	return boat;
}