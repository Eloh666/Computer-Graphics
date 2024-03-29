#pragma once
#include <glm\glm.hpp>
#include <graphics_framework.h>


using namespace std;
using namespace graphics_framework;
using namespace glm;


inline effect createMultiInstanceEffect()
{
	effect eff;
	eff.add_shader("shaders/multi_instance_shader.vert", GL_VERTEX_SHADER);
	eff.add_shader("shaders/normal_map.frag", GL_FRAGMENT_SHADER);
	eff.add_shader("shaders/part_direction.frag", GL_FRAGMENT_SHADER);
	eff.add_shader("shaders/part_normal_map.frag", GL_FRAGMENT_SHADER);
	eff.add_shader("shaders/part_point.frag", GL_FRAGMENT_SHADER);
	eff.add_shader("shaders/part_spot.frag", GL_FRAGMENT_SHADER);
	eff.add_shader("shaders/part_shadow.frag", GL_FRAGMENT_SHADER);
	eff.build();
	return eff;
}

inline effect createMultiInstanceRemoveAlphaEffect()
{
	effect eff;
	eff.add_shader("shaders/multi_instance_shader.vert", GL_VERTEX_SHADER);
	eff.add_shader("shaders/normal_remove_alpha.frag", GL_FRAGMENT_SHADER);
	eff.add_shader("shaders/part_direction.frag", GL_FRAGMENT_SHADER);
	eff.add_shader("shaders/part_point.frag", GL_FRAGMENT_SHADER);
	eff.add_shader("shaders/part_spot.frag", GL_FRAGMENT_SHADER);
	eff.add_shader("shaders/part_normal_map.frag", GL_FRAGMENT_SHADER);
	eff.add_shader("shaders/part_remove_alpha.frag", GL_FRAGMENT_SHADER);
	eff.add_shader("shaders/part_shadow.frag", GL_FRAGMENT_SHADER);
	eff.build();
	return eff;
}