#include <glm\glm.hpp>
#include <graphics_framework.h>


using namespace std;
using namespace graphics_framework;
using namespace glm;


mesh createVioletTreeMesh()
{
	auto tree = mesh(geometry("models/violet.obj"));
	tree.get_transform().scale = vec3(2.5, 1.5, 2.5);
	tree.get_transform().translate(vec3(20, 44, 23));
	//tree.get_transform().rotate(vec3(half_pi<float>(), 0.0f, half_pi<float>()));
	tree.get_material().set_emissive(vec4(0.01f, 0.01f, 0.01f, 0.1f));
	tree.get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	tree.get_material().set_shininess(65.0f);

	return tree;
}

mesh createDeadTreeMesh()
{
	auto tree = mesh(geometry("models/deadTree.obj"));
	tree.get_transform().scale = vec3(0.005, 0.005, 0.005);
	tree.get_transform().translate(vec3(-350, 40, -24));
	//tree.get_transform().rotate(vec3(half_pi<float>(), 0.0f, half_pi<float>()));
	tree.get_material().set_emissive(vec4(0.01f, 0.01f, 0.01f, 0.1f));
	tree.get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	tree.get_material().set_shininess(65.0f);

	return tree;
}

mesh createTreeMesh()
{
	auto tree = mesh(geometry("models/tree.obj"));
	tree.get_transform().scale = vec3(0.5, 0.5, 0.5);
	tree.get_transform().translate(vec3(-100, 50, 100));
	//tree.get_transform().rotate(vec3(half_pi<float>(), 0.0f, half_pi<float>()));
	tree.get_material().set_emissive(vec4(0.01f, 0.01f, 0.01f, 0.1f));
	tree.get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	tree.get_material().set_shininess(65.0f);

	return tree;
}

void getTreeLocations(vector<vec3> &modelMatrices)
{
	auto locations = vector<vec3>{
		vec3(295, 66, 89),
		vec3(278, 62, 102),
	};
}

void generateTreesTransforms(vector<mat4> &modelMatrices)
{
	auto base = 0.1f;
	auto ceil = 0.5f;
	auto scale = base + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (ceil - base)));
	for (auto &mat : modelMatrices)
	{
		// randoms the size of the tree
		mat = glm::scale(mat, vec3(scale));

		// randoms its spin
		GLfloat rotAngle = (rand() % 360);
		mat = glm::rotate(mat, rotAngle, glm::vec3(0, 0.5f,0));
	}
}
