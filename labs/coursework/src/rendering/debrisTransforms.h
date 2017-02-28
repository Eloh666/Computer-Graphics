#include <glm\glm.hpp>
#include <graphics_framework.h>


using namespace std;
using namespace graphics_framework;
using namespace glm;



// Generates the transformation for the debris field
// NOTE: This code is (a slighly modified) part of an instanciation tutorial. I take no credit for it.
void createFloatersTransforms(vector<mat4> &modelMatrices, int amount, vec3 fulcrum, float scaleSize)
{
	// Generate a large list of semi-random model transformation matrices

	srand(glfwGetTime()); // initialize random seed	
	auto radius = 100.0f;
	auto offsetX = 35.0f;
	auto offsetY = 15.0f;
	auto offsetZ = 30.0f;
	for (GLuint i = 0; i < amount; i++)
	{
		glm::mat4 model;
		auto angle = static_cast<float>(i) / static_cast<float>(amount) * 360.0f;
		auto displacement = (rand() % static_cast<int>(2 * offsetX * 100)) / 100.0f - offsetX;
		auto x = sin(angle) * radius + displacement;

		displacement = (rand() % static_cast<int>(2 * offsetY * 100)) / 100.0f - offsetY;
		auto y = -2.5f + displacement * 0.2f;

		displacement = (rand() % static_cast<int>(2 * offsetZ * 100)) / 100.0f - offsetZ;
		auto z = cos(angle) * radius + displacement;

		model = glm::translate(model, fulcrum + glm::vec3(x, y, z));

		// rotation
		GLfloat rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// scale
		auto Xscale = (rand() % 20) / 75.0f + scaleSize;
		auto Yscale = (rand() % 20) / 75.0f + scaleSize;
		auto Zscale = (rand() % 20) / 75.0f + scaleSize;
		model = glm::scale(model, glm::vec3(Xscale, Yscale, Zscale));

		// 4. Now add to list of matrices
		modelMatrices[i] = model;
	}
}