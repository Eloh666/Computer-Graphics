#ifndef graphics_framework.h
#include <graphics_framework.h>
#endif

#ifndef glm.hpp
#include <graphics_framework.h>
#include <glm\glm.hpp>
#endif

#include "../../../../../GraphicsCoursework/katana.h"

using namespace std;
using namespace graphics_framework;
using namespace glm;

map<string, mesh> meshes;
effect eff;
texture tex;
target_camera staticCam;
directional_light light;
vector<point_light> points(4);
vector<spot_light> spots(5);
map<string, material> materials;
free_camera freeCam;



bool load_content() {
	// Create plane mesh
	meshes["plane"] = mesh(geometry_builder::create_plane());

	meshes["katana"] = createKatana();

	// Load texture
	tex = texture("textures/grass.jpg");
	
	// Set lighting values
	light.set_ambient_intensity(vec4(0.3f, 0.3f, 0.3f, 1.0f));
	light.set_direction(vec3(1.0f, 1.0f, -1.0f));
	light.set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));


	// Load in shaders
	eff.add_shader("shaders/shader.vert", GL_VERTEX_SHADER);
	// Name of fragment shaders required
	vector<string> frag_shaders{ "shaders/shader.frag", "shaders/part_direction.frag",
		"shaders/part_point.frag", "shaders/part_spot.frag" };
	eff.add_shader(frag_shaders, GL_FRAGMENT_SHADER);
	// Build effect
	eff.build();

	// Set camera properties
	freeCam.set_position(vec3(0.0f, 5.0f, 10.0f));
	freeCam.set_target(vec3(0.0f, 0.0f, 0.0f));
	freeCam.set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, 1000.0f);
	glfwSetInputMode(renderer::get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	return true;
}

bool update(float delta_time) {
	// The ratio of pixels to rotation - remember the fov
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
	// Use keyboard to move the camera - WASD
	vec3 translation(0.0f, 0.0f, 0.0f);
	if (glfwGetKey(renderer::get_window(), 'W')) {
		translation.z += 5.0f * delta_time;
	}
	if (glfwGetKey(renderer::get_window(), 'S')) {
		translation.z -= 5.0f * delta_time;
	}
	if (glfwGetKey(renderer::get_window(), 'A')) {
		translation.x -= 5.0f * delta_time;
	}
	if (glfwGetKey(renderer::get_window(), 'D')) {
		translation.x += 5.0f * delta_time;
	}
	// Move camera
	freeCam.move(translation);
	// Update the camera
	freeCam.update(delta_time);
	// Update cursor pos
	cursor_x = current_x;
	cursor_y = current_y;
	return true;
}

bool render() {
	// Render meshes
	for (auto &e : meshes) {
		auto m = e.second;
		// Bind effect
		renderer::bind(eff);
		// Create MVP matrix
		auto M = m.get_transform().get_transform_matrix();
		auto V = freeCam.get_view();
		auto P = freeCam.get_projection();
		auto MVP = P * V * M;
		// Set MVP matrix uniform
		glUniformMatrix4fv(eff.get_uniform_location("MVP"), // Location of uniform
			1,                               // Number of values - 1 mat4
			GL_FALSE,                        // Transpose the matrix?
			value_ptr(MVP));                 // Pointer to matrix data
											 // *********************************
											 // Set M matrix uniform
		glUniformMatrix4fv(eff.get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));

		// Set N matrix uniform - remember - 3x3 matrix
		auto normal = m.get_transform().get_normal_matrix();
		glUniformMatrix3fv(eff.get_uniform_location("N"),
			1,
			GL_FALSE,
			value_ptr(normal));

		// Bind material
		renderer::bind(materials[e.first], "mat");

		// Bind point lights
		renderer::bind(spots, "spots");

		// Bind spot lights
		renderer::bind(points, "points");

		// Bind env light
		renderer::bind(light, "light");

		// Bind texture
		renderer::bind(tex, 0);

		// Set tex uniform
		glUniform1i(eff.get_uniform_location("tex"), 0);

		// Set eye position - Get this from active camera
		glUniform3fv(
			eff.get_uniform_location("eye_pos"),
			1,
			value_ptr(freeCam.get_position())
			);
		// Render mesh
		renderer::render(m);

		// *********************************

		// Render mesh
		renderer::render(m);
	}

	return true;
}

void main() {
	// Create application
	app application("52_Multifile_Shaders");
	// Set load content, update and render methods
	application.set_load_content(load_content);
	application.set_update(update);
	application.set_render(render);
	// Run application
	application.run();
}