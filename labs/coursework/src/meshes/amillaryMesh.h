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

void generateAmillaryRings(vector<mat4> &modelMatrices, vec3 corePos, float rotationCoefficient)
{

	vector<vec3> rotations{
		vec3(0.0f, 0.0f, -quarter_pi<float>()),
		vec3(quarter_pi<float>(), 0.0f, 0.0f),
		vec3(-quarter_pi<float>(), 0, quarter_pi<float>()),
		vec3(0.0f, 0.0f, -quarter_pi<float>()),
		vec3(quarter_pi<float>(), 0.0f, 0.0f)
	};


	for (auto i = 0; i < modelMatrices.size(); i++)
	{
		mat4 model;
		modelMatrices[i] = translate(model, corePos);
		modelMatrices[i] = rotate(modelMatrices[i], rotationCoefficient, rotations[i % 5]);
		modelMatrices[i] = glm::scale(modelMatrices[i], vec3(0.25, 0.25, 0.25) + vec3(0.45, 0.45, 0.45) * static_cast<float> (i + 1));
	}
}