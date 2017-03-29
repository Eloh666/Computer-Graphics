#include <glm\glm.hpp>
#include <graphics_framework.h>


using namespace std;
using namespace graphics_framework;
using namespace glm;


inline effect createBasicRainEffect()
{
	effect eff;
	eff.add_shader("shaders/basic_colour.vert", GL_VERTEX_SHADER);
	eff.add_shader("shaders/basic_colour.frag", GL_FRAGMENT_SHADER);
	// Build effect
	eff.build();
	return eff;
}


inline effect createRainComputeShader()
{
	effect eff;
	eff.add_shader("shaders/particle.comp", GL_COMPUTE_SHADER);
	// Build effect
	eff.build();
	return eff;
}