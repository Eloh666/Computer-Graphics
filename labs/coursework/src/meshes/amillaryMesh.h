#include <glm\glm.hpp>
#include <graphics_framework.h>


using namespace std;
using namespace graphics_framework;
using namespace glm;


mesh createAmillaryMesh()
{
	auto amillaryBody = mesh(geometry("models/amillary.obj"));
	amillaryBody.get_transform().translate(vec3(150, 100, 0));
	amillaryBody.get_transform().scale = vec3(0.15, 0.15, 0.15);
	amillaryBody.get_material().set_emissive((vec4(0.1, 0.95, 0.9, 1)));
	amillaryBody.get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	amillaryBody.get_material().set_shininess(100.0f);
	return amillaryBody;
}

void generateAmillaryRings(vector<mat4> &modelMatrices, mat4 corePos, float rotationCoefficient)
{
	// individual rotation for each ring
	vector<vec3> rotations{
		vec3(0.0f, 0.0f, -quarter_pi<float>()),
		vec3(quarter_pi<float>(), 0.0f, 0.0f),
		vec3(0.0f, quarter_pi<float>(), 0.0f),
		vec3(0.0f, 0.0f, -quarter_pi<float>()),
		vec3(quarter_pi<float>(), 0.0f, 0.0f),
		vec3(0.0f, -quarter_pi<float>(), 0.0f),
	};


	for (auto i = 0; i < modelMatrices.size(); i++)
	{
		// base model
		mat4 model;
		modelMatrices[i] = model;
		// take up previous rotation
		for (auto j = i; j > 0; j--) {
			modelMatrices[i] = modelMatrices[j - 1] * modelMatrices[i];
		}
		modelMatrices[i] = rotate(modelMatrices[i], rotationCoefficient, rotations[i % 6]);
	}
	for (auto i = 0; i < modelMatrices.size(); i++)
	{	
		// move to the same position of the ancestor
		modelMatrices[i] = scale(corePos, vec3(7, 7, 7)) * modelMatrices[i];
		// scale based on the ring number
		modelMatrices[i] = scale(modelMatrices[i], vec3(0.35f, 0.35f, 0.35f) + vec3(0.35, 0.35, 0.35) * static_cast<float> (i + 1));
	}
}