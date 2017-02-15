#include <glm\glm.hpp>
#include <graphics_framework.h>


using namespace std;
using namespace graphics_framework;
using namespace glm;


mesh createTree()
{
	auto tree = mesh(geometry("models/palm.obj"));
	tree.get_transform().scale = vec3(2.5, 1.5, 2.5);
	tree.get_transform().translate(vec3(20, 44, 23));
	//tree.get_transform().rotate(vec3(half_pi<float>(), 0.0f, half_pi<float>()));
	tree.get_material().set_emissive(vec4(0.01f, 0.01f, 0.01f, 0.1f));
	tree.get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	tree.get_material().set_shininess(65.0f);

	return tree;
}