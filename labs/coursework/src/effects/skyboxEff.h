#include <glm\glm.hpp>
#include <graphics_framework.h>


using namespace std;
using namespace graphics_framework;
using namespace glm;


effect createSkyboxEffect()
{
	effect eff;
	eff.add_shader("shaders/skybox.vert", GL_VERTEX_SHADER);
	eff.add_shader("shaders/skybox.frag", GL_FRAGMENT_SHADER);
	// Build effect
	eff.build();
	eff.build();
	return eff;
}