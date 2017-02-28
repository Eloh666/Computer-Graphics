#include <glm\glm.hpp>
#include <graphics_framework.h>


using namespace std;
using namespace graphics_framework;
using namespace glm;


effect createMultiLightEffect()
{
	effect eff;
	eff.add_shader("shaders/shader.vert", GL_VERTEX_SHADER);
	eff.add_shader("shaders/shader.frag", GL_FRAGMENT_SHADER);
	eff.add_shader("shaders/part_direction.frag", GL_FRAGMENT_SHADER);
	eff.add_shader("shaders/part_point.frag", GL_FRAGMENT_SHADER);
	eff.add_shader("shaders/part_spot.frag", GL_FRAGMENT_SHADER);
	eff.add_shader("shaders/part_shadow.frag", GL_FRAGMENT_SHADER);

	eff.build();
	return eff;
}

effect createMultiLightRemoveAlphaEffect()
{
	effect eff;
	eff.add_shader("shaders/shader.vert", GL_VERTEX_SHADER);
	eff.add_shader("shaders/remove_alpha.frag", GL_FRAGMENT_SHADER);
	eff.add_shader("shaders/part_direction.frag", GL_FRAGMENT_SHADER);
	eff.add_shader("shaders/part_point.frag", GL_FRAGMENT_SHADER);
	eff.add_shader("shaders/part_spot.frag", GL_FRAGMENT_SHADER);
	eff.add_shader("shaders/part_shadow.frag", GL_FRAGMENT_SHADER);
	eff.add_shader("shaders/part_remove_alpha.frag", GL_FRAGMENT_SHADER);
	eff.build();
	eff.build();
	return eff;
}
