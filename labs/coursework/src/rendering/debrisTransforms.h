#include <glm\glm.hpp>
#include <graphics_framework.h>


using namespace std;
using namespace graphics_framework;
using namespace glm;


void createSpheresTransforms(vector<mat4> &modelMatrices, int amount, vec3 fulcrum, float scaleSize)
{
	// Generate a large list of semi-random model transformation matrices

	srand(glfwGetTime()); // initialize random seed	
	GLfloat radius = 100.0f;
	GLfloat offsetX = 35.0f;
	GLfloat offsetY = 15.0f;
	GLfloat offsetZ = 30.0f;
	for (GLuint i = 0; i < amount; i++)
	{
		glm::mat4 model;
		GLfloat angle = (GLfloat)i / (GLfloat)amount * 360.0f;
		GLfloat displacement = (rand() % (GLint)(2 * offsetX * 100)) / 100.0f - offsetX;
		GLfloat x = sin(angle) * radius + displacement;
		displacement = (rand() % (GLint)(2 * offsetY * 100)) / 100.0f - offsetY;
		GLfloat y = -2.5f + displacement * 0.2f; // Keep height of asteroid field smaller compared to width of x and z
		displacement = (rand() % (GLint)(2 * offsetZ * 100)) / 100.0f - offsetZ;
		GLfloat z = cos(angle) * radius + displacement;
		model = glm::translate(model, fulcrum + glm::vec3(x, y, z));

		// 2. Scale: Scale between 0.05 and 0.25f
		GLfloat Xscale = (rand() % 20) / 75.0f + scaleSize;
		GLfloat Yscale = (rand() % 20) / 75.0f + scaleSize;
		GLfloat Zscale = (rand() % 20) / 75.0f + scaleSize;
		model = glm::scale(model, glm::vec3(Xscale, Yscale, Zscale));

		// 3. Rotation: add random rotation around a (semi)randomly picked rotation axis vector
		GLfloat rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// 4. Now add to list of matrices
		modelMatrices[i] = model;
	}
}