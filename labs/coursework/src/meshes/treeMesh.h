#include <glm\glm.hpp>
#include <graphics_framework.h>


using namespace std;
using namespace graphics_framework;
using namespace glm;


inline mesh createVioletTreeMesh()
{
	auto tree = mesh(geometry("models/violet.obj"));
	tree.get_transform().scale = vec3(2.0, 1.0, 2.0);
	tree.get_transform().translate(vec3(20, 74, 23));
	tree.get_material().set_specular(vec4(1, 0.75, 1, 1));
	tree.get_material().set_shininess(25.0f);
	tree.get_material().set_shininess(65.0f);

	return tree;
}

inline mesh createDeadTreeMesh()
{
	auto tree = mesh(geometry("models/deadTree.obj"));
	tree.get_transform().scale = vec3(0.005, 0.005, 0.005);
	//tree.get_transform().translate(vec3(-350, 0, -24));
	tree.get_transform().translate((vec3(-250, 45, 45)));
	tree.get_material().set_emissive(vec4(0.01f, 0.01f, 0.01f, 0.1f));
	tree.get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	tree.get_material().set_shininess(65.0f);

	return tree;
}

inline mesh createTreeMesh()
{
	auto tree = mesh(geometry("models/tree.obj"));
	tree.get_material().set_emissive(vec4(0.01f, 0.01f, 0.01f, 0.1f));
	tree.get_material().set_specular(vec4(0.1, 0.4, 0.1, 0.4));
	tree.get_material().set_shininess(65.0f);

	return tree;
}

inline void generateTreesTransforms(vector<mat4> &transforms)
{
	auto locations = vector<vec3>{
		vec3(285, 56, 89),
		vec3(278, 52, 102),
		vec3(298, 56, 89),
		vec3(278, 52, 102),
		vec3(250, 59, 115),
		vec3(195, 60, 127),
		vec3(180, 51, 95),
		vec3(172, 65, 112),
		vec3(124, 47, 67),
		vec3(126, 41, 98),
		vec3(127, 36, 118),
		vec3(123, 45, 139),
		vec3(104, 74, 143),
		vec3(89, 66, 128),
		vec3(85, 59, 96),
		vec3(63, 57, 87),
		vec3(46, 60, 99),
		vec3(91, 32, 60),
		vec3(40, 75, 115),
		vec3(45, 45, 135),
		vec3(25, 71, 175),
		vec3(15, 70, 145),
		vec3(6, 66, 142),
		vec3(34, 46, 142),
		vec3(119, 60, 112),
		vec3(40, 44, 137),
		vec3(-40, 35, 156),
		vec3(-26, 66, 169),
		vec3(-70, 60, 155),
		vec3(-88, 60, 154),
		vec3(-75, 62, 191),
		vec3(-53, 63, 215),
		vec3(32, 62, 224),
		vec3(14, 64, 253),
		vec3(6, 63, 226),
		vec3(10, 73, 203),
		vec3(38, 75, 285),
		vec3(49, 63, 213),
		vec3(106, 70, 350),
		vec3(80, 50, 337),
	};


	auto base = 0.35f;
	auto ceil = 1.0f;
	auto scale = base + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (ceil - base)));
	for (auto i = 0; i < locations.size(); i++)
	{
		mat4 model;
		model = translate(model, locations[i]);

		// randoms its spin
		GLfloat rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0, 0.5f, 0));

		// randoms the size of the tree
		model = glm::scale(model, vec3(scale));
		transforms[i] = model;
	}
}
