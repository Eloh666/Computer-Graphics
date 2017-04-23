#pragma once
#include <glm\glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace graphics_framework;
using namespace glm;

inline bool shouldRenderMesh(mat4 VP, vec3 center, float radius)
{
	vec4 transformedCentre = VP * vec4(center, 1);
	vec3 PCN = vec3(
		transformedCentre.x / transformedCentre.z,
		transformedCentre.y / transformedCentre.z,
		transformedCentre.z / transformedCentre.z
		);
	if( abs(PCN.x) > 1 || abs(PCN.y) > 1 ||	abs(PCN.y) > 1 )
	{
		// dodgy, leaving it off
		return true;
	}
	return true;
}