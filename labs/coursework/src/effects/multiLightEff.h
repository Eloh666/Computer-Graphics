#include <glm\glm.hpp>
#include <graphics_framework.h>


using namespace std;
using namespace graphics_framework;
using namespace glm;


effect createMultiLightEffect()
{
	effect eff;
	eff.add_shader("shaders/shader.vert", GL_VERTEX_SHADER);
	vector<string> frag_shaders{ "shaders/shader.frag", "shaders/part_direction.frag",
		"shaders/part_point.frag", "shaders/part_spot.frag", "shaders/part_shadow.frag" };
	eff.add_shader(frag_shaders, GL_FRAGMENT_SHADER);
	eff.build();
	return eff;
}

effect createMultiLightRemoveAlphaEffect()
{
	effect eff;
	eff.add_shader("shaders/shader.vert", GL_VERTEX_SHADER);
	vector<string> frag_shaders{ "shaders/remove_alpha.frag", "shaders/part_direction.frag",
		"shaders/part_point.frag", "shaders/part_spot.frag", "shaders/part_remove_alpha.frag" };
	eff.add_shader(frag_shaders, GL_FRAGMENT_SHADER);
	eff.build();
	return eff;
}