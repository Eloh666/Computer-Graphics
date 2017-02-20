#include <glm\glm.hpp>
#include <graphics_framework.h>


using namespace std;
using namespace graphics_framework;
using namespace glm;


void initPointLights(vector<point_light> &points)
{
	// boat lantern
	points[0].set_position(vec3(131, 25, -90));
	points[0].set_light_colour(vec4(1, 0.6, 0, 1));
	points[0].set_range(75);

	// guardian gem point light
	points[1].set_position(vec3(170, 230, 217.5));
	points[1].set_light_colour(vec4(0.4, 0.6, 1, 1));
	points[1].set_range(350);

	// grave candles
	points[2].set_position(vec3(40, 45, 4.3));
	points[2].set_light_colour(vec4(1, 0.6, 0, 1));
	points[2].set_range(15);

	points[3].set_position(vec3(38, 45, 4.3));
	points[3].set_light_colour(vec4(1, 0.6, 0, 1));
	points[3].set_range(15);

	points[4].set_position(vec3(36, 45, 4.3));
	points[4].set_light_colour(vec4(1, 0.6, 0, 1));
	points[4].set_range(15);

	points[5].set_position(vec3(0, 0, 0));
	points[5].set_light_colour(vec4(0.4, 0.6, 1, 1));
	points[5].set_range(125);
}

void initSpotLights(vector<spot_light> &spots)
{
	// spot onto the moon surface
	spots[0].set_position(vec3(-300, 365, 290));
	spots[0].set_light_colour(vec4(1, 1, 1, 1));
	spots[0].set_range(4);
	spots[0].set_power(1.0f);
	spots[0].set_direction(vec3(-350, 385, 285));

	// moon general light
	spots[1].set_position(vec3(-350, 385, 285));
	spots[1].set_light_colour(vec4(1, 1, 1, 1));
	spots[1].set_range(15);
	spots[1].set_power(1.0f);
	spots[1].set_direction(vec3(600, 100, -350));

	//guardian blue spot
	spots[2].set_position(vec3(170, 230, 217.5));
	spots[2].set_light_colour(vec4(0.1, 0.95, 0.9, 1));
	spots[2].set_range(5);
	spots[2].set_power(1.0f);
	spots[2].set_direction(vec3(170, 230, -300));

}