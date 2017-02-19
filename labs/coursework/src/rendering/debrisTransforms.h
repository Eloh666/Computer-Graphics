#include <glm\glm.hpp>
#include <graphics_framework.h>


using namespace std;
using namespace graphics_framework;
using namespace glm;


void createSpheresTransforms(vector<mat4> &modelMatrices, int amount, vec3 fulcrum, float scaleSize)
{
	// Generate a large list of semi-random model transformation matrices

	srand(glfwGetTime()); // initialize random seed	
	GLfloat radius = 75.0f;
	GLfloat offset = 55.0f;
	for (GLuint i = 0; i < amount; i++)
	{
		glm::mat4 model;
		GLfloat angle = (GLfloat)i / (GLfloat)amount * 360.0f;
		GLfloat displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
		GLfloat x = sin(angle) * radius + displacement;
		displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
		GLfloat y = -2.5f + displacement * 0.2f; // Keep height of asteroid field smaller compared to width of x and z
		displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
		GLfloat z = cos(angle) * radius + displacement * 0.25;
		model = glm::translate(model, fulcrum + glm::vec3(x, y, z));

		// 2. Scale: Scale between 0.05 and 0.25f
		GLfloat scale = (rand() % 20) / 500.0f + scaleSize;
		model = glm::scale(model, glm::vec3(scale));

		// 3. Rotation: add random rotation around a (semi)randomly picked rotation axis vector
		GLfloat rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// 4. Now add to list of matrices
		modelMatrices[i] = model;
	}
}