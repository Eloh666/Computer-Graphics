#pragma once
#include <glm\glm.hpp>
#include <graphics_framework.h>


using namespace std;
using namespace graphics_framework;
using namespace glm;


void initScreenQuad(geometry &screenQuad)
{
	// Create screen quad
	vector<vec3> positions{ vec3(-1.0f, -1.0f, 0.0f), vec3(1.0f, -1.0f, 0.0f), vec3(-1.0f, 1.0f, 0.0f),
		vec3(1.0f, 1.0f, 0.0f) };
	vector<vec2> tex_coords{ vec2(0.0, 0.0), vec2(1.0f, 0.0f), vec2(0.0f, 1.0f), vec2(1.0f, 1.0f) };
	screenQuad.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);
	screenQuad.add_buffer(tex_coords, BUFFER_INDEXES::TEXTURE_COORDS_0);
	screenQuad.set_type(GL_TRIANGLE_STRIP);
}

inline effect createMotionBlurEffect()
{
	effect eff;
	eff.add_shader("shaders/simpTex.vert", GL_VERTEX_SHADER);
	eff.add_shader("shaders/motion_blur.frag", GL_FRAGMENT_SHADER);
	eff.build();
	return eff;
}

inline effect createBasicTexturingEffect()
{
	effect eff;
	eff.add_shader("shaders/simpTex.vert", GL_VERTEX_SHADER);
	eff.add_shader("shaders/simpTex.frag", GL_FRAGMENT_SHADER);
	eff.build();
	return eff;

}

inline effect createBlurTexturingEffect()
{
	effect eff;
	eff.add_shader("shaders/simpTex.vert", GL_VERTEX_SHADER);
	eff.add_shader("shaders/blur.frag", GL_FRAGMENT_SHADER);
	eff.build();
	return eff;

}

inline effect createDOFTexturingEffect()
{
	effect eff;
	eff.add_shader("shaders/simpTex.vert", GL_VERTEX_SHADER);
	eff.add_shader("shaders/depth_of_field.frag", GL_FRAGMENT_SHADER);
	eff.build();
	return eff;

}

inline effect createGreyscaleEffect()
{
	effect eff;
	eff.add_shader("shaders/simpTex.vert", GL_VERTEX_SHADER);
	eff.add_shader("shaders/greyscale.frag", GL_FRAGMENT_SHADER);
	eff.build();
	return eff;

}