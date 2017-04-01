#pragma once
#include <glm\glm.hpp>
#include <graphics_framework.h>


using namespace std;
using namespace graphics_framework;
using namespace glm;


inline effect createGrassEffect()
{
	effect eff;
	eff.add_shader("shaders/billboard.vert", GL_VERTEX_SHADER);
	eff.add_shader("shaders/billboard.geom", GL_GEOMETRY_SHADER);
	eff.add_shader("shaders/billboard.frag", GL_FRAGMENT_SHADER);
	// Build effect
	eff.build();
	return eff;
}
