#include <glm\glm.hpp>
#include <graphics_framework.h>


using namespace std;
using namespace graphics_framework;
using namespace glm;


effect createNormalMapEffect()
{
	effect eff;
	eff.add_shader("shaders/normal_map.vert", GL_VERTEX_SHADER);
	eff.add_shader("shaders/normal_map.frag", GL_FRAGMENT_SHADER);
	eff.add_shader("shaders/part_direction.frag", GL_FRAGMENT_SHADER);
	eff.add_shader("shaders/part_normal_map.frag", GL_FRAGMENT_SHADER);
	eff.add_shader("shaders/part_point.frag", GL_FRAGMENT_SHADER);
	eff.add_shader("shaders/part_spot.frag", GL_FRAGMENT_SHADER);
	eff.add_shader("shaders/part_shadow.frag", GL_FRAGMENT_SHADER);
	eff.build();
	return eff;
}