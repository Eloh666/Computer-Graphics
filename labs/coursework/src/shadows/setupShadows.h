#include <glm\glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace graphics_framework;
using namespace glm;

effect createShadowEffect()
{

	effect eff;
	eff.add_shader("shaders/basic_spotLight.vert", GL_VERTEX_SHADER);
	eff.add_shader("shaders/basic_spotLight.frag", GL_FRAGMENT_SHADER);
	eff.build();
	return eff;
}

// Calculates the shadows based on the positions of the spotlights
// This naive version does not yet take in consideration point lights
void updateShadowPostions(vector<shadow_map> &shadows, vector<spot_light> &spots)
{
	for (auto i = 0; i < spots.size(); ++i)
	{
		shadows[i].light_dir = spots[i].get_direction();
		shadows[i].light_position = spots[i].get_position();
	}
}

// Computes the light mvp matrix based on the number of shadows
vector<mat4> getLightProjectionMatrices(vector<shadow_map> &shadows, mat4 lightProjection, mat4 meshModel)
{
	vector<mat4> lightMVPs(shadows.size());
	for (auto i = 0; i < lightMVPs.size(); ++i)
	{
		lightMVPs[i] = lightProjection * shadows[i].get_view() * meshModel;
	}
	return lightMVPs;
}