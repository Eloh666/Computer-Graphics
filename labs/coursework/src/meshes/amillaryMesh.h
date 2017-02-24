#include <glm\glm.hpp>
#include <graphics_framework.h>


using namespace std;
using namespace graphics_framework;
using namespace glm;


mesh createAmillaryMesh()
{
	auto amillaryBody = mesh(geometry("models/amillary.obj"));
	amillaryBody.get_transform().translate(vec3(150, 100, 0));
	amillaryBody.get_transform().scale = vec3(0.75, 0.75, 0.75);
	amillaryBody.get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	amillaryBody.get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	amillaryBody.get_material().set_shininess(100.0f);
	return amillaryBody;
}

void generateAmillaryRings(vector<mat4> &modelMatrices)
{
	for (auto i = 0; i < modelMatrices.size(); i++)
	{
		modelMatrices[i] = translate(modelMatrices[i], vec3(20, 150, 23));
		modelMatrices[i] = glm::scale(modelMatrices[i], vec3(0.75, 0.75, 0.75) + vec3(1, 1, 1) * static_cast<float> (i + 1));
	}
}