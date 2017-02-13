#include <glm\glm.hpp>
#include <graphics_framework.h>


using namespace std;
using namespace graphics_framework;
using namespace glm;


vector<vec3> gePositions()
{
	return{
		vec3(-1.0f, 1.0f, -1.0f),
		vec3(-1.0f, -1.0f, -1.0f),
		vec3(1.0f, -1.0f, -1.0f),
		vec3(1.0f, -1.0f, -1.0f),
		vec3(1.0f, 1.0f, -1.0f),
		vec3(-1.0f, 1.0f, -1.0f),

		vec3(-1.0f, -1.0f, 1.0f),
		vec3(-1.0f, -1.0f, -10.0),
		vec3(-1.0f, 1.0f, -1.0f),
		vec3(-1.0f, 1.0f, -1.0f),
		vec3(-1.0f, 1.0f, 1.0f),
		vec3(-1.0f, -1.0f, 1.0f),

		vec3(1.0f, -1.0f, -1.0f),
		vec3(1.0f, -1.0f, 1.0f),
		vec3(1.0f, 1.0f, 1.0f),
		vec3(1.0f, 1.0f, 1.0f),
		vec3(1.0f, 1.0f, -1.0f),
		vec3(1.0f, -1.0f, -1.0f),

		vec3(-1.0f, -1.0f, 1.0f),
		vec3(-1.0f, 1.0f, 1.0f),
		vec3(1.0f, 1.0f, 1.0f),
		vec3(1.0f, 1.0f, 1.0f),
		vec3(1.0f, -1.0f, 1.0f),
		vec3(-1.0f, -1.0f, 1.0f),

		vec3(-1.0f, 1.0f, -1.0f),
		vec3(1.0f, 1.0f, -1.0f),
		vec3(1.0f, 1.0f, 1.0f),
		vec3(1.0f, 1.0f, 1.0f),
		vec3(-1.0f, 1.0f, 1.0f),
		vec3(-1.0f, 1.0f, -1.0f),

		vec3(-1.0f, -1.0f, -1.0f),
		vec3(-1.0f, -1.0f, 1.0f),
		vec3(1.0f, -1.0f, -1.0f),
		vec3(1.0f, -1.0f, -1.0f),
		vec3(-1.0f, -1.0f, 1.0f),
		vec3(1.0f, -1.0f, 1.0f)
	};
}

mesh createSkybox()
{
	geometry geom;
	geom.add_buffer(gePositions(), BUFFER_INDEXES::POSITION_BUFFER);
	auto skybox = mesh(geom);
	return skybox;
}