#pragma once
#include <glm\glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace graphics_framework;
using namespace glm;


inline mesh createStoneGuardianMesh()
{
	auto statue = mesh(geometry("models/guardian.obj"));
	statue.get_transform().translate(vec3(168, 200, 230));
	statue.get_transform().scale = vec3(20, 20, 20);
	statue.get_transform().rotate(vec3(0, pi<float>(), 0));
	statue.get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	statue.get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	statue.get_material().set_shininess(65.0f);

	return statue;
}