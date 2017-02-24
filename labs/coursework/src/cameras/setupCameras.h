#include <glm\glm.hpp>
#include <graphics_framework.h>


using namespace std;
using namespace graphics_framework;
using namespace glm;

float viewPortSize = 3000.0f;

void setupFreeCam(free_camera &freeCam)
{
	freeCam.set_position(vec3(-250, 45, 45));
	freeCam.set_target(vec3(100, 100, 100));
	freeCam.set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, viewPortSize);
}

void setupChaseCamera(chase_camera &camera, mesh modelToFollow)
{
	camera.set_pos_offset(vec3(0.0f, 2.0f, 50.0f));
	camera.set_springiness(0.3f);
	camera.move(modelToFollow.get_transform().position, eulerAngles(modelToFollow.get_transform().orientation));
	camera.set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, viewPortSize);

}

void setupTargetCameras(vector<target_camera> &cameras, map<string, mesh> meshes)
{
	// katana
	cameras[0].set_position(vec3(30, 50, 8));
	cameras[0].set_target(meshes["katana"].get_transform().position + vec3(0, 2, 0));
	cameras[0].set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, viewPortSize);

	// guardian 1
	cameras[1].set_position(vec3(170, 240, 100));
	cameras[1].set_target(vec3(150, 230, 210));
	cameras[1].set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, viewPortSize);

	// global
	cameras[2].set_position(vec3(415, 65, -215));
	cameras[2].set_target(vec3(50, 185, 125));
	cameras[2].set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, viewPortSize);

	// global
	cameras[3].set_position(vec3(45, 100, -65));
	cameras[3].set_target(vec3(-200, 240, 150));
	cameras[3].set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, viewPortSize);

	// guardian 2
	cameras[4].set_position(vec3(220, 185, 145));
	cameras[4].set_target(vec3(90, 270, 280));
	cameras[4].set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, viewPortSize);

	// moon
	cameras[5].set_position(vec3(-125, 180, -295));
	cameras[5].set_target(vec3(75, 90, -30));
	cameras[5].set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, viewPortSize);

	// boat
	cameras[6].set_position(vec3(-195, 55, -130));
	cameras[6].set_target(vec3(125, 32, -20));
	cameras[6].set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, viewPortSize);
}

void handleFreeCameraMovement(free_camera &freeCam, float delta_time, vec3 &translation)
{

	static double ratio_width = quarter_pi<float>() / static_cast<float>(renderer::get_screen_width());
	static double ratio_height =
		(quarter_pi<float>() * renderer::get_screen_aspect()) / static_cast<float>(renderer::get_screen_height());
	static double cursor_x = 0.0;
	static double cursor_y = 0.0;
	double current_x;
	double current_y;
	// Get the current cursor position
	glfwGetCursorPos(renderer::get_window(), &current_x, &current_y);
	// Calculate delta of cursor positions from last frame
	double delta_x = current_x - cursor_x;
	double delta_y = current_y - cursor_y;
	// Multiply deltas by ratios - gets actual change in orientation
	delta_x *= ratio_width;
	delta_y *= ratio_height;
	// Rotate cameras by delta
	freeCam.rotate(delta_x, -delta_y);
	// Move camera
	freeCam.move(translation);
	// Update cursor pos
	cursor_x = current_x;
	cursor_y = current_y;
}

void handleChaseCameraMovement(chase_camera &camera, float delta_time, mesh modelToFollow)
{
	static double ratio_width = quarter_pi<float>() / static_cast<float>(renderer::get_screen_width());
	static double ratio_height =
		(quarter_pi<float>() * renderer::get_screen_aspect()) / static_cast<float>(renderer::get_screen_height());
	static double cursor_x = 0.0;
	static double cursor_y = 0.0;
	double current_x;
	double current_y;
	glfwGetCursorPos(renderer::get_window(), &current_x, &current_y);
	auto x = (current_x - cursor_x) * ratio_width * delta_time;
	auto y = -(current_y - cursor_y) * ratio_height * delta_time;

	camera.rotate(vec3(x, y, 0));
	camera.move(modelToFollow.get_transform().position , eulerAngles(modelToFollow.get_transform().orientation));

	cursor_x = current_x;
	cursor_y = current_y;
}