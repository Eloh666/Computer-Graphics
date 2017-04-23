#pragma once
#include <glm\glm.hpp>
#include <graphics_framework.h>


using namespace std;
using namespace graphics_framework;
using namespace glm;


inline effect createGrassEffect()
{
	effect eff;
	eff.add_shader("shaders/basicPosition.vert", GL_VERTEX_SHADER);
	eff.add_shader("shaders/grass.geom", GL_GEOMETRY_SHADER);
	eff.add_shader("shaders/frustumCulling.geom", GL_GEOMETRY_SHADER);
	eff.add_shader("shaders/part_direction.frag", GL_FRAGMENT_SHADER);
	eff.add_shader("shaders/part_point.frag", GL_FRAGMENT_SHADER);
	eff.add_shader("shaders/part_spot.frag", GL_FRAGMENT_SHADER);
	eff.add_shader("shaders/grass.frag", GL_FRAGMENT_SHADER);
	// Build effect
	eff.build();
	return eff;
}

inline geometry generateGrassPositions(const texture &height_map, vec3 renderScale)
{
	geometry geom;

	float minOffset = 1.5f;
	float maxOffset = 2.5f;

	vec3 currentPatch(-renderScale.x*0.5f + minOffset, 0.0f, renderScale.z*0.5f - minOffset);

	auto positions = vector<vec3>();

	auto grassPatches = 0;

	while (currentPatch.x < renderScale.x*0.5f)
	{
		currentPatch.z = renderScale.z*0.5f - minOffset;

		while (currentPatch.z > -renderScale.z*0.5f)
		{
			currentPatch.y = 1;// GetHeightFromRealVector(currentPatch) - 0.3f;
			positions.push_back(currentPatch);
			grassPatches += 1;

			currentPatch.z -= minOffset + (maxOffset - minOffset)*float(rand() % 1000)*0.001f;
		}

		currentPatch.x += minOffset + (maxOffset - minOffset)*float(rand() % 1000)*0.001f;
	}

	return geom;
}