#include <glm\glm.hpp>
#include <graphics_framework.h>


using namespace std;
using namespace graphics_framework;
using namespace glm;


mesh createcCherryTreeMesh()
{
	auto cherryTree = mesh(geometry("models/tree.obj"));
	cherryTree.get_transform().scale = vec3(0.05f, 0.05f, 0.05f);
	cherryTree.get_transform().translate(vec3(40, 43, -5));
	cherryTree.get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	cherryTree.get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	cherryTree.get_material().set_shininess(65.0f);

	return cherryTree;
}