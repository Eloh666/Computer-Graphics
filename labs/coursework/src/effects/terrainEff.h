#include <glm\glm.hpp>
#include <graphics_framework.h>


using namespace std;
using namespace graphics_framework;
using namespace glm;


effect createTerrainEffect()
{
	effect eff;
	eff.add_shader("shaders/terrain.vert", GL_VERTEX_SHADER);
	eff.add_shader("shaders/terrain.frag", GL_FRAGMENT_SHADER);
	eff.add_shader("shaders/part_direction.frag", GL_FRAGMENT_SHADER);
	eff.add_shader("shaders/part_point.frag", GL_FRAGMENT_SHADER);
	eff.add_shader("shaders/part_spot.frag", GL_FRAGMENT_SHADER);
	eff.add_shader("shaders/part_weighted_texture_4.frag", GL_FRAGMENT_SHADER);
	eff.build();
	return eff;
}