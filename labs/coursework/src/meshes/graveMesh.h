#pragma once
#include <glm\glm.hpp>
#include <graphics_framework.h>


using namespace std;
using namespace graphics_framework;
using namespace glm;


inline mesh createGraveMesh()
{
	auto grave = mesh(geometry("models/grave.obj"));
	grave.get_transform().translate(vec3(38, 43, 6));
	grave.get_transform().scale = vec3(0.05f, 0.05f, 0.05f);
	grave.get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	grave.get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	grave.get_material().set_shininess(65.0f);

	return grave;
}