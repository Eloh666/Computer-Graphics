#include <glm\glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace graphics_framework;
using namespace glm;

effect createShadowEffect()
{

	effect eff;
	//eff.add_shader("shaders/shader.vert", GL_VERTEX_SHADER);
	//eff.add_shader("shaders/basic_spotLight.frag", GL_FRAGMENT_SHADER);
	//eff.add_shader("shaders/part_spot.frag", GL_FRAGMENT_SHADER);

	eff.add_shader("shaders/shadow_map.vert", GL_VERTEX_SHADER);
	eff.add_shader("shaders/shadow_map.frag", GL_FRAGMENT_SHADER);

	eff.build();
	return eff;
}