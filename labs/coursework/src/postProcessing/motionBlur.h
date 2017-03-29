#include <glm\glm.hpp>
#include <graphics_framework.h>


using namespace std;
using namespace graphics_framework;
using namespace glm;

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