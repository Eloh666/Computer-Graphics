#pragma once
#include <glm\glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace graphics_framework;
using namespace glm;

inline effect createShadowEffect()
{

	effect eff;
	eff.add_shader("shaders/shadow_map.vert", GL_VERTEX_SHADER);
	eff.add_shader("shaders/shadow_map.frag", GL_FRAGMENT_SHADER);

	eff.build();
	return eff;
}